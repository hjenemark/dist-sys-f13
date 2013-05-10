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
			close(new_fd);
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
						/*TODO */
						break;
					/* case MSG TYPE:
						 * DO SOMETHING *
						 * break;
						 */ 
				}
				rx_msg = rx_msg->next;
			}
			exit(0);
        }
        close(new_fd);  // parent doesn't need this

	}
	close(socketfd);
	pthread_exit(NULL);
}
