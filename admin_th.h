#include "common.h"

#ifndef __ADMIN_TH_HEADER
#define __ADMIN_TH_HEADER

#define ADMIN_SLEEP_INTERVAL 10
#define BACKLOG 100

void *admin_network_thread_entry(void *np);
void recurse_worker(int32_t socket);


#endif