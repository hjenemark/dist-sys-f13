/**
 * \file sens_list.h
 */
#include <time.h>
#include <stdint.h>

#ifndef SENS_LIST_H
#define SENS_LIST_H


/**
 * \brief NODE_ID_LENGTH sets a fix length of a char string holding at most an IP addr.
 **/
#define NODE_ID_LENGTH 15

/**
 * \brief TEMP_EXPIRED_TIME limit for the time a temperature value is valid.
 *
 * Limit is 5 minutes in seconds: 5*60
 **/
#define TEMP_EXPIRED_TIME 300

/**
 * \brief Short description of a sensor node
 **/
typedef struct node_sense_val{
    char* node_id; /*!< Unique ID of the note. This could be the IP-address*/
    time_t timestamp; /*!< Timestamp from when the sensor value was captured */
    int32_t temp_val;  /*!< Sensor value */

    struct node_sense_val *next; /*!< Pointer to the next element in the linked list of node descriptions*/
}node_sens;


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
//int32_t add_node(void** list, int32_t (*populate)(void* node, ...), int32_t type, ...);
uint32_t add_node_sens(node_sens** list, char* id, time_t timestamp, int sens_value);


/**
 * \brief Removes a node from the linked list
 **/
uint32_t remove_node_sens(node_sens** list, node_sens* lnode);

/**
 * \brief Clears an entire list of sensor values
 */
uint32_t clear_list_sens(node_sens** list);

/**
 * \brief Prints out the content of a list
 **/
void print_node_list(node_sens* list);

#endif
