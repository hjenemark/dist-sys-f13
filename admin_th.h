/**
 * \file admin_th.h
 *
 * Provides an entry to the admin_th.c functions
 **/

#include "common.h"

#ifndef __ADMIN_TH_HEADER
#define __ADMIN_TH_HEADER

#define ADMIN_SLEEP_INTERVAL 10 /*!< Sleep interval of admin node */
#define BACKLOG 100 /*!< Max number of connections to queue */

/** 
 * \brief Entry function of admin thread.
 *
 * \return void
 **/
void *admin_network_thread_entry();

/** 
 * \brief Worker function of admin thread.
 * 
 * If the node is the admin a message is broadcast on the network.
 * Otherwise it waits for the promote message.
 * 
 * \param socket Broadcast socket to use when communicating.
 *
 * \return void
 **/
void recurse_worker(int32_t socket);

/** 
 * \brief Calculate time offset of local clock to master node clock and store in global.
 *
 * \return void
 **/
void calculate_time_offset();
#endif
