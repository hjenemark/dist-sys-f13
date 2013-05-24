/**
 * \file msg_list.h
 * Header file for msg_list.c
 */
#include <stdint.h>

#ifndef MSG_LIST_H
#define MSG_LIST_H

/** \struct node_message
 * Struct for holding data in the linked list. Designed for TLV data
 **/
typedef struct node_message{
    uint32_t operation; /*!< Type of data in the linked list node */
    uint32_t op_size;   /*!< Size of data in the linked list node */
    char* operand;      /*!< Data in the linked list node */
    struct node_message *next; /*!< Points to next node in the linked list */
}node_msg;
/**
 * \brief serializes a linked list before sending it over the network
 * @param list The list to be serialized
 * @return A pointer to a string ready for network transfer
 **/
char* serilization(node_msg* list);

/**
 * \brief Makes a linked list from a string of data in TLV format
 * @param data The char string to be processed
 * @return List A pointer to a linked form from the provided data.
 **/
node_msg* deserialize(char* data);

/**
 * \brief Adds a node of type node_msg to the front of a linked list
 * @param list The list to append the node to
 * @param operation The operation type for the given node
 * @param op_size The size of the operand in this node
 * @param operand The data itself for this node
 * @return Returns 0 upon success
 **/
uint32_t add_node_msg(node_msg** list, uint32_t operation, uint32_t op_size, char* operand);

/**
 * \brief Removes all nodes from a node_msg linked list
 * @param list The list to be cleared
 * @return Returns 0 upon success
 **/
uint32_t clear_list_msg(node_msg** list);

/**
 * \brief Removes a node from a node_msg linked list and frees the memory
 * @param list The list to remove a node from
 * @param lnode The node to remove from the list
 * @return Returns 0 upon success
 **/
uint32_t remove_node_msg(node_msg** list, node_msg* lnode);

/**
 * \brief Prints a list of type node_msg to stdout
 * @param list The list to print
 **/
void print_node_list_msg(node_msg* list);


#endif
