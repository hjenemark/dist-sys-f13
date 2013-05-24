/**
 * \file temp_th.c
 **/
#include "temp_th.h"
#include "globals.h"

void *temperature_thread_entry()
{
	/* Setup variables */
	int8_t temperature;
	struct peer_net_params admin_addr;
	admin_addr.ipstr[0] = '\0';

	/* Main working loop of temperature thread */
	while (1) {
		temperature = get_temperature();
		get_admin_params(&admin_addr);
		if (admin_addr.ipstr[0] != '\0') {
			send_temperature(&admin_addr, temperature);
		} else {
			printf("[Temp] Admin address not know yet!\r\n");
		}
		sleep(SENSOR_SLEEP_INTERVAL);
	}
	pthread_exit(NULL);
}

int8_t get_temperature()
{
	/* Return random number in a range 0 ~ 49 */
	srand(time(NULL));
    int32_t random_temp = rand() % 50;
    return random_temp;
}

int8_t get_admin_params(struct peer_net_params *pnp)
{
	/* Read global admin parameters to local data structure */
	pthread_mutex_lock (&mutex_adminp);
	pnp->family = admin_net_params.family;
	strcpy(pnp->ipstr, admin_net_params.ipstr);
	printf("[Temp]DEBUG: Admin IP:%s\r\n", pnp->ipstr);
	pthread_mutex_unlock (&mutex_adminp);
	return 0;
}

int32_t get_time_offset()
{
	/* Get time offset from global variable */
	int32_t timeoffset;
	pthread_mutex_lock (&mutex_timeoffset);
	timeoffset = node_admin_offset;
	pthread_mutex_unlock (&mutex_timeoffset);
	return timeoffset;
}

void send_temperature(struct peer_net_params *pnp,int8_t temperature)
{
	/* Status output for loging and debug */
	printf("[Temp] Sending temperature \"%d\" to server \"%s\".\r\n", 
	   	 temperature, pnp->ipstr);	

	/* Setup required variables */
	int32_t sockfd, buflen, timenow, timeoffset, realtime;
	char buffer[15];
	struct node_message *node_msg = NULL;

	/* Create message containing temperature */
	buflen = snprintf(buffer, 15, "%d", temperature);
	add_node_msg (&node_msg, REPORT_TEMPERATURE, buflen, buffer);

	/* Create timestamp of admin local time */ 
	timenow = time(0);
	timeoffset = get_time_offset();
	realtime = timenow + timeoffset;
	printf("[Temp] Real time at admin node: %d\r\n", realtime);

	/* Add admin local timestamp to message */
	buflen = snprintf(buffer, 15, "%d", realtime);
	add_node_msg (&node_msg, TEMP_TIMESTAMP, buflen, buffer);

	/* Serialize all messages */
	char *msg=NULL;
	msg = serilization(node_msg);
	int32_t len = strlen(msg);

	/* Request socket and send out data */
	sockfd = get_socket(&np, pnp, DATA_SUBMIT);
	send(sockfd, msg, len, 0);
	close(sockfd);
}
