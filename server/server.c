#include <stdio.h>
#include "server.h"
#include "worker.h"

int main(int argc, char * argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        print_usage(stderr, argv[0]);
        return -1;
    }

    //Criar fifo de requests

    //Criar num_ticket_offices threads auxiliares

    //Busy listen no fifo de requests
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

void print_usage(FILE * stream, char * progname) {
    fprintf(stream, "usage: %s <num_room_seats> <num_ticket_offices> <open_time>\n", progname);
}