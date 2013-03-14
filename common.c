#include "common.h"

/* TODO: Encode a message */
void encode_message()
{
}

/* TODO: Create socket based on requirements */
int get_socket(struct network_params *np, struct peer_net_params *pnp, int socket_type)
{
	struct addrinfo hints, *res;
	int sockfd, rv;

	memset(&hints, 0, sizeof hints);
	
	if (np->net_mode == USER_PROVIDED_IP) {
		/* TODO: Bind to specific user provided IP */
	} else {
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
	}

	char port_str[5];
	char *node;		
	switch (socket_type) {
		case CONTROL_LISTEN:
			sprintf(port_str, "%d", ADMIN_PORT);
			node=NULL;			
			break;
		case DATA_LISTEN:
			sprintf(port_str, "%d", DATA_PORT);
			node=NULL;			
			break;
		case DATA_SUBMIT:
			sprintf(port_str, "%d", DATA_PORT);
			/* TODO Populate node */			
			break;
	}

	/* TODO: Add network params from peer network params */
	
	
	if ((rv = getaddrinfo(node, port_str, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}	
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		perror("client: socket");
		return -1;
	}
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		close(sockfd);
		perror("client: connect");
		return -1;
        }

	freeaddrinfo(res);
	return sockfd;
}
