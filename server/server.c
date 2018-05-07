#include <stdio.h>
#include "server.h"
#include "worker.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

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

    //Criar num_ticket_offices threads auxiliares
    pthread_t t_ids[num_ticket_offices];
    int i;
    for(i = 0; i < num_ticket_offices; ++i) {
        printf("Creating thread number %d\n", i);
        pthread_create(&(t_ids[i]), NULL, startWorking, NULL);
        //TODO: Create the threads
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
    //Fechar fifo de respostas
    //Informar os threads que devem terminar
}

int listen_for_requests() {
    int fifo_read_fd = open("requests", O_RDONLY);

    if(fifo_read_fd == -1) {
        return -1;
    }

    //Complete

    return 0;
}

void print_usage(FILE * stream, char * progname) {
    fprintf(stream, "usage: %s <num_room_seats> <num_ticket_offices> <open_time>\n", progname);
}