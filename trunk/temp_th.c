/**
 * \file temp_th.c
 **/
#include "temp_th.h"

#define MAXDATASIZE 100 // max number of bytes we can get at once 

void *temperature_thread_entry(void *np)
{
	int8_t temperature;
	struct peer_net_params admin_addr;

	/*	
	struct network_params *my_np;
	my_np = (struct network_params *)np;	
	*/

	while (1) {
		temperature = get_temperature();
		get_admin_params(&admin_addr);
                //printf(
		
		/*send_temperature(my_np, &admin_addr, temperature);*/
		send_temperature((struct network_params *)np, &admin_addr, temperature);		

		sleep(SENSOR_SLEEP_INTERVAL);
	}
}

/* TODO: Implement temperature reading */
int8_t get_temperature()
{
	return 8;
}

/* TODO: Implement global admin parameters */
int8_t get_admin_params(struct peer_net_params *pnp)
{
	pnp->family = AF_INET;
	strcpy(pnp->ipstr, "127.0.0.1");
	return 0;
}

void send_temperature(struct network_params *np, struct peer_net_params *pnp,
		      int8_t temperature)
{
	/* Status output for loging and debug */
	printf("[Temp] Sending temperature \"%d\" to server \"%s\".\r\n", 
	   	 temperature, pnp->ipstr);
	
	/* TODO: message encoding needs work */	
	encode_message();	

	int sockfd;

	sockfd = get_socket(np, pnp, DATA_SUBMIT);

	char *msg = "Temperature!";
	int len;

	len = strlen(msg);
	send(sockfd, msg, len, 0);

	close(sockfd);
}
