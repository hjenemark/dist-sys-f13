/**
 * \file sensor.c
 **/
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sensor.h"
#include "globals.h"


int32_t current_master_id = 0; /*!< Current master ID global */
int32_t node_is_master = 0; /*!< Is current node a master */
pthread_mutex_t mutex_master_params; /*!< Mutex to protect above parameters */

struct peer_net_params admin_net_params; /*!< Admin node IP parameters */
pthread_mutex_t mutex_adminp; /*!< Mutex to protect admin node IP parameters */

int32_t node_admin_offset = 0; /*!< Local to node time offset */
pthread_mutex_t mutex_timeoffset; /*!< Mutex to protect time offset */

struct network_params np; /*!< Struct holding local IP parameters */

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void check_startup_params(int argc, char *argv[])
{
	/* No parameters provided. Default OS parameters will be used.*/
	if (argc == 1) {
		np.net_mode = OS_PROVIDED_IP;
		printf("OS Provided IP will be used.\r\n");
		return;
	}

	/* Update network parameters struct with IP version. */
	int s;
	np.net_mode = USER_PROVIDED_IP;
	printf("User provided IP will be used.\r\n");	
	
	if (strcmp(argv[1], "v4") == 0) np.family = AF_INET;
	else if (strcmp(argv[1], "v6") == 0) np.family = AF_INET6;
	else {
		printf("Wrong version parameter!\n");
		exit(EXIT_FAILURE);
	}

	/* Check that provided IP is consistent with IP version. */
	strcpy(np.ipstr, argv[2]);
	char buff[INET6_ADDRSTRLEN];
	s = inet_pton(np.family, argv[2], buff);
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
	np.ipstr[0] = '\0';
	
	printf("Sensor process implementation\r\n");
	printf("HJE & JP. DTU 2013\r\n");

	/* Check for start-up paramas */	
	if(!(argc == 1 || argc == 3)) {
		printf("Usage: %s [{v4|v6} IP-Address]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	check_startup_params(argc, argv);

	/* Initialize mutexes */
	pthread_mutex_init(&mutex_adminp, NULL);
	pthread_mutex_init(&mutex_timeoffset, NULL);
	pthread_mutex_init(&mutex_master_params, NULL);
	

	/* Remove orphaned and dead processes */
	struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

	/* Start working threads */
	pthread_t threads[3];
	if(pthread_create(
			&threads[0], NULL, 
			temperature_thread_entry, NULL)) {
		printf("Sensor thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_create(
			&threads[1], NULL, 
			admin_network_thread_entry, NULL)) {
		printf("Admin network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
		
	if(pthread_create(
			&threads[2], NULL, 
			data_network_thread_entry, NULL)) {
		printf("Data network thread creation failed! Programm will abort!\r\n");
		exit(EXIT_FAILURE);
	}
	
	/* Loop while sensor is alive. Sleep to conserve CPU */
	while (1) {
		sleep(1);
	}

	printf("Bye!\r\n");
	exit(EXIT_SUCCESS);
}

