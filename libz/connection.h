#include <stdio.h>
#ifndef CONNECTION_H
#define CONNECTION_H
// g++ -o con raylinker.cc

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"
#include "typesdef.h"
#include "settings.h"
#include <errno.h>
#endif
void shadowsocks_conn(const char* serveraddress,int serverport,const char* password){
    #define SS_SERVER_ADDRESS serveraddress
    #define SS_SERVER_PORT serverport
    #define SS_PASSWORD password
    printf(WARNING_SIGN,"testing connection to server");
    fflush(stdout);
        int listen_fd;
        struct sockaddr_in listen_addr;
        if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        printf(ERROR_SIGN,"failed to create a socket");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(1080); 
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    printf(INFO_SIGN,"starting to listen on the server and port...");
    fflush(stdout);
    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) == -1) {
    perror("Bind failed");
    printf(ERROR_SIGN,"binding failed with server");
    fflush(stdout);
    
    exit(EXIT_FAILURE);
    }
    if (listen(listen_fd, 10) == -1) {
    perror("Listen failed");
    printf(ERROR_SIGN,"listen failed on the port");
    fflush(stdout);
    exit(EXIT_FAILURE);
    }
        while (1) {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        printf(INFO_SIGN,"starting to accept the client...");
        fflush(stdout);

        if ((client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Accept failed");
            printf("accept failed");
            fflush(stdout);
            continue;
        }
    else {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("\n\rClient connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        printf("\nAccepted connection from client %s:%d\n", 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        fflush(stdout);
    }

        // Create a new Shadowsocks connection structure
        struct ss_conn *conn = (struct ss_conn *)malloc(sizeof(struct ss_conn));
        conn->client_fd = client_fd;

        // Connect to the Shadowsocks server
        if ((conn->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Server socket creation failed");
            fflush(stdout);
            close(client_fd);
            free(conn);
            continue;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SS_SERVER_PORT);

        if (inet_pton(AF_INET, SS_SERVER_ADDRESS, &server_addr.sin_addr) <= 0) {
            perror("Invalid address/Address not supported");
            printf("address not valid");
            fflush(stdout);
            close(client_fd);
            close(conn->server_fd);
            free(conn);
            continue;
        }

        if (connect(conn->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Connection to server failed");
            printf("connection failed to server");
            fflush(stdout);
            close(client_fd);
            close(conn->server_fd);
            free(conn);
            continue;
        }

        printf("Connected to Shadowsocks server %s:%d\n", SS_SERVER_ADDRESS, SS_SERVER_PORT);
        fflush(stdout);

        if (fork() == 0) {
            close(listen_fd);
            handle_client(conn);
        } else {
            close(client_fd);
            handle_server(conn); // Handle server in the same process
        }
    }
    fflush(stdout);

    close(listen_fd);

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

//!vless connection 
/* 
#define VLESS_URI "vless://48bd4d22-e42f-4749-af6a-85ecf716c823@ubia2-join.outline-vpn.cloud:80?path=%2F%3Fed%3D2048&security=none&encryption=none&host=fkhamnei.kosnanatfilterchi.eu.org&type=ws#MTN+56+ID%28%40Outline_Vpn%29"
#define MAX_PASSWORD_LENGTH 64
#define MAX_SERVER_ADDRESS_LENGTH 256 
*/



int vless(const char* vless_uri,int maxpass_len,int max_server_len) {
    #define VLESS_URI vless_uri
    #define MAX_SERVER_ADDRESS_LENGTH max_server_len
    #define MAX_PASSWORD_LENGTH maxpass_len

    char server_address[MAX_SERVER_ADDRESS_LENGTH];
    int server_port;
    char password[MAX_PASSWORD_LENGTH];

    // Extract server address, port, and password from the URI
    if (sscanf(VLESS_URI, "vless://%*[^@]@%255[^:]:%d", server_address, &server_port) != 2) {
        fprintf(stderr, "Failed to parse the URI\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    int client_fd;
    struct sockaddr_in server_addr;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_address, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server address: %s\n", strerror(errno));
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Connection failed: %s\n", strerror(errno));
        fflush(stdout);

        exit(EXIT_FAILURE);
    }

    // Connection successful, you can now send and receive data

    return 0;
}
//!eof