/**
 * \file sens_list.c
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sens_list.h"

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
    
    free(lnode->node_id);
    free(lnode);
    return 0;
}

uint32_t add_node_sens(node_sens** list, char* id, time_t timestamp, int sens_value){
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
    newnode->node_id = (char*)malloc(sizeof(char)*NODE_ID_LENGTH);
    memcpy(newnode->node_id, id, NODE_ID_LENGTH);
    newnode->timestamp = timestamp;
    newnode->temp_val = sens_value;
    printf("[adding] Node added with id: %s\n",(*list)->node_id);
    
    return 0;
}

void print_node_list(node_sens* list){
    node_sens *current = list;
    printf("[Printing list]\n");

    while(current != NULL){
        printf("Node: %s\n", current->node_id);
        printf("- Time: %lld\n", (long long)current->timestamp);
        printf("- Temp: %i\n", current->temp_val);

        current = current->next;
    }
}
