/**
 * \file main.c
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>

#include "test.h"
#include "../sens_list.h"
#include "../common.h"

int main(int argc, char *argv[]){
    printf("Starting node...\n");
    printf("Adding nodes to list...\n");

    node_sens* list = NULL;
    struct sockaddr_in ipv4;
    ipv4.sin_family = AF_INET;
    ipv4.sin_port = htons(3490);
    char* char_buff = (char*)malloc(sizeof(char)*(INET_ADDRSTRLEN + 1));

    //inet_pton(AF_INET, "123.23.123.01", &ipv4.sin_addr);
    
    printf("[testing] node address: %s\n", inet_ntop(AF_INET, &ipv4.sin_addr, char_buff, INET_ADDRSTRLEN));

    add_node_sens(&list, 8, 1,  (struct sockaddr*)&ipv4);

    print_node_list(list);
    
    inet_pton(AF_INET, "123.123.123.02", &ipv4.sin_addr);

    add_node_sens(&list, 9, 2,  (struct sockaddr*)&ipv4);

    print_node_list(list);
    
    remove_node_sens(&list, list);

    print_node_list(list);
    
    clear_list_sens(&list);

    print_node_list(list);
    return 0;
}
