#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#define USER_PROVIDED_IP 1
#define OS_PROVIDED_IP 0

#define DATA_PORT 51001
#define ADMIN_PORT 51002

/**
 *	Structure to keep information about IP address that client will bind to.
 */ 
struct network_params {
	uint8_t net_mode;				/*!< User or OS provided IP address */
	int domain;					/*!< IP version */
	char buf[sizeof(struct in6_addr)];	/*!< Binary representation of IP address */
};

/**
 *	Structure to keep peer network parameters.
 */
struct peer_net_params {
	int domain;		/*!< IP version */
	char *text_ip_addr;	/*!< Textual address representation */
};

void encode_message();
#endif
