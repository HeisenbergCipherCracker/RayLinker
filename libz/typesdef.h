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
    const char* ss_server_port;
    const char* ss_server_address;
    const char* ss_password;
    const char* server_type;

}serverinfo;

struct ss_conn {
    int client_fd;
    int server_fd;
};




#endif
