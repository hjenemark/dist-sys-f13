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

struct network_params {
	uint8_t net_mode;	
	int domain;
	unsigned char buf[sizeof(struct in6_addr)];	
};

#endif
