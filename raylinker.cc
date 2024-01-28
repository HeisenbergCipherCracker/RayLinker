#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "libz/connection.h"
#include "logos.h"

#define SS_SERVER_ADDRESS "series-a2-mec.samanehha.co"
#define SS_SERVER_PORT 443
#define SS_PASSWORD "u17T3BvpYaaiauW2c"
int main(){
    print_logo();
    shadowsocks_conn("series-a2-mec.samanehha.co",443,"u17T3BvpYaaiauW2c");
    return 0;
}
