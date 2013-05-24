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

#define USER_PROVIDED_IP 1 /* User user provided IP */
#define OS_PROVIDED_IP 0 /* User OS provided IP */

#define DATA_PORT 51001 /* Port for unicast communication */
#define ADMIN_PORT 51002 /* Port for multicast communication */

#define CONN_BACKLOG 100 /* Number of connections held in backlog */

#define CONTROL_LISTEN 1 /* Requested socket type */
#define DATA_LISTEN 2 /* Requested socket type */
#define DATA_SUBMIT 3 /* Requested socket type */

#define REPORT_TEMPERATURE 1 /* Temperature value of sensor */
#define ANNOUNCE_MASTER 2 /* Broadcast by master node indicating master */
#define PROMOTE_TO_MASTER 3 /* Message by user to become a master */
#define PROMO_KEY 4 /* Promotion key of current message */
#define TEMP_TIMESTAMP 5 /* Message containing timestamp */
#define GET_AVG_TEMP 6 /* Request by user to get avg temperature */
#define REPORT_AVG_TEMP 7 /* Reply of admin node with average temperature */
#define REQ_TIME 8 /* Request to provide time values */
#define TIME_REQ_RX 9 /* Time request received */
#define TIME_REP_TX 10 /* Time reply sent */

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

/** 
 * \brief Returns pointer to data skipping padding.
 *
 * \param sockaddr *sa A stuct hosling socket information
 * 
 * \return void* pointer to data in sockaddr structure after padding
 **/
void *get_in_addr(struct sockaddr *sa);
#endif
