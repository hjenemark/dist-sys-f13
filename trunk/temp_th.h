#include "common.h"

#ifndef _TEMP_TH_HEADER
#define _TEMP_TH_HEADER

#define SENSOR_SLEEP_INTERVAL 15

void *temperature_thread_entry(void *np);
int8_t get_temperature();
void send_temperature(struct network_params *np, struct peer_net_params *pnp,
		      int8_t temperature);
int8_t get_admin_params(struct peer_net_params *pnp);

#endif
