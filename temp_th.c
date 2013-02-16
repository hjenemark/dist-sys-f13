#include "temp_th.h"

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
	int sock;
	struct sockaddr_in tempsender;
	char buffer[512];
	uint32_t echolen;
	uint32_t received = 0;
	/* ATTN = UDP Socket !!!! */
	if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("[Temp] Failed to create socket");
		pthread_exit(NULL);
	}
	
	memset(&tempsender, 0, sizeof(tempsender)); 	/* Clear struct */
	tempsender.sin_family = np->domain; 		/* Internet/IP */
	tempsender.sin_addr.s_addr = np->buf;		/* IP address */
	tempsender.sin_port = DATA_PORT; 		/* server port */
	
	if (connect(
		sock, (struct sockaddr *) &tempsender, sizeof(tempsender)) < 0) {
		perror("[Temp] Failed to connect with server");
		pthread_exit(NULL);
	}

	send(sock, temperature, sizeof(temperature), 0);
}
