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

		socklen_t addr_len;
		struct sockaddr_storage their_addr;
		addr_len = sizeof their_addr;
		int numbytes;
		
		char buf[100];
		
		if ((numbytes = recvfrom(socket, buf, 100-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
   			perror("recvfrom");
    		exit(1);
		}

		printf("listener: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);

		sleep(ADMIN_SLEEP_INTERVAL);
	}

	recurse_worker (socket);
}