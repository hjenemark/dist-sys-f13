#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

#include "temp_th.h"
#include "admin_th.h"
#include "data_th.h"

#define SENSOR_SLEEP_INTERVAL 15

#define USER_PROVIDED_IP 1
#define OS_PROVIDED_IP 0

struct network_params {
	uint8_t net_mode;	
	int domain;
	unsigned char buf[sizeof(struct in6_addr)];
	
};
