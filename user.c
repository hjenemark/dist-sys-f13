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
	
	*socket = get_socket(&np, pnp, DATA_SUBMIT);
	return 0;
}
uint32_t promote_node(uint32_t socket)
{
	return 0;
}
uint32_t get_avg_temp(uint32_t socket)
{
	return 0;
}

int32_t main ()
{
	bool admin_connected = false;
	uint32_t socket;
	struct peer_net_params pnp;
	
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
		

		uint32_t option;
		scanf("%d", &option);

		switch(option) {
			case 1:
				if (admin_connected) close(socket);
				exit(EXIT_SUCCESS);
				break;
			case 2:
				if (admin_connected) close(socket);
				connect_to_node(&pnp, &socket);
				admin_connected = true;
				break;
			case 3:
				promote_node(socket);
				break;
			case 4:
				get_avg_temp(socket);
				break;
				
		}
	}
}