/**
 * \file msg_list.h
 */
#include <time.h>
#include <stdint.h>

#ifndef MSG_LIST_H
#define MSG_LIST_H

typedef struct node_message{
    uint32_t operation;
    char* operand;
    uint32_t op_size;
    struct node_message *next;
}node_msg;


uint32_t add_node_msg(node_msg** list, uint32_t operation, char* operand, uint32_t op_size);


uint32_t remove_node_msg(node_msg** list, node_msg* lnode);


void print_node_list_msg(node_msg* list);


#endif
