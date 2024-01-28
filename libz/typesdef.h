#include <stdio.h>
#include <stdlib.h>
#ifndef TYPESDEF_H
#define TYPESDEF_H

typedef struct{
    int* numeric;
    char* string;
    void* voidptr;
    float* decimal;
    double* dnumeric;
}List;


typedef struct{
    int* ss_server_port;
    char* ss_server_address;
    char* ss_password;
}serverifno;

struct ss_conn {
    int client_fd;
    int server_fd;
};


#endif
