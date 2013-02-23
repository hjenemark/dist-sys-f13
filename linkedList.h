/**
 * \file linkedList.h
 **/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <time.h>

/**
 * \brief NODE_ID_LENGTH sets a fix length of a char string holding at most an IP addr.
 **/
#define NODE_ID_LENGTH 15

/**
 * \brief Short description of a sensor node
 **/
typedef struct node_description{
    char* node_id; /*!< Unique ID of the note. This could be the IP-address*/
    time_t timestamp; /*!< Timestamp from when the sensor value was captured */
    int temp_val;  /*!< Sensor value */
    struct node_description *next; /*!< Pointer to the next element in the linked list of node descriptions*/
} node;

/** 
 * \brief Adds a node to the linked list of node descriptions
 *
 * \param list A pointer to the start of the linked list where a new node should be added.
 * 
 * \param id A char pointer the a null-terminated string holding the node id
 *
 * \param time Timestamp for the captured sensor value. Seconds since Unix epoch
 * 
 * \param sens_value An int holding the captured sensor value between -126 and 127.
 *
 * \return 0: upon success, 1: if failed
 **/
int add_node(node** list, char* id, time_t time, int sens_value);

/**
 * \brief Prints out the content of a list
 **/
void print_node_list(node* list);

#endif
