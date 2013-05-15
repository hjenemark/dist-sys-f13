/**
 * \file user.c
 **/
#include <stdbool.h>
#include "common.h"

uint32_t connect_to_node(struct peer_net_params *pnp, uint32_t *socket)
{
	printf("Enter node IP address:\r\n");
	scanf("%s", pnp->ipstr);

	struct network_params np;

	np.net_mode = OS_PROVIDED_IP;
	pnp->family = AF_INET;
	
	if((*socket = get_socket(&np, pnp, DATA_SUBMIT)) == -1) return -1;
	return 0;
}
uint32_t promote_node(uint32_t socket, uint32_t *promokey)
{
	printf("Promoting to master with master key: %u\r\n",*promokey);

	struct node_message* node_msg = NULL;
	add_node_msg (&node_msg, PROMOTE_TO_MASTER, 0, NULL);

	char buffer[10];
	uint8_t buflen = snprintf(buffer, 10, "%d", (*promokey));
	add_node_msg (&node_msg, PROMO_KEY, buflen, buffer);

	char *msg=NULL;
	msg = serilization(node_msg);
	int32_t len = strlen(msg);

	int32_t sentbytes;
	sentbytes = send(socket, msg, len, 0);
	if (sentbytes != len) {
		printf("Unable to send message. Socket will be closed");
		close(socket);
		return 1;
	}
	
	(*promokey)++;
	return 0;
}
uint32_t get_avg_temp(uint32_t socket)
{
	printf("Requesting average temperature!\r\n");
	struct node_message* node_msg = NULL;
	add_node_msg (&node_msg, GET_AVG_TEMP, 0, NULL);

	char *msg=NULL;
	msg = serilization (node_msg);
	int32_t len = strlen(msg);

	send(socket, msg, len, 0);

	char buff[256];
	int32_t rx_bytes;
	if((rx_bytes=recv(socket, buff, 256, 0))==0) {
		printf("Remote end closed connection!\r\n");
		return 1;
	}
	buff[rx_bytes] = '\0';
	
	struct node_message* rx_msg;
	rx_msg = deserialize(buff);

    while (rx_msg != NULL) {
		if (rx_msg->operation == REPORT_AVG_TEMP)
			printf("Average temperature: %d\r\n",atoi(rx_msg->operand));
		rx_msg = rx_msg->next;
	}
	
	return 0;
}

int32_t main ()
{
	bool admin_connected = false, wait;
	uint32_t socket, promokey = 1, error = 0, retval; 
	char option;
	struct peer_net_params pnp;

	struct timespec tv;
	tv.tv_sec = 3;
	tv.tv_nsec = 0;

	fd_set readfds;
	FD_ZERO(&readfds);

	printf("User node program.\r\n");
	while (1) {
		admin_connected ? 
			printf("Connected to: %s\r\n", pnp.ipstr) : 
			printf("Connected to: NONE\r\n") ;

		printf("Select task:\r\n");
		printf("[1] Quit.\r\n");
		printf("[2] Connect to node.\r\n");
		if (admin_connected) {
			printf("[3] Promote node to Admin mode.\r\n");
			printf("[4] Get average T from node.\r\n");
		}

		wait = true;
		while(wait) {
			FD_SET(STDIN_FILENO, &readfds);
			pselect(STDIN_FILENO+1, &readfds, NULL, NULL, &tv, NULL);
			if(FD_ISSET(STDIN_FILENO, &readfds)) {
				scanf("%c", &option);
				wait = false;
			} else if(admin_connected) {
				socklen_t len = sizeof (error);
				retval = getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &len );
				if(retval != 0) {
					printf("Socket is broken!\r\n");
					admin_connected = false;
					wait = false;
					option = 'x';
				}
			}
		}

		switch(option) {
			case '1':
				if (admin_connected) close(socket);
				exit(EXIT_SUCCESS);
				break;
			case '2':
				if (admin_connected) close(socket);
				if(connect_to_node(&pnp, &socket) == -1) {
					printf("Connecting failed!\r\n");
					admin_connected = false;
				}
				else
					admin_connected = true;
				break;
			case '3':
				if(promote_node(socket, &promokey)==1) admin_connected = false;
				break;
			case '4':
				if(get_avg_temp(socket)==1) admin_connected = false;
				break;
			case 'x':
				break;
			default:
				printf("Unrecognized command!\r\n");
				break;
		}
	}
}