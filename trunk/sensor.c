#include "sensor.h"

void check_startup_params(int argc, char *argv[], struct network_params *np)
{
	if (argc == 1) {
		np->net_mode = OS_PROVIDED_IP;
		printf("OS Provided IP will be used.\r\n");
		return;
	}

	int s;
	np->net_mode = USER_PROVIDED_IP;
	printf("User provided IP will be used.\r\n");	
	
	if (strcmp(argv[1], "v4") == 0) np->domain = AF_INET;
	else if (strcmp(argv[1], "v6") == 0) np->domain = AF_INET6;
	else {
		fprintf(stderr, "Wrong version parameter!\n");
		exit(EXIT_FAILURE);
	}

	s = inet_pton(np->domain, argv[2], np->buf);
	if (s <= 0) {
		if (s == 0)
	    		fprintf(stderr, "Wrong address format!\n");
		else
	    		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
}

int main (int argc, char *argv[])
{
	struct network_params np;
	
	printf("Sensor process implementation\r\n");
	printf("HJR & JP. DTU 2013\r\n");

	/* Check for start-up paramas */	
	
	if(!(argc == 1 || argc == 3)) {
		printf("Usage: %s [{v4|v6} IP-Address]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	check_startup_params(argc, argv, &np);

	/* Start worker threads */
	/* TODO: MutEx for Admin IP sharing */
	pthread_t threads[3];

	struct network_params np_array[3];
	int i;	
	for (i=0; i<3; i++) {
		np_array[i].net_mode = np.net_mode;
		np_array[i].domain = np.domain;
		strcpy(np_array[i].buf, np.buf);
	} 
	
	if(pthread_create(
			&threads[0], NULL, 
			temperature_thread_entry, (void *) &np_array[0])) {
		printf("Sensor thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	
	/* Uncomment once ready */
	/*
	if(pthread_create(
			&threads[1], NULL, 
			data_network_thread_entry, (void *) &np_array[1])) {
		printf("Data network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}

	
	if(pthread_create(
			&threads[2], NULL, 
			admin_network_thread_entry, (void *) &np_array[2])) {
		printf("Admin network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}*/
	
	/* Loop while sensor is alive */
	while (1) {
	}

	printf("Bye!\r\n");
	exit(EXIT_SUCCESS);
}
