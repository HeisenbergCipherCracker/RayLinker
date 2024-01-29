#include <stdio.h>
#ifndef COMMON_H
#define COMMON_H
#endif
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif
#include <string.h>
#include "typesdef.h"

void handle_client(struct ss_conn *conn);
void handle_server(struct ss_conn *conn);

void create_table(const char* data[], const char* column_headers[], size_t num_columns, size_t num_rows) {
    const char plus_sign = '+';
    const char minus_sign = '-';
    const char pipe_sign = '|';

    size_t max_column_width = 0;

    // Check if the number of columns is valid
    if (num_columns > 0) {
        // Calculate the maximum length of data in each column
        for (size_t i = 0; i < num_columns; i++) {
            size_t column_width = strlen(column_headers[i]);
            if (column_width > max_column_width) {
                max_column_width = column_width;
            }
        }

        // Print the top line with plus signs
        printf("%c", plus_sign);
        for (size_t i = 0; i < num_columns * (max_column_width + 4) - 1; i++) {
            printf("%c", minus_sign);
        }
        printf("%c\n", plus_sign);

        // Print the column headers
        for (size_t i = 0; i < num_columns; i++) {
            printf("| %-*s ", (int)max_column_width, column_headers[i]);
        }
        printf("|\n");

        // Print the middle line with plus signs
        printf("%c", plus_sign);
        for (size_t i = 0; i < num_columns * (max_column_width + 4) - 1; i++) {
            printf("%c", minus_sign);
        }
        printf("%c\n", plus_sign);
    }

    // Print the data in each column
    for (size_t row = 0; row < num_rows; row++) {
        for (size_t col = 0; col < num_columns; col++) {
            printf("| %-*s ", (int)max_column_width, data[col + row * num_columns]);
        }
        printf("|\n");

        // Print the line with minuses after each row
        printf("%c", plus_sign);
        for (size_t i = 0; i < num_columns * (max_column_width + 4) - 1; i++) {
            printf("%c", minus_sign);
        }
        printf("%c\n", plus_sign);
    }
}



List get_server_address_info(){
    List list;
    const char* serveraddress;
    const char* serverport;
    const char* servertype;
    printf("Enter the server address:");
    fflush(stdout);
    scanf("%s",serveraddress);
    printf("Enter the server port:");
    fflush(stdout);
    scanf("%s",serverport);
    printf("Enter the server type:");
    fflush(stdout);
    scanf("%s",servertype);
    list.string = serveraddress;
    list.numeric = serverport;
    list.string = servertype;
    return list;
}