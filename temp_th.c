#include "temp_th.h"

#define MAXDATASIZE 100 // max number of bytes we can get at once 

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

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


	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(pnp->text_ip_addr, DATA_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)		
	connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)

	inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr),
	    s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	
	if (send(sockfd, "Hello, world!", 13, 0) == -1) perror("send");

	close(sockfd);
}
