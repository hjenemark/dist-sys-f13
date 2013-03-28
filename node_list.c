/**
 * \file linkedList.c
 **/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "node_list.h"

int append_temp_db(node** list, char* node_id, time_t timestamp, int sens_value){
    node* current = NULL;

    
    while(current != NULL){
        if((time(NULL) - current->timestamp) > TEMP_EXPIRED_TIME){
            remove_node(list, current);
        }
    }

    return 0;
}
int remove_node(node** list, node* lnode){
    
    printf("[remove] %s\n", lnode->node_id);

    node* current = NULL;

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
int add_node(node** list, char* id, time_t timestamp, int sens_value){
    node* newnode;
    newnode = (node*)malloc(sizeof(node));

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


void print_node_list(node* list){
    node *current = list;
    printf("[Printing list]\n");

    while(current != NULL){
        printf("Node: %s\n", current->node_id);
        printf("- Time: %lld\n", (long long)current->timestamp);
        printf("- Temp: %i\n", current->temp_val);

        current = current->next;
    }
}
