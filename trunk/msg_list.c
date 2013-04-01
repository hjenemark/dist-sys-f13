/**
 * \file msg_list.c
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "msg_list.h"


uint32_t remove_node_msg(node_msg** list, node_msg* lnode){
    node_msg* current = NULL;

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
    
    free(lnode->operand);
    free(lnode);
    return 0;
}


uint32_t add_node_msg(node_msg** list, uint32_t operation, char* operand, uint32_t op_size){
    node_msg* newnode;
    newnode = (node_msg*)malloc(sizeof(node_msg));

    if(newnode == NULL){
        return -1;
    }

    newnode->operation = operation;
    newnode->operand = (char*)malloc(op_size * sizeof(char));
    memcpy(newnode->operand, operand, op_size);
    newnode->op_size = op_size;

    
    newnode->next = *list;
    *list = newnode;
    printf("[adding] Node added with operand: %s\n", newnode->operand);

    return 0;
}

void print_node_list(node_msg* list){
    node_msg *current = list;
    printf("[Printing list]\n");

    while(current != NULL){
        printf("Node: %i\n", current->operation);
        printf("- operand: %s\n", current->operand);
        printf("- op_size: %i\n", current->op_size);

        current = current->next;
    }
}
