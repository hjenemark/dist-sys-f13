/**
 * \file common.h
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h> /*! ATTN: This works on *nix. Win needs time.h */

#include <pthread.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "msg_list.h"
#include "sens_list.h"

#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#define USER_PROVIDED_IP 1
#define OS_PROVIDED_IP 0

#define DATA_PORT 51001
#define ADMIN_PORT 51002

#define CONTROL_LISTEN 1
#define DATA_LISTEN 2
#define DATA_SUBMIT 3

#define GET_TEMPERATURE 1
#define REPORT_TEMPERATURE 2
#define ANNOUNCE_MASTER 3
#define TIME_REQUEST 4
#define TIME_REPLY 5
#define PROMOTE_TO_MASTER 6
#define PROMO_KEY 7

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
 * \brief Encode message for sending over network.
 * 
 * For operation REPORT_TEMPERATUR this function uses the prototype:
 * char* encode_message( uint32_t operation, int8_t temperature, char* ipstr
 **/
char* encode_message( uint32_t operation, ...);
void* decode_message( char* message);

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
#endif
