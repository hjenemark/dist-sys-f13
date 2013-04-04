/**
 * \file sense_db.h
 */
#include "sens_list.h"

#ifndef SENSE_DB_H
#define SENSE_DB_H
int append_temp_db(node_sens** list, char* node_id, time_t timestamp, int sens_value);
bool value_expired(time_t timestamp);


#endif
