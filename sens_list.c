/**
 * \file sens_list.c
 */

//#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sens_list.h"

uint32_t clear_list_sens(node_sens** list){
    while(*list != NULL){
        remove_node_sens(list, *list);
    }
    return 0;
}

uint32_t remove_node_sens(node_sens** list, node_sens* lnode){
    node_sens* current = NULL;

    if(*list == lnode){
        *list = lnode->next;
    } else {
        current = *list;
        while(current->next != lnode){
            current = current->next;
        }
        if(lnode->next == NULL){
            current->next = NULL;
        } else {
            current->next = current->next->next;
        }
    }
    free(lnode->node_addr);
    free(lnode);
    return 0;
}

uint32_t add_node_sens(node_sens** list, 
        int32_t sens_value, int32_t timestamp, struct sockaddr* node_addr){
    node_sens* newnode;

    newnode = (node_sens*)malloc(sizeof(node_sens));

    if(newnode == NULL){
        return -1;
    }

    /**
     * Adds a node in the beginning of the list.
     */
    newnode->next = *list;
    *list = newnode;

    
    /**
     * Populates the new node.
    */
    newnode->node_addr = (struct sockaddr*)malloc(sizeof(struct sockaddr));
    memcpy(newnode->node_addr, node_addr, sizeof(struct sockaddr));
    newnode->timestamp = timestamp;
    newnode->temp_val = sens_value;
    //printf("[adding] Node added with id: %s\n",(*list)->node_id);
    
    return 0;
}

void print_node_list(node_sens* list){
    node_sens *current = list;
    char* char_buff = (char*)malloc(sizeof(char)*(INET_ADDRSTRLEN + 1));
    memset(char_buff, '0', INET_ADDRSTRLEN + 1);
    struct sockaddr_in* ip4addr;
    //char* char_buff = (char*)malloc(sizeof(char)*INET_ADDRSTRLEN + 1);
    printf("[Printing list]\n");
    

    while(current != NULL){
        printf("- - -\n");
        ip4addr = (struct sockaddr_in*) current->node_addr;
        printf("- Node: %s\n", inet_ntop(AF_INET, &(ip4addr->sin_addr), char_buff, INET_ADDRSTRLEN));
        printf("- - Time: %d\n", current->timestamp);
        printf("- - Temp: %i\n", current->temp_val);
        printf("- - -\n");

        current = current->next;
    }
    
    printf("[End of list]\n");
    free(char_buff);
}
