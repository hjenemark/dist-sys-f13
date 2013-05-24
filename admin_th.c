/**
 * \file admin_th.c
 *
 **/

#include "common.h"
#include "admin_th.h"
#include "globals.h"

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
		/* Prepare local variable */
		int32_t buflen, numbytes;
		char buffer[15];
		struct node_message *node_msg = NULL;
		struct sockaddr_in their_addr;

		printf("[ADMIN DEBUG]: Node is master. Broadcasting.\r\n");

		/* Prepare message containing admin IP address */
		char ipstr[INET6_ADDRSTRLEN];
		strcpy (ipstr,np.ipstr);
		int32_t addrlen = strlen(ipstr);
		printf("[ADMIN DEBUG]: Master IP Len: %d IP: %s\r\n", addrlen, ipstr);
		add_node_msg (&node_msg, ANNOUNCE_MASTER, addrlen, ipstr);

		/* Prepare message containing current admin ID number */
		buflen = snprintf(buffer, 15, "%d", cpy_node_masterid);
		add_node_msg (&node_msg, PROMO_KEY, buflen, buffer);

		/* Serialize messages */
		char *msg=NULL;
		msg = serilization(node_msg);
		int32_t len = strlen(msg);

		/* Send data to broadcast address */
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(ADMIN_PORT);
		struct hostent *he;
		he=gethostbyname("10.10.10.255");
		their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

		if ((numbytes=sendto(socket, msg, len, 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
   			perror("[Admin] sendto");
    		exit(1);
		}
		
		printf("[Admin] Broadcast beacon of size %d sent!\r\n", numbytes);
		sleep(ADMIN_SLEEP_INTERVAL);
	} else {
		/* Node is slave */
		/* Prepare local variables */
		struct timeval tv;
		fd_set readfds;

		tv.tv_sec = ADMIN_SLEEP_INTERVAL;
		tv.tv_usec = 0;

		FD_ZERO(&readfds);
		FD_SET(socket, &readfds);

		/* Wait for data or SLEEP interval whichever first */
		select(socket+1, &readfds, NULL, NULL, &tv);

		/* Check if data was received */
		if(FD_ISSET(socket, &readfds)) {
			/* Some data received */
			socklen_t addr_len;
			struct sockaddr_storage their_addr;
			addr_len = sizeof their_addr;
			int32_t numbytes;
			char s[INET6_ADDRSTRLEN];
			char buf[100];

			/* Prepare temporaty variables */
			char temp_ip[INET6_ADDRSTRLEN];
			temp_ip[0]='0';
			int32_t temp_master_id=0;
			int32_t temp_af_family;

			/* Receive data from socket */
			if ((numbytes = recvfrom(socket, buf, 100-1 , 0,
		    (struct sockaddr *)&their_addr, &addr_len)) == -1) {
	   			perror("recvfrom");
				exit(1);
			}
			//buf[numbytes] = '\0';

			printf("[Admin]: got packet from %s\n",
    			inet_ntop(their_addr.ss_family,
        		get_in_addr((struct sockaddr *)&their_addr),
        		s, sizeof s));

			/* Parse received messages */
			node_msg* rx_msg;
			rx_msg = deserialize(buf);
			print_node_list_msg(rx_msg);
			while(rx_msg != NULL) {
				switch(rx_msg->operation) {
					case ANNOUNCE_MASTER:
						/* Store admin node's IP address to temp variable */
						printf("[Admin]: New master announcement:  %s\n", rx_msg->operand);
						//strncpy(temp_ip, rx_msg->operand, rx_msg->op_size);
						strcpy(temp_ip, rx_msg->operand);
                                                printf("[Admin] Debug: temp_ip: %s\n", temp_ip);
						temp_af_family = their_addr.ss_family;
						if(temp_master_id != 0) {
							/* Master announcement is valid - save it */
							printf("[Admin] New valid master announcment!\r\n");
							pthread_mutex_lock (&mutex_adminp);
							admin_net_params.family=temp_af_family;
							strncpy(admin_net_params.ipstr, temp_ip, strlen(temp_ip));
							current_master_id = temp_master_id;
							pthread_mutex_unlock (&mutex_adminp);
                                                        printf("[Admin] Debug: admin_net_params.ipstr: %s\n", admin_net_params.ipstr);
							temp_master_id=0;
							temp_ip[0] = '0';
							/* New master found. Update time offset */
							calculate_time_offset();
						}
						break;
					case PROMO_KEY:
						printf("[Admin]: New master ID announcement:  %d\n", atoi(rx_msg->operand));
						/* Check if received id is higher than curent */
						printf("[Admin] Master id curr: %d rxd: %d\r\n", current_master_id, atoi(rx_msg->operand) );
						if (current_master_id < atoi(rx_msg->operand))
							temp_master_id = atoi(rx_msg->operand);
						else {
							printf("[Admin] Not updating master id.\r\n");
							temp_master_id = 0;
							temp_ip[0] = '0';
						}
						if(temp_ip[0] != '0') {
							/* Master announcement is valid - save it */
							printf("[Admin] New valid master announcment!\r\n");
							pthread_mutex_lock (&mutex_adminp);
							admin_net_params.family=temp_af_family;
							strncpy(admin_net_params.ipstr, temp_ip, strlen(temp_ip));
							current_master_id = temp_master_id;
							pthread_mutex_unlock (&mutex_adminp);
							temp_master_id=0;
							temp_ip[0] = '0';
							/* New master found. Update time offset */
							calculate_time_offset();
						}
						break;
				}
				rx_msg = rx_msg->next;
			}
		}
	}
}

void calculate_time_offset()
{
	printf("[Admin] Calculating time offset!\r\n");

	// Time values for offset calculation
	int32_t time_req_sent, time_req_rx, time_rep_sent, time_rep_rx;
	
	/* Create TLV requesting average temperature */
	struct node_message* node_msg = NULL;
	add_node_msg(&node_msg, REQ_TIME, 0, NULL);

	/* Serialize TLV for sending */
	char *msg=NULL;
	msg = serilization(node_msg);
	int32_t len = strlen(msg);

	/* Create socket and send data */
	int32_t socket;
        printf("[Admin] Debug: %s\n", np.ipstr);
        printf("[Admin] Debug: %s\n", admin_net_params.ipstr);
        
	socket = get_socket(&np, &admin_net_params, DATA_SUBMIT);
    
	time_req_sent = time(0);
	send(socket, msg, len, 0);

	/* Receive a reply */
	char buff[256];
	int32_t rx_bytes;
	if ((rx_bytes=recv(socket, buff, 256, 0))==0) {
		printf("Remote end closed connection!\r\n");
	}
	time_rep_rx = time(0);
	//buff[rx_bytes] = '\0';
	
	/* Deserialize the data */
	struct node_message* rx_msg;
	rx_msg = deserialize(buff);

	/* Find data containing time values */
	while(rx_msg != NULL) {
		switch(rx_msg->operation) {
			case TIME_REQ_RX:
				time_req_rx = atoi(rx_msg->operand);
				break;
			case TIME_REP_TX:
				time_rep_sent = atoi(rx_msg->operand);
				break;
			default:
				break;
		}
		rx_msg = rx_msg->next;
	}

	/* Calculate time offset */
	int32_t timeoffset;
	timeoffset=(int32_t)((time_req_rx-time_req_sent)+(time_rep_sent-time_rep_rx))/2;
	printf("[Admin] Time offset is %d!\r\n", timeoffset);

	/* Store time offset in global variable */
	pthread_mutex_lock (&mutex_timeoffset);
	node_admin_offset = timeoffset;
	pthread_mutex_unlock (&mutex_timeoffset);
}

