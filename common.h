/**
 * \file common.h
 * Provides constants and network parameters to be used in all entities of the project
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h> /*! ATTN: This works on *nix. Win needs time.h */
#include <time.h>

#include <pthread.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "msg_list.h"
#include "sens_list.h"
#include "sense_db.h"

#ifndef __COMMON_HEADER
#define __COMMON_HEADER

#define USER_PROVIDED_IP 1
#define OS_PROVIDED_IP 0

#define DATA_PORT 51001
#define ADMIN_PORT 51002

#define CONN_BACKLOG 100

#define CONTROL_LISTEN 1
#define DATA_LISTEN 2
#define DATA_SUBMIT 3

#define GET_TEMPERATURE 1
#define REPORT_TEMPERATURE 2 /* Temperature value of sensor */
#define ANNOUNCE_MASTER 3 /* Broadcast by master node indicating master */
#define TIME_REQUEST 4
#define TIME_REPLY 5
#define PROMOTE_TO_MASTER 6 /* Message by user to become a master */
#define PROMO_KEY 7 /* Promotion key of current message */
#define TEMP_TIMESTAMP 8 /* Message containing timestamp */
#define GET_AVG_TEMP 9 /* Request by user to get avg temperature */
#define REPORT_AVG_TEMP 10 /* Reply of admin node with average temperature */


/** \struct network_params
 *
 *	Structure to keep information about IP address that client will bind to.
 */ 
struct network_params {
	uint8_t net_mode;				/*!< User or OS provided IP address */
	int family;						/*!< IP version */
	char ipstr[INET6_ADDRSTRLEN];   /*!< Textual representation of IP address */
};

/** \struct peer_net_params
 * 
 *	Structure to keep network parameters of a peer we will connect to.
 */
struct peer_net_params {
	int family;						/*!< IP version */
	char ipstr[INET6_ADDRSTRLEN];	/*!< Textual representation of IP address */
};

/** 
 * \brief Returns socket based on requested parameters.
 *
 * \param np A stuct indicating local network parameters. Used when binding on user provided IP.
 * 
 * \param pnp A struct indicating peer network parameters.
 *
 * \param socket_type Enumerator to indicate requred socket type.
 *
 * \return socketnumber: upon success, -1: if failed
 **/
int get_socket(struct network_params *np, struct peer_net_params *pnp, 
               int socket_type);

void *get_in_addr(struct sockaddr *sa);
#endif
