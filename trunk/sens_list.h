/**
 * \file sens_list.h
 * Provides the functionality of a single linked list to organize temperature values
 */
#include <stdint.h>
#include <netinet/in.h>

#ifndef SENS_LIST_H
#define SENS_LIST_H


/**
 * \brief NODE_ID_LENGTH sets a fix length of a char string holding at most an IP addr.
 **/
#define PROTOCOL_ADDRESS 14

/**
 * \brief TEMP_EXPIRED_TIME limit for the time a temperature value is valid.
 *
 * Limit is 5 minutes in seconds: 5*60
 **/
#define TEMP_EXPIRED_TIME 300

/** \struct node_sense_val
 * \brief Short description of a sensor node
 **/
typedef struct node_sense_val{
    struct sockaddr_storage* node_addr; /*!< Unique ID of the node*/
    int32_t timestamp; /*!< Timestamp from when the sensor value was captured */
    int32_t temp_val;  /*!< Sensor value */

    struct node_sense_val *next; /*!< Pointer to the next element in the linked list of node descriptions*/
}node_sens;


/** 
 * \brief Adds a node to the linked list of node descriptions
 *
 * \param list A pointer to the start of the linked list where a new node should be added.
 * 
 * \param node_addr A sockaddr pointer holding the address info of the node.
 *
 * \param time Timestamp for the captured sensor value. Seconds since Unix epoch
 * 
 * \param sens_value An int holding the captured sensor value between -126 and 127.
 *
 * \return 0: upon success, 1: if failed
 **/
uint32_t add_node_sens(node_sens** list, int32_t sens_value, int32_t timestamp, struct sockaddr_storage* node_addr);


/**
 * \brief Removes a node from the linked list
 * \param list The list to remove a node from
 * \param lnode The node to remove from the list
 **/
uint32_t remove_node_sens(node_sens** list, node_sens* lnode);

/**
 * \brief Clears an entire list of sensor values
 * \param list The list to clear
 */
uint32_t clear_list_sens(node_sens** list);

/**
 * \brief Prints out the content of a list to stdout
 * \param list The list to print 
 **/
void print_node_list(node_sens* list);

#endif
