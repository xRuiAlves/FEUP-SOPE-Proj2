#include <stdio.h>
#include "server.h"
#include "worker.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include "synchronization.h"
#include "buffer.h"
#include "defs.h"
#include "parser.h"
#include <limits.h>
#include "seats.h"
#include <errno.h>
#include "sbook.h"
#include "slog.h"
#include <signal.h>

static pthread_t thread_ids[MAX_NUM_THREADS];
static unsigned int num_threads;

int main(int argc, char * argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        print_usage(stderr, argv[0]);
        return -1;
    }

    unsigned int num_room_seats = parse_unsigned_int(argv[1]);
    if(num_room_seats == UINT_MAX || num_room_seats == 0 || num_room_seats > MAX_ROOM_SEATS) {
        fprintf(stderr, "Invalid value for num_room_seats, must be non zero positive value smaller than MAX_ROOM_SEATS (%d)\n", MAX_ROOM_SEATS);
        print_usage(stderr, argv[0]);
        return -3;
    }
    
    initNrAvailableSeats(num_room_seats);
    unsigned int num_ticket_offices = parse_unsigned_int(argv[2]);
    if(num_ticket_offices == UINT_MAX || num_ticket_offices == 0) {
        fprintf(stderr, "Invalid value for num_ticket_offices, must be non zero positive value\n");
        print_usage(stderr, argv[0]);
        return -3;
    }
    unsigned int open_time = parse_unsigned_int(argv[3]);
    if(open_time == UINT_MAX || open_time == 0) {
        fprintf(stderr, "Invalid value for open_time, must be non zero positive value\n\nNOT YET USING THIS!!!\n");
        print_usage(stderr, argv[0]);
        return -3;
    }

    // Open log files
    open_slog_file();
    open_sbook_file();

    //Criar fifo de requests
    if(mkfifo(REQUEST_FIFO_NAME, 0660) != 0) {
        if (errno == EEXIST) {
            unlink(REQUEST_FIFO_NAME);
            mkfifo(REQUEST_FIFO_NAME, 0660);
        } else {
            fprintf(stderr, "Error creating requests fifo\n");
            return -2;
        }   
    }

    //Inicializar mecanismos de sincronização
   if(init_sync() != 0) {
       fprintf(stderr, "Error initializing synchornization mechanisms!\n");
       return -4;
   }

    //Criar num_ticket_offices threads auxiliares
    int i;
    num_threads = num_ticket_offices;
    for(i = 0; i < num_ticket_offices; ++i) {
        //printf("Creating thread number %d\n", i);
        if(pthread_create(&(thread_ids[i]), NULL, startWorking, NULL) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
        }
    }

    //Busy listen no fifo de requests
    setup_alarm(open_time);
    listen_for_requests(open_time);
    //Colocar num buffer unitário para threads irem buscar (feito acima)

    //Para cada thread:
    /**
     * 0 - Receber pedido
     *     a - Wait no semáforo de "data available to read" -> que é o main thread a dar signal quando lê do fifo
     *     b - (a-1??) Lock no mutex de leitura do buffer (sendo unitário não deverá ser tão problemático mas tem de se ver)
     * 1 - Validar pedido
     *     a - quantidade de lugares a reservar (num_wanted_seats) está na gama [1..MAX_CLI_SEATS]
     *     b - número de lugares preferidos está na gama [num_wanted_seats..MAX_CLI_SEATS]
     * 2 - Se for válido, executar pedido
     *     a -
     * 3 - Responder ao cliente pelo fifo criado por ele (ver valores de erro) com uma resposta no formato <n_lugares_reservados> <lista de lugares reservados> se não ocorrer erro
     */

    //Fim do main thread:
    
    return 0;
}

static int readline_until_char(int fd, char buffer[], char delim) {
    char read_char;
    int counter = 0;
    ssize_t read_status = 0;
    while((read_status = read(fd, &read_char, 1)) > 0) {
        if(read_char == delim) {
           break;
        }

        if(counter >= MAX_MESSAGE_SIZE) {
            fprintf(stderr, "Attempting to read a message bigger than the expected maximum message size!\n");
            return -1;
        }

        buffer[counter++] = read_char;
    }

    if(read_status == -1) {
        if(errno == EAGAIN) {
            return 0;
        } else {
            return -2;
        }
    }

    //Closing the buffer with a null terminator
    buffer[counter] = '\0';

    return counter;
}

int listen_for_requests(int open_time_s) {
    int fifo_read_fd = open(REQUEST_FIFO_NAME, O_RDONLY | O_NONBLOCK);
    char read_buffer[MAX_MESSAGE_SIZE];
    int n_chars_read = 0;

    if(fifo_read_fd == -1) {
        return -1;
    }

    while(1) {
        //Waits until potentially read data can be sent for reading
        if (is_buffer_full()) {
            continue;
        }
        else {
            wait_until_buffer_empty();
        }

        //Reads data
        do {
            n_chars_read = readline_until_char(fifo_read_fd, read_buffer, '\n');
            if(n_chars_read < 0) {
                fprintf(stderr, "Failure in reading message from request fifo with error level %d\n", n_chars_read);
                return -2;
            }
            if(n_chars_read == 0) {
                //Sleeping for a bit to reduce waste of processing
                usleep(CLOSED_WRITE_FIFO_WAIT_DELAY_MS * 1000);
                continue;
            }
        } while(n_chars_read == 0);

        //Because it is possible to exit the internal loop with nothing read (time has ended)
        if(n_chars_read > 0) {
            //Writes it to buffer
            write_to_buffer(read_buffer);
            //Signals that there is data to read
            signal_buffer_full();
        } 
    }

    close(fifo_read_fd);
    return 0;
}

void print_usage(FILE * stream, char * progname) {
    fprintf(stream, "usage: %s <num_room_seats> <num_ticket_offices> <open_time>\n", progname);
}

void close_server() {
    //Fechar fifo de pedidos (é fechado na função de leitura o descritor de leitura)
    unlink(REQUEST_FIFO_NAME);
    //Informar os threads que devem terminar
    set_worker_status(WORKER_STOP);
    //Aguardar que os threads terminem
    int i;
    for(i = 0; i < num_threads; ++i) {
        //printf("Waiting for thread number %d\n", i);
        if(pthread_join(thread_ids[i], NULL) != 0) {
            fprintf(stderr, "Error attempting to join thread %d\n", i);
        }
    }

    //Fecho de mecanismos de sincronização
    if(finish_sync() != 0) {
        fprintf(stderr, "Error in closing synchronization mechanisms!\n");
    }

    // Close log files
    writeServerClosing();
    close_slog_file();
    close_sbook_file();
    
    exit(0);
}

void setup_alarm(unsigned int time_out) {
    alarm(time_out);
    signal(SIGALRM, close_server);
}