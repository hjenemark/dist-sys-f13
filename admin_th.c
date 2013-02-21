#include "admin_th.h"

void *admin_network_thread_entry(void *np)
{
	/* */
	printf("[Admin]\r\n");
	pthread_exit(NULL);
}
