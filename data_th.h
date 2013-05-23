#include "common.h"

#ifndef __DATA_TH_HEADER
#define __DATA_TH_HEADER

struct temp_storage {
	int32_t temperature;
	int32_t timestamp;
	struct sockaddr *node_addr;
};

void *data_network_worker(void *con);
void *data_network_thread_entry();
void report_average_temperature(int32_t socket);
void update_promo_key(char *keydata);
void become_master();
void append_db_timestamp(
	node_sens* temp_database, struct temp_storage* temp_storage, char *operand);
void append_db_data(
	node_sens* temp_database, struct temp_storage* temp_data, char *operand, 
    struct sockaddr *node_addr);

#endif
