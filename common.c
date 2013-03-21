#include "common.h"

/* TODO: Encode a message */
void encode_message()
{
}

int get_socket(struct network_params *np, struct peer_net_params *pnp, int socket_type)
{
	if((pnp->family == AF_INET && np->family == AF_INET6) ||
	   (pnp->family == AF_INET6 && np->family == AF_INET)) {
		printf("[Common] Local and Remote IP versions does not match!\r\n");
		return -1;
	}
	struct addrinfo host_hints, *host_res;
	int sockfd, rv;

	memset(&host_hints, 0, sizeof host_hints);

	char *port_str, peer_port_str[5];
	port_str = malloc(5*sizeof(char));
	char *node;

	if (np->net_mode == USER_PROVIDED_IP) {
		host_hints.ai_family = pnp->family;
		host_hints.ai_socktype = SOCK_STREAM;
		node = np->ipstr;
	} else {
		host_hints.ai_family = pnp->family;
		host_hints.ai_socktype = SOCK_STREAM;
		host_hints.ai_flags = AI_PASSIVE;
		node = pnp->ipstr;
	}
		
	switch (socket_type) {
		case CONTROL_LISTEN:
			sprintf(port_str, "%d", ADMIN_PORT);
			host_hints.ai_socktype = SOCK_STREAM;			
			break;
		case DATA_LISTEN:
			sprintf(port_str, "%d", DATA_PORT);
			host_hints.ai_socktype = SOCK_STREAM;			
			break;
		case DATA_SUBMIT:
			port_str = NULL;
			sprintf(peer_port_str, "%d", DATA_PORT);
			host_hints.ai_socktype = SOCK_STREAM;			
			break;
	}

	if ((rv = getaddrinfo(node, port_str, &host_hints, &host_res)) != 0) {
		fprintf(stderr, "getaddrinfo_host: %s\n", gai_strerror(rv));
		return -1;
	}

	if ((sockfd = socket(host_res->ai_family, host_res->ai_socktype, host_res->ai_protocol)) == -1) {
		perror("client: socket");
		return -1;
	}

	if (np->net_mode == USER_PROVIDED_IP) {
		if (bind(sockfd, host_res->ai_addr, host_res->ai_addrlen) == -1) {
			perror("client: bind");
			return -1;
		}
	}

	char host_ipstr[INET6_ADDRSTRLEN];
	void *host_addr;
	int host_port;
    char *ipver;
	
	if (host_res->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)host_res->ai_addr;
        host_addr = &(ipv4->sin_addr);
		host_port = ipv4->sin_port;
        ipver = "IPv4";
    }
	inet_ntop(host_res->ai_family, host_addr, host_ipstr, sizeof host_ipstr);
	printf("[Common] Host ver %s Host addr: %s:%d\r\n", ipver, host_ipstr, ntohs(host_port));

	struct addrinfo peer_hints, *peer_res;
	memset(&peer_hints, 0, sizeof peer_hints);

	peer_hints.ai_family = pnp->family;
	
	if ((rv = getaddrinfo(pnp->ipstr, peer_port_str, &peer_hints, &peer_res)) != 0) {
		fprintf(stderr, "getaddrinfo_peer: %s\n", gai_strerror(rv));
		return -1;
	}
	
	char peer_ipstr[INET6_ADDRSTRLEN];
	void *peer_addr;
	int peer_port;
	
	if (peer_res->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)peer_res->ai_addr;
        peer_addr = &(ipv4->sin_addr);
		peer_port = ipv4->sin_port;
        ipver = "IPv4";
    }
	inet_ntop(peer_res->ai_family, peer_addr, peer_ipstr, sizeof peer_ipstr);
	printf("[Common] Peer ver %s Peer addr: %s:%d\r\n", ipver, peer_ipstr, ntohs(peer_port));
	
	if (connect(sockfd, peer_res->ai_addr, peer_res->ai_addrlen) == -1) {
		close(sockfd);
		perror("client: connect");
		return -1;
    }

	freeaddrinfo(peer_res);
	freeaddrinfo(host_res);
		
	return sockfd;
}