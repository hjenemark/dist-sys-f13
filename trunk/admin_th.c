/**
 * \file admin_th.c
 *
 * Worker functions for the admin thread
 **/

#include "common.h"
#include "admin_th.h"
#include "globals.h"

/**
 * \brief Admin thread main function
 **/
void *admin_network_thread_entry()
{
	printf("[Admin] Starting admin thread!\r\n");
	int32_t socketfd;
	struct peer_net_params pnp;
	pnp.family = AF_INET;
	//strcpy(pnp.ipstr, "10.10.10.255");
	strcpy(pnp.ipstr, "255.255.255.255");
	struct network_params np1;
	np1.net_mode = USER_PROVIDED_IP;
	np1.family = AF_INET;
	//strcpy(np1.ipstr, "10.10.10.255");
	strcpy(np1.ipstr, "10.10.10.255");

	socketfd = get_socket(&np1, &pnp, CONTROL_LISTEN);

	while (1) {
		recurse_worker(socketfd);
		sleep(ADMIN_SLEEP_INTERVAL);
	}

	close(socketfd);
	pthread_exit(NULL);
}

/**
 * \brief Does the work of the admin thread.
 *
 * If the node is the admin a message is broadcast on the network.
 * Otherwise it waits for the promote message.
 **/
void recurse_worker(int32_t socket)
{
	int32_t cpy_node_master, cpy_node_masterid;
	pthread_mutex_lock (&mutex_master_params);
	cpy_node_master = node_is_master;
	cpy_node_masterid = current_master_id;
	pthread_mutex_unlock (&mutex_master_params);

	printf("[Admin]DEBUG: masterid: %d\r\n", cpy_node_masterid);

	if (cpy_node_master) {
		/* Node is master */
		int32_t buflen, numbytes;
		char buffer[15];
		struct node_message *node_msg = NULL;
		struct addrinfo hints, *servinfo;
		struct sockaddr_in their_addr;

		printf("ADMIN DEBUG: Node is master. Broadcasting.\r\n");
		char ipstr[INET6_ADDRSTRLEN];
		strcpy (ipstr,np.ipstr);
		int32_t addrlen = strlen(ipstr);
		printf("ADMIN DEBUG: Master IP Len: %d IP: %s\r\n", addrlen, ipstr);
		
		add_node_msg (&node_msg, ANNOUNCE_MASTER, addrlen, ipstr);
		buflen = snprintf(buffer, 15, "%d", cpy_node_masterid);
		add_node_msg (&node_msg, PROMO_KEY, buflen, buffer);

		char *msg=NULL, port_str[5];
		msg = serilization(node_msg);
		int32_t len = strlen(msg);

		/*memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		sprintf(port_str, "%d", ADMIN_PORT);
		getaddrinfo("255.255.255.255", port_str, &hints, &servinfo);*/
		their_addr.sin_family = AF_INET;     // host byte order
		their_addr.sin_port = htons(51002); // short, network byte order
		struct hostent *he;
		he=gethostbyname("10.10.10.255");
		their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

		if ((numbytes=sendto(socket, msg, len, 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
   			perror("sendto");
    		exit(1);
		}
		
		/*if ((numbytes = sendto(socket, msg, len, 0,
             servinfo->ai_addr, servinfo->ai_addrlen)) == -1) {
    			perror("admin-th: sendto");
    			exit(1);
		}*/
		printf("[Admin] Broadcast beacon size %d sent!\r\n",numbytes);
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

			char ipstr[INET6_ADDRSTRLEN];
			struct sockaddr_in *st = (struct sockaddr_in *)&their_addr;
			inet_ntop(AF_INET, &st->sin_addr, ipstr, sizeof ipstr);
			printf("[DEBUG] REAL Sensor IP address: %s\n", ipstr);

			node_msg* rx_msg;
			rx_msg = deserialize(buf);
			print_node_list_msg(rx_msg);
			while(rx_msg != NULL) {
				switch(rx_msg->operation) {
					case ANNOUNCE_MASTER:
						pthread_mutex_lock (&mutex_adminp);
						strncpy(admin_net_params.ipstr, rx_msg->operand, rx_msg->op_size);
						admin_net_params.family=their_addr.ss_family;
						pthread_mutex_unlock (&mutex_adminp);
						printf("[Admin]: New master announcement:  %s\n", admin_net_params.ipstr);
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
}

