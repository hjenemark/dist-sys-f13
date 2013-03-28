/**
 * \file user.c
 **/

#include "common.h"

int connect_to_node()
{
	return 0;
}
int promote_node() 
{
	return 0;
}
int get_avg_temp() 
{
	return 0;
}
int main (int argc, char *argv[])
{
	printf("User node program.\r\n");
	while (1) {
		printf("Select task:\r\n");
		printf("[1] Connect to Admin Node.\r\n");
		printf("[2] Promote node to Admin node.\r\n");
		printf("[3] Get average T from Admin node.\r\n");
		printf("[4] Quit.\r\n");

		int option;
		scanf("%d", &option);

		switch(option) {
			case 1:
				connect_to_node();
				break;
			case 2:
				promote_node();
				break;
			case 3:
				get_avg_temp();
				break;
			case 4:
				exit(EXIT_SUCCESS);
				break;
		}
	}
}