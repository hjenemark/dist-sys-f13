/**
 * \file admin_th.h
 *
 * Provides an entry to the admin_th.c functions
 **/

#include "common.h"

#ifndef __ADMIN_TH_HEADER
#define __ADMIN_TH_HEADER

#define ADMIN_SLEEP_INTERVAL 10
#define BACKLOG 100

void *admin_network_thread_entry();
void recurse_worker(int32_t socket);

#endif
