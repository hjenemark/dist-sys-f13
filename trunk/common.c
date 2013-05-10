/**
 * \file common.c
 **/

#include "common.h"

/* TODO: Encode a message */
//char* encode_message( int8_t temperature, char* ipstr, uint32_t operation ) 
char* encode_message(uint32_t operation, ... ) 
{
    va_list arguments;
    va_start(arguments, operation);
    node_msg* temp_msg = (node_msg*)calloc(1, sizeof(node_msg));
    char* serilized=NULL;
    if(operation == REPORT_TEMPERATURE){
        /*11: length of timestamp
         * 1: length of temperature
         *15: length of IP address string 
         * 2: seperating commas
         * adds up to 28 bytes for operand
         */
        temp_msg->operand = (char*)calloc(29, sizeof(char));
        long current_time = time(NULL);
        int32_t temperature = (int32_t)va_arg(arguments, int);
        char* ipstr = va_arg(arguments, char*);
        snprintf(temp_msg->operand, 29, "%li,%i,%s", 
                current_time, 
                temperature,
                ipstr);
    }

    temp_msg->op_size = strlen(temp_msg->operand);
    temp_msg->operation = operation;
    
    va_end(arguments);
    serilized = serilization(temp_msg);
    free(temp_msg->operand);
    free(temp_msg);
    return serilized;
}

void* decode_message( char* message){
    int32_t marker = 0;
    int32_t operation;
    int8_t list_type;
    int32_t op_size;
    char* operand;
    int32_t msg_len = strlen(message);
    node_msg* current = NULL;
    node_sens* temp_list = NULL;
    list_type = 
            (message[marker + 0] - '0') * 1000 + 
            (message[marker + 1] - '0') * 100 + 
            (message[marker + 2] - '0') * 10 + 
            (message[marker + 3] - '0');

    node_msg* list = NULL;

    while(msg_len > marker){
        operation = 
            (message[marker + 0] - '0') * 1000 + 
            (message[marker + 1] - '0') * 100 + 
            (message[marker + 2] - '0') * 10 + 
            (message[marker + 3] - '0');
        marker += 4;
        op_size = 
            (message[marker + 0] - '0') * 1000 + 
            (message[marker + 1] - '0') * 100 + 
            (message[marker + 2] - '0') * 10 + 
            (message[marker + 3] - '0');
        marker += 4;

        operand = (char*)calloc(op_size + 1, sizeof(char));
        memmove(operand, (message + marker), op_size);
        marker += op_size;
        add_node_msg(&list, operation, op_size, operand);
        free(operand);
    }

    current = list;

    char* time_char = NULL;
    char* sens_value_char = NULL;
    char* id = NULL;
    while(current != NULL){
        if(current->operation == REPORT_TEMPERATURE){
            char** saveptr = (char**)malloc(current->op_size*sizeof(char));
            time_char = strtok_r(current->operand, ",", saveptr);
            sens_value_char = strtok_r(NULL, ",", saveptr);
            id = strtok_r(NULL, ",", saveptr);
            free(saveptr);
            int32_t timestamp = 0;
            int32_t sens_val = 0;

            marker = 0;
            while(marker < strlen(time_char)){
                timestamp = (timestamp * 10) + (time_char[marker] - '0');
                marker += 1;
            }
            marker = 0;
            while(marker < strlen(sens_value_char)){
                sens_val = (sens_val *10) + (sens_value_char[marker] - '0');
                marker += 1;
            }
            
            //add_node_sens(&temp_list, id, timestamp, sens_val);
        }
        current = current->next;
    }

    clear_list_msg(&list);
    if(list_type == REPORT_TEMPERATURE){
        return temp_list;
    } else {
        return NULL;
    }
} 

int get_socket(struct network_params *np, struct peer_net_params *pnp, int socket_type)
{
	/**
     * Ensure that local and remote IP versions does not conflict.
    */
	if((pnp->family == AF_INET && np->family == AF_INET6) ||
	   (pnp->family == AF_INET6 && np->family == AF_INET)) {
		printf("[Common] Local and Remote IP versions does not match!\r\n");
		return -1;
	}

	/**
     * Populate hints for network setup
    */
	struct addrinfo host_hints, *host_res;
	int sockfd, rv;

	memset(&host_hints, 0, sizeof host_hints);

	char *port_str, peer_port_str[5];
	port_str = malloc(5*sizeof(char));
	char *node;

	if (np->net_mode == USER_PROVIDED_IP) {
		host_hints.ai_family = pnp->family;
		host_hints.ai_socktype = SOCK_STREAM;
		node = np->ipstr; //TODO Fix this
	} else {
		host_hints.ai_family = pnp->family;
		host_hints.ai_socktype = SOCK_STREAM;
		host_hints.ai_flags = AI_PASSIVE;
		node = NULL;
	}

	/**
     * Populates hints based on socket type.
    */
	switch (socket_type) {
		case CONTROL_LISTEN:
			sprintf(port_str, "%d", ADMIN_PORT);
			sprintf(peer_port_str, "%d", ADMIN_PORT);
			host_hints.ai_socktype = SOCK_DGRAM;			
			break;
		case DATA_LISTEN:
			sprintf(port_str, "%d", DATA_PORT);
			host_hints.ai_socktype = SOCK_STREAM;			
			break;
		case DATA_SUBMIT:
			//port_str = NULL;
			sprintf(port_str, "%d", 0);
			sprintf(peer_port_str, "%d", DATA_PORT);
			host_hints.ai_socktype = SOCK_STREAM;			
			break;
	}

	if ((rv = getaddrinfo(node, port_str, &host_hints, &host_res)) != 0) {
		fprintf(stderr, "getaddrinfo_host: %s\n", gai_strerror(rv));
		return -1;
	}

	/**
     * Create and bind socket to required parameters
    */
	if ((sockfd = socket(host_res->ai_family, host_res->ai_socktype, host_res->ai_protocol)) == -1) {
		perror("client: socket");
		return -1;
	}

	/**
	 * Make socket broadcast
	 */
	int broadcast = 1;
	if(socket_type == CONTROL_LISTEN) {
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
		    sizeof broadcast) == -1) {
		    perror("setsockopt (SO_BROADCAST)");
		    exit(1);
		}
	}

	if (np->net_mode == USER_PROVIDED_IP || socket_type == CONTROL_LISTEN) {
		if (bind(sockfd, host_res->ai_addr, host_res->ai_addrlen) == -1) {
			perror("client: bind");
			return -1;
		}
	}



	/**
     * Print informations about local socket.
    */
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

	if(socket_type == CONTROL_LISTEN) {
		freeaddrinfo(host_res);
		return sockfd;
	}
	
	/**
     * Populates hints for remote connection.
    */
	struct addrinfo peer_hints, *peer_res;
	memset(&peer_hints, 0, sizeof peer_hints);

	peer_hints.ai_family = pnp->family;
	
	if ((rv = getaddrinfo(pnp->ipstr, peer_port_str, &peer_hints, &peer_res)) != 0) {
		fprintf(stderr, "getaddrinfo_peer: %s\n", gai_strerror(rv));
		return -1;
	}

	/**
     * Print information about remote connection.
    */
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
	
	/**
     * Create connection and free no longer needed hints.
    */
	if (connect(sockfd, peer_res->ai_addr, peer_res->ai_addrlen) == -1) {
		close(sockfd);
		perror("client: connect");
		return -1;
    }

	freeaddrinfo(peer_res);
	freeaddrinfo(host_res);
		
	return sockfd;
}
