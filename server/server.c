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

int main(int argc, char * argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        print_usage(stderr, argv[0]);
        return -1;
    }

    //Criar fifo de requests
    if(mkfifo("requests", 0660) != 0) {
        fprintf(stderr, "Error creating requests fifo\n");
        return -2;
    }

    ///TODO: Clean up this bit
    int num_room_seats = atoi(argv[1]);
    int num_ticket_offices = atoi(argv[2]);
    int open_time = atoi(argv[3]);

    //Inicializar mecanismos de sincronização
   if(init_sync() != 0) {
       fprintf(stderr, "Error initializing synchornization mechanisms!\n");
       return -3;
   }

    //Criar num_ticket_offices threads auxiliares
    pthread_t t_ids[num_ticket_offices];
    int i;
    for(i = 0; i < num_ticket_offices; ++i) {
        printf("Creating thread number %d\n", i);
        pthread_create(&(t_ids[i]), NULL, startWorking, NULL);
    }

    //Busy listen no fifo de requests
    listen_for_requests();
    //Colocar num buffer unitário para threads irem buscar

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
    //Fechar fifo de pedidos (feito na função de leitura o descritor de leitura)
    unlink("request");
    //Informar os threads que devem terminar
    set_worker_status(WORKER_STOP);
    //Aguardar que os threads terminem
    for(i = 0; i < num_ticket_offices; ++i) {
        printf("Waiting for thread number %d\n", i);
        if(pthread_join(t_ids[i], NULL) != 0) {
            printf("Error attempting to join thread %d\n", i);
        }
    }

    //
    if(finish_sync() != 0) {
        fprintf(stderr, "Error in closing synchronization mechanisms!\n");
    }

    return 0;
}

static unsigned int readline_until_char(int fd, char buffer[], char delim) {
    char read_char;
    unsigned int counter = 0;
    while(read(fd, &read_char, 1) != 0) {
        if(read_char == delim) {
           break;
        }

        if(counter >= MAX_MESSAGE_SIZE) {
            fprintf(stderr, "Attempting to read a message bigger than the expected maximum message size!\n");
            return -1;
        }

        buffer[counter++] = read_char;
    }

    //Closing the buffer with a null terminator
    buffer[counter] = '\0';

    return counter;
}

int listen_for_requests() {
    int fifo_read_fd = open("requests", O_RDONLY);
    char read_buffer[MAX_MESSAGE_SIZE];
    unsigned int n_chars_read = 0;

    if(fifo_read_fd == -1) {
        return -1;
    }

    //TODO: change loop condition
    while(1) {
        //Waits until potentially read data can be sent for reading
        wait_can_send_data_sem();
        //Reads data
        do {
            n_chars_read = readline_until_char(fifo_read_fd, read_buffer, '\n');
            if(n_chars_read == -1) {
                fprintf(stderr, "Error in reading message from request fifo\n");
                return -2;
            }
            if(n_chars_read == 0) {
                //printf("request fifo is not open for writing\n");
                //Sleeping for a bit to reduce waste of processing
                usleep(CLOSED_WRITE_FIFO_WAIT_DELAY_MS * 1000);
                continue;
            }
            printf("Concluded reading of message with %u chars: %s\n", n_chars_read, read_buffer);
        } while(n_chars_read == 0);
        //Writes it to buffer
        write_to_buffer(read_buffer);
        //Signals that there is data to read
        signal_has_data_sem();
    }

    return 0;
}

void print_usage(FILE * stream, char * progname) {
    fprintf(stream, "usage: %s <num_room_seats> <num_ticket_offices> <open_time>\n", progname);
}