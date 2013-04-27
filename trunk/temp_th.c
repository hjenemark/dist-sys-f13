/**
 * \file temp_th.c
 **/
#include "temp_th.h"

void *temperature_thread_entry(void *np)
{
	int8_t temperature;
	struct peer_net_params admin_addr;

	while (1) {
		temperature = get_temperature();
		get_admin_params(&admin_addr);
		send_temperature((struct network_params *)np, &admin_addr, temperature);		
		sleep(SENSOR_SLEEP_INTERVAL);
	}
}

/* TODO: Implement temperature reading */
int8_t get_temperature()
{
	return 8;
}

int8_t get_admin_params(struct peer_net_params *pnp)
{
	pthread_mutex_lock (&mutex_adminp);
	pnp->family = admin_net_params.family;
	strcpy(pnp->ipstr, admin_net_params.ipstr);
	pthread_mutex_unlock (&mutex_adminp);
	return 0;
}

int32_t get_time_offset()
{
	int32_t timeoffset;
	pthread_mutex_lock (&mutex_timeoffset);
	timeoffset = node_admin_offset;
	pthread_mutex_unlock (&mutex_timeoffset);
	return timeoffset;
}

void send_temperature(struct network_params *np, struct peer_net_params *pnp,
		      int8_t temperature)
{
	/* Status output for loging and debug */
	printf("[Temp] Sending temperature \"%d\" to server \"%s\".\r\n", 
	   	 temperature, pnp->ipstr);	

	int32_t sockfd, buflen, timenow, timeoffset, realtime;
	char buffer[15];
	struct node_message *node_msg = NULL;
	
	buflen = snprintf(buffer, 15, "%d", temperature);
	add_node_msg (&node_msg, REPORT_TEMPERATURE, buflen, buffer);

	timenow = time(0);
	timeoffset = get_time_offset();
	realtime = timenow + timeoffset;
	printf("[Temp] Real time at admin node: %d\r\n", realtime);

	buflen = snprintf(buffer, 15, "%d", realtime);
	add_node_msg (&node_msg, TEMP_TIMESTAMP, buflen, buffer);

	char *msg=NULL;
	msg = serilization(node_msg);
	int32_t len = strlen(msg);

	sockfd = get_socket(np, pnp, DATA_SUBMIT);
	send(sockfd, msg, len, 0);

	close(sockfd);
}
