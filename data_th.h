#include "common.h"

#ifndef _DATA_TH_HEADER
#define _DATA_TH_HEADER

void *data_network_thread_entry(void *np);
void report_average_temperature(int32_t socket);
void update_promo_key(char *keydata);
void become_master(struct network_params *np);

#endif
