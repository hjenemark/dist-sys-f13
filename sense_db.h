/**
 * \file sense_db.h
 */
#include "sens_list.h"

#include <netinet/in.h>
#include <stdbool.h>

#ifndef __SENSE_DB_H
#define __SENSE_DB_H
uint32_t append_temp_db(node_sens** list, int32_t temperature, int32_t timestamp, struct sockaddr_storage *node_addr);
bool value_expired(int32_t timestamp);

int32_t average_temp(node_sens* list);
bool compare_addr(struct sockaddr_storage* current, struct sockaddr_storage* new);

#endif
