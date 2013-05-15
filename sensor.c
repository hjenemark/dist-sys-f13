/**
 * \file sensor.c
 **/
#include "sensor.h"

/** 
 * \brief Checks startup parameters
 *
 * \param argc Integer indicating number of parameters.
 * 
 * \param argv[] Pointer to a lits of passed parameters.
 *
 * \param np A struct for storing parsed parameters.
 **/
void check_startup_params(int argc, char *argv[], struct network_params *np)
{
	/**
     * No parameters provided. Default OS parameters will be used.
     */
	if (argc == 1) {
		np->net_mode = OS_PROVIDED_IP;
		printf("OS Provided IP will be used.\r\n");
		return;
	}

	/**
     * Update network parameters struct with IP version.
     */
	int s;
	np->net_mode = USER_PROVIDED_IP;
	printf("User provided IP will be used.\r\n");	
	
	if (strcmp(argv[1], "v4") == 0) np->family = AF_INET;
	else if (strcmp(argv[1], "v6") == 0) np->family = AF_INET6;
	else {
		printf("Wrong version parameter!\n");
		exit(EXIT_FAILURE);
	}

	/**
     * Check that provided IP is consistent with IP version.
     */
	strcpy(np->ipstr, argv[2]);
	char buff[INET6_ADDRSTRLEN];
	s = inet_pton(np->family, argv[2], buff);
	if (s <= 0) {
		if (s == 0)
	    		printf("Wrong address format!\n");
		else
	    		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
}

int main (int argc, char *argv[])
{
	struct network_params np;
	np.ipstr[0] = NULL;
	
	printf("Sensor process implementation\r\n");
	printf("HJE & JP. DTU 2013\r\n");

	/* Check for start-up paramas */	
	
	if(!(argc == 1 || argc == 3)) {
		printf("Usage: %s [{v4|v6} IP-Address]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	check_startup_params(argc, argv, &np);

	/* Start worker threads */

	/* TODO: This is temporary until admin parameter detection is implemented */
	admin_net_params.family = AF_INET;
	strcpy(admin_net_params.ipstr, "127.0.0.2");
	/* End of temporary code */

	/* Init time offset */
	node_admin_offset = 0;

	/* Init master status */
	node_is_master = 0;
	current_master_id = 0;

	/* Init mutexes */
	pthread_mutex_init(&mutex_adminp, NULL);
	pthread_mutex_init(&mutex_timeoffset, NULL);
	pthread_mutex_init(&mutex_master_params, NULL);
	
	pthread_t threads[3];

	struct network_params np_array[3];
	int i;	
	for (i=0; i<3; i++) {
		np_array[i].net_mode = np.net_mode;
		np_array[i].family = np.family;
		strcpy(np_array[i].ipstr, np.ipstr);
	}
	/*
	if(pthread_create(
			&threads[0], NULL, 
			temperature_thread_entry, (void *) &np_array[0])) {
		printf("Sensor thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}*/
	/*
	if(pthread_create(
			&threads[2], NULL, 
			admin_network_thread_entry, (void *) &np_array[2])) {
		printf("Admin network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	*/

	/* Uncomment once ready */
	/**/
	
	if(pthread_create(
			&threads[1], NULL, 
			data_network_thread_entry, (void *) &np_array[1])) {
		printf("Data network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	
	/* Loop while sensor is alive */
	while (1) {
	}

	printf("Bye!\r\n");
	exit(EXIT_SUCCESS);
}
