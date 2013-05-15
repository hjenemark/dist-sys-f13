#include "data_th.h"

void *data_network_thread_entry(void *np)
{
	printf("[Admin] Starting data thread!\r\n");
	int32_t socketfd, new_fd;
	struct peer_net_params pnp;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;
	pnp.family = AF_INET;
	char s[INET6_ADDRSTRLEN];

	socketfd = get_socket(np, &pnp, DATA_LISTEN);

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
		
		inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
        	close(socketfd); // child doesn't need the listener
        	char buff[256];
			int rx_bytes;
			rx_bytes = recv(new_fd, buff, 256, 0);
			printf("Received %d bytes: \"%s\".\r\n", rx_bytes, buff);
			//struct node_message* rx_msg;
			node_msg* rx_msg;
			rx_msg = deserialize(buff);
			print_node_list_msg(rx_msg);
			while(rx_msg != NULL) {
				switch(rx_msg->operation) {
					case ANNOUNCE_MASTER:
						/*TODO */
						break;
					case PROMO_KEY:
						update_promo_key(rx_msg->operand);
						break;
					case PROMOTE_TO_MASTER:
						become_master(np);
						break;
					case GET_AVG_TEMP:
						report_average_temperature (new_fd);
						break;
					default:
						break;
					/* case MSG TYPE:
						 * DO SOMETHING *
						 * break;
						 */ 
				}
				rx_msg = rx_msg->next;
			}
			close(new_fd);
			exit(0);
        }
        close(new_fd);  // parent doesn't need this

	}
	close(socketfd);
	pthread_exit(NULL);
}

void report_average_temperature(int32_t socket)
{
	printf("[Admin] Average T requested!\r\n");

	/* TODO: Fix this! */
	int32_t temperature = 7;
	
	char buffer[15];
	int32_t buflen;
	buflen = snprintf(buffer, 15, "%d", temperature);
	
	struct node_message* reply_message = NULL;
	add_node_msg (&reply_message, REPORT_AVG_TEMP, buflen, buffer);

	char *reply_string=NULL;
	reply_string = serilization (reply_message);
	int32_t reply_len = strlen (reply_string);

	send(socket, reply_string, reply_len, 0);
}

void update_promo_key(char *keydata)
{
	printf("[Admin] New PROMO key received!\r\n");
	int32_t new_key;
	new_key = atoi(keydata);

	printf("[Admin] New PROMO is %d!\r\n", new_key);
	pthread_mutex_lock (&mutex_master_params);
	current_master_id = new_key;
	pthread_mutex_unlock (&mutex_master_params);
}

void become_master(struct network_params *np)
{
	printf("[Admin] Node promoted to be master!\r\n");
	pthread_mutex_lock (&mutex_master_params);
	pthread_mutex_lock (&mutex_adminp);

	node_is_master = 1;
	if(np->net_mode == OS_PROVIDED_IP) 
		strcpy(admin_net_params.ipstr, "127.0.0.1");
	else
		strcpy(admin_net_params.ipstr, np->ipstr);
	
	pthread_mutex_unlock (&mutex_master_params);
	pthread_mutex_unlock (&mutex_adminp);
}
