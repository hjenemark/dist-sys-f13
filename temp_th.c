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

/* TODO: Implement global admin parameters */
int8_t get_admin_params(struct peer_net_params *pnp)
{
	pnp->family = AF_INET;
	strcpy(pnp->ipstr, "127.0.0.1");
	//strcpy(pnp->ipstr, "10.0.0.10");
	return 0;
}

void send_temperature(struct network_params *np, struct peer_net_params *pnp,
		      int8_t temperature)
{
	/* Status output for loging and debug */
	printf("[Temp] Sending temperature \"%d\" to server \"%s\".\r\n", 
	   	 temperature, pnp->ipstr);	

	int32_t sockfd, buflen;

	char buffer[10];
	buflen = sprintf(buffer, "%d", temperature);

	struct node_message node_msg;
	node_msg.operation = REPORT_TEMPERATURE;
	node_msg.op_size = buflen;
	node_msg.operand = buffer;
	node_msg.next = NULL;

	char *msg=NULL;
	msg = serilization(&node_msg);
	int32_t len = strlen(msg);

	sockfd = get_socket(np, pnp, DATA_SUBMIT);
	send(sockfd, msg, len, 0);

	close(sockfd);
}
