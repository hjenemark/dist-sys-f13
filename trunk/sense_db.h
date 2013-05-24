/**
 * \file sense_db.h
 * Provides the functionality of the temperature database
 */
#include "sens_list.h"

#include <netinet/in.h>
#include <stdbool.h>

#ifndef __SENSE_DB_H
#define __SENSE_DB_H
/**
 * \brief Appends a temperature value to the database
 * If the value is from a known source node the old on is overridden.
 * Removes any values that are too old
 * @param list The list that holds the temperature values
 * @param temperature The new temperature value to store
 * @param timestamp The unix timestamp of the temperature value
 * @param node_addr Information about the source node of the temperature value
 * @return 0: success, 1: failed
 **/
uint32_t append_temp_db(node_sens** list, int32_t temperature, int32_t timestamp, struct sockaddr_storage *node_addr);

/**
 * \brief Tests if the given timestamp has expired
 * @param timestamp Unix timestamp
 * @return true: if expired, false: if not expired
 **/
bool value_expired(int32_t timestamp);

/**
 * \brief Returns the average temperature from the given list
 * @param list The list with data to process
 * @return The average number from the temperature values in the provided list.
 **/
int32_t average_temp(node_sens* list);

/**
 * \brief Compares 2 node adresses to see if they are the same based on IP adresses
 * @param current The address already in store
 * @param new The new address to be evaluated
 * @return True: if the adresses are the same. False: if the adresses are not the same.
 **/
bool compare_addr(struct sockaddr_storage* current, struct sockaddr_storage* new);

#endif
