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
	pnp->domain = AF_INET;
	char my_ip_addr[] = "192.168.1.74";
	pnp->text_ip_addr = my_ip_addr;
	return 0;
}

void send_temperature(struct network_params *np, struct peer_net_params *pnp,
		      int8_t temperature)
{
	printf("[Temp] Sending temperature \"%d\" to server \"%s\".\r\n", temperature, pnp->text_ip_addr);	
	/* TODO: message encoding needs work */	
	encode_message();	


	struct addrinfo hints, *res;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	/* TODO: Add network params from peer network params */
	char str[15];
	sprintf(str, "%d", DATA_PORT);	
	getaddrinfo("127.0.0.1", str, &hints, &res);

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	connect(sockfd, res->ai_addr, res->ai_addrlen);

	char *msg = "Temperature!";
	int len;

	len = strlen(msg);
	send(sockfd, msg, len, 0);

	close(sockfd);
	freeaddrinfo(res);
}
