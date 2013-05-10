#include "common.h"
#include "admin_th.h"

void *admin_network_thread_entry(void *np)
{
	/* */
	printf("[Admin] Starting admin thread!\r\n");
	int32_t socketfd;
	struct peer_net_params pnp;
	pnp.family = AF_INET;
	strcpy(pnp.ipstr, "255.255.255.255");

	socketfd = get_socket(np, &pnp, CONTROL_LISTEN);

	recurse_worker(socketfd);

	close(socketfd);
	pthread_exit(NULL);
}

void recurse_worker(int32_t socket)
{
	int32_t cpy_node_master, cpy_node_masterid;
	pthread_mutex_lock (&mutex_master_params);
	cpy_node_master = node_is_master;
	cpy_node_masterid = current_master_id;
	pthread_mutex_unlock (&mutex_master_params);

	if (cpy_node_master) {
		/* Node is master */
		int32_t buflen, numbytes;
		char buffer[15];
		struct node_message *node_msg = NULL;
		struct addrinfo hints, *servinfo;
		
		add_node_msg (&node_msg, ANNOUNCE_MASTER, 0, NULL);
		buflen = snprintf(buffer, 15, "%d", cpy_node_masterid);
		add_node_msg (&node_msg, PROMO_KEY, buflen, buffer);

		char *msg=NULL, port_str[5];
		msg = serilization(node_msg);
		int32_t len = strlen(msg);

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		sprintf(port_str, "%d", ADMIN_PORT);
		getaddrinfo("255.255.255.255", port_str, &hints, &servinfo);

		if ((numbytes = sendto(socket, msg, len, 0,
             servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
    			perror("admin-th: sendto");
    			exit(1);
		}
		printf("[Admin] Broadcast beacon sent!\r\n");
		sleep(ADMIN_SLEEP_INTERVAL);
	} else {
		/* Node is slave */
		struct timeval tv;
		fd_set readfds;

		tv.tv_sec = ADMIN_SLEEP_INTERVAL;
		tv.tv_usec = 0;

		FD_ZERO(&readfds);
		FD_SET(socket, &readfds);

		select(socket+1, &readfds, NULL, NULL, &tv);

		if(FD_ISSET(socket, &readfds)) {
			/* Some data received */
			socklen_t addr_len;
			struct sockaddr_storage their_addr;
			addr_len = sizeof their_addr;
			int numbytes;
			char s[INET6_ADDRSTRLEN];
		
			char buf[100];
		
			if ((numbytes = recvfrom(socket, buf, 100-1 , 0,
		    (struct sockaddr *)&their_addr, &addr_len)) == -1) {
	   			perror("recvfrom");
				exit(1);
			}

			printf("[Admin]: got packet from %s\n",
    			inet_ntop(their_addr.ss_family,
        		get_in_addr((struct sockaddr *)&their_addr),
        		s, sizeof s));

			node_msg* rx_msg;
			rx_msg = deserialize(buf);
			print_node_list_msg(rx_msg);
			while(rx_msg != NULL) {
				switch(rx_msg->operation) {
					case ANNOUNCE_MASTER:
						pthread_mutex_lock (&mutex_adminp);
						strcpy(admin_net_params.ipstr, s);
						admin_net_params.family=their_addr.ss_family;
						pthread_mutex_unlock (&mutex_adminp);
						printf("[Admin]: New master announcement:  %s\n", s);
						break;
					case PROMO_KEY:
						pthread_mutex_lock (&mutex_adminp);
						current_master_id=atoi(rx_msg->operand);
						pthread_mutex_unlock (&mutex_adminp);
						printf("[Admin]: New master id:  %d\n", 
						   	 atoi(rx_msg->operand));
						break;
				}
				rx_msg = rx_msg->next;
			}
			
		}
	}
	recurse_worker (socket);
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}