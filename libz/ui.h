#include <stdio.h>
#include "common.h"
#ifndef UI_H
#define UI_H

#endif

#include "typesdef.h"


void print_network_info(){
    serverinfo inf =  get_server_address_info();
    const char* server_headers[] = {"Server", "Proxy Type", "Port", "Data"};
    printf("Server: %s\n", inf.ss_server_address);
    printf("Port: %d\n", inf.ss_server_port);
    printf("Password: %s\n", inf.server_type);
    const char* datas[] = {inf.ss_server_address,inf.ss_server_port,inf.server_type};
    size_t num_columns = sizeof(server_headers) / sizeof(server_headers[0]);
    size_t num_rows = sizeof(datas) / (num_columns * sizeof(datas[0]));
    create_table(datas,server_headers,num_columns,num_rows);
    
}