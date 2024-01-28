#include <stdio.h>
#ifndef CONNECTION_H
#define CONNECTION_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"
#include "typesdef.h"
#include "settings.h"
#endif
//!shadow socks server
#ifndef SS_SERVER_ADDRESS

#define SS_SERVER_ADDRESS 0

#endif

#ifndef SS_SERVER_PORT
#define SS_SERVER_PORT 1
#endif
#ifndef SS_PASSWORD
#define SS_PASSWORD 1
#endif

int shadowsocksconn(const char* serveraddress,int serverport,const char* password){
    printf("fef");
    #if SS_SERVER_ADDRESS == 0 && SS_SERVER_PORT == 1 && SS_PASSWORD == 1
    #define SS_SERVER_ADDRESS serveraddress
    #define SS_SERVER_PORT serverport
    #define SS_PASSWORD password
    print_inf_message("setting the shadowsocks server...");
        int listen_fd;
        struct sockaddr_in listen_addr;
        if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        print_error_msg("connection creation failed with server");
        exit(EXIT_FAILURE);
    }
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(1080); 
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    print_inf_message("starting to listen on the server and port...");
    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) == -1) {
    perror("Bind failed");
    print_error_msg("binding failed with server");
    exit(EXIT_FAILURE);
    }
    if (listen(listen_fd, 10) == -1) {
    perror("Listen failed");
    print_error_msg("listen failed on the port");
    exit(EXIT_FAILURE);
    }
        while (1) {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        print_inf_message("starting to accept the client...");

        // Accept incoming connection
        if ((client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Accepted connection from client %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new Shadowsocks connection structure
        struct ss_conn *conn = (struct ss_conn *)malloc(sizeof(struct ss_conn));
        conn->client_fd = client_fd;

        // Connect to the Shadowsocks server
        if ((conn->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Server socket creation failed");
            close(client_fd);
            free(conn);
            continue;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SS_SERVER_PORT);

        if (inet_pton(AF_INET, SS_SERVER_ADDRESS, &server_addr.sin_addr) <= 0) {
            perror("Invalid address/Address not supported");
            close(client_fd);
            close(conn->server_fd);
            free(conn);
            continue;
        }

        if (connect(conn->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Connection to server failed");
            close(client_fd);
            close(conn->server_fd);
            free(conn);
            continue;
        }

        printf("Connected to Shadowsocks server %s:%d\n", SS_SERVER_ADDRESS, SS_SERVER_PORT);

        if (fork() == 0) {
            close(listen_fd);
            handle_client(conn);
        } else {
            close(client_fd);
            handle_server(conn); // Handle server in the same process
        }
    }

    close(listen_fd);
    #endif

}


void handle_client(struct ss_conn *conn) {
    char buffer[4096];
    ssize_t bytesRead;

    while ((bytesRead = read(conn->client_fd, buffer, sizeof(buffer))) > 0) {
        // Implement your Shadowsocks decryption logic here (not provided in this example)
        // Forward the decrypted data to the server
        write(conn->server_fd, buffer, bytesRead);
    }

    // Cleanup
    close(conn->client_fd);
    close(conn->server_fd);
    free(conn);
    exit(EXIT_SUCCESS);
}

void handle_server(struct ss_conn *conn) {
    char buffer[4096];
    ssize_t bytesRead;

    while ((bytesRead = read(conn->server_fd, buffer, sizeof(buffer))) > 0) {
        // Implement your Shadowsocks encryption logic here (not provided in this example)
        // Forward the encrypted data to the client
        write(conn->client_fd, buffer, bytesRead);
    }

    // Cleanup
    close(conn->server_fd);
    free(conn);
    exit(EXIT_SUCCESS);
}


//!eof