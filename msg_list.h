/**
 * \file msg_list.h
 */
#include <time.h>
#include <stdint.h>

#ifndef MSG_LIST_H
#define MSG_LIST_H

typedef struct node_message{
    uint32_t operation;
    uint32_t op_size;
    char* operand;
    struct node_message *next;
}node_msg;

char* serilization(node_msg* list);
node_msg* deserialize(char* data);

uint32_t add_node_msg(node_msg** list, uint32_t operation, uint32_t op_size, char* operand);
uint32_t clear_list_msg(node_msg** list);
uint32_t remove_node_msg(node_msg** list, node_msg* lnode);

void print_node_list_msg(node_msg* list);


#endif
