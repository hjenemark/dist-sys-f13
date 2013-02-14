#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "sensor.h"

int main (int argc, char *argv[])
{
	uint8_t net_mode;
	
	printf("Sensor process implementation\r\n");
	printf("HJR & JP. DTU 2013\r\n");

	/* Check for start-up paramas */	
	if(argc == 1) {
		net_mode = 0;
		printf("Sensor will get IP address from OS\r\n");
	}
	else if (argc == 2) {
		net_mode = 1;
		printf("Sensor will use IP provided\r\n");
	}	
	else {
		printf("Unsupported number of parameters. Program will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	
	if(!check_startup_params(argv[1])) {
		printf("Incompatible adress provided. Program will abort!\r\n");
		exit(EXIT_FAILURE);
	}

	/* Start worker threads */
	pthread_t threads[3];
	uint8_t thread_args[3];
	
	/* Loop while sensor is alive */	
	while (1) {
	}
	exit(EXIT_SUCCESS);
}
