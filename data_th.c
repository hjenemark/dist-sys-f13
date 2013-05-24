/**
 * \file data_th.c
 **/

#include "data_th.h"
#include "globals.h"

pthread_mutex_t database_access; /* Mutex to controll DB access */
node_sens *temp_database = NULL; /* Initialize Temperature Database */

void *data_network_thread_entry()
{
	printf("[Data] Starting data thread!\r\n");
	/* Prepare variables */
	int32_t socketfd, new_fd;
	struct peer_net_params pnp;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;
	pnp.family = AF_INET;
	char s[INET6_ADDRSTRLEN];

	/* Initialize mutex */
	pthread_mutex_init(&database_access, NULL);

	/* Create socket */
	socketfd = get_socket(&np, &pnp, DATA_LISTEN);

	/* Main work loop */
    while(1) {
		/* Accept connection */
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

		/* Create hread to handle connection */
		pthread_t worker_thread;
		if(pthread_create(
			&worker_thread, NULL, 
			data_network_worker, (void *)&new_fd)) {
				printf("Worker thread creation failed!\r\n");
				exit(EXIT_FAILURE);
		}
	}
	close(socketfd);
	pthread_exit(NULL);
}

void report_average_temperature(int32_t socket)
{
	printf("[Data] Average T requested!\r\n");

	/* Request temperature from database */
	int32_t temperature;
	temperature = average_temp(temp_database);
	printf("[Data] Average T is %d!\r\n", temperature);
	
	char buffer[15];
	int32_t buflen;
	buflen = snprintf(buffer, 15, "%d", temperature);

	/* Prepare message containing temperature */
	struct node_message* reply_message = NULL;
	add_node_msg (&reply_message, REPORT_AVG_TEMP, buflen, buffer);

	/* Serialize the message */
	char *reply_string=NULL;
	reply_string = serilization (reply_message);
	int32_t reply_len = strlen (reply_string);

	/* Send the message */
	send(socket, reply_string, reply_len, 0);
}

void update_promo_key(char *keydata)
{
	printf("[Data] New PROMO key received!\r\n");
	printf("[Data] Old PROMO key: %d \r\n", current_master_id);

	/* Get key data */
	int32_t new_key;
	new_key = atoi(keydata);

	/* Update key data */
	pthread_mutex_lock (&mutex_master_params);
	current_master_id = new_key;
	printf("[Data] New PROMO is %d!\r\n", current_master_id);
	pthread_mutex_unlock (&mutex_master_params);
}

void become_master()
{
	printf("[Data] Node promoted to be master!\r\n");
	pthread_mutex_lock (&mutex_master_params);
	pthread_mutex_lock (&mutex_adminp);

	/* Change node parameter to become master */
	node_is_master = 1;

	/* Update master node's IP address */
	if(np.net_mode == OS_PROVIDED_IP) 
		strcpy(admin_net_params.ipstr, "127.0.0.1");
	else
		strcpy(admin_net_params.ipstr, np.ipstr);

	/* Release mutexes after update */
	pthread_mutex_unlock (&mutex_master_params);
	pthread_mutex_unlock (&mutex_adminp);
}

void append_db_timestamp(struct temp_storage *temp_data, char *operand)
{
	printf ("[Data] Appending timestamp to temporary DB.\r\n");
	/* Prepare variables */
	int32_t timestamp;
	timestamp = atoi(operand);
	temp_data->timestamp = timestamp;
	
	/* If temporary data ready, append to master DB */
	if(temp_data->temperature != 0) {
		printf ("[Data] Appending temporary DB to master DB.\r\n");
		pthread_mutex_lock (&database_access);
		append_temp_db(
			&temp_database,
		    temp_data->temperature,
		    temp_data->timestamp,
		    temp_data->node_addr);
		pthread_mutex_unlock (&database_access);
		memset(temp_data, '\0', sizeof(struct temp_storage));
	}
}

void append_db_data(struct temp_storage *temp_data, char *operand , struct sockaddr_storage *node_addr)
{
	printf ("[Data] Appending temperature to temporary DB.\r\n");

	/* Prepare variables */
	int32_t temperature;
	temperature = atoi(operand);
	temp_data->temperature = temperature;
	temp_data->node_addr = node_addr;

	/* If temporary data is ready append to master DB */
	if(temp_data->timestamp !=0) {
		printf ("[Data] Appending temporary DB to master DB.\r\n");
		pthread_mutex_lock (&database_access);
		append_temp_db(
			&temp_database,
		    temp_data->temperature,
		    temp_data->timestamp,
		    temp_data->node_addr);
		pthread_mutex_unlock (&database_access);
		memset(temp_data, '\0', sizeof(struct temp_storage));
	}
}

void report_time_values(int32_t socket, int32_t time_rx)
{
	printf("[Data] Timestamps requested!\r\n");

	/* Prepare variables */
	char buffer[15];
	int32_t buflen, time_tx;
	struct node_message* reply_message = NULL;

	/* Append message with first timestamp */
	buflen = snprintf(buffer, 15, "%d", time_rx);
	add_node_msg (&reply_message, TIME_REQ_RX, buflen, buffer);

	/* Append message with second timestamp */
	time_tx = time(0);
	buflen = snprintf(buffer, 15, "%d", time_tx);
	add_node_msg (&reply_message, TIME_REP_TX, buflen, buffer);

	/* Serialize the message */
	char *reply_string=NULL;
	reply_string = serilization (reply_message);
	int32_t reply_len = strlen (reply_string);

	/* Send the message */
	send(socket, reply_string, reply_len, 0);
}

void *data_network_worker(void *sock)
{
	/* Prepare variables */
	int32_t new_fd, rx_bytes, timerx;
	new_fd = *((int *) sock);
	struct temp_storage temp_data;
	char buff[256];

	/* Get address of remote end */
	struct sockaddr_storage their_addr;
	socklen_t their_len = sizeof their_addr;
	getpeername(new_fd, (struct sockaddr*)&their_addr, &their_len);

	/* Receive the message */
	rx_bytes = recv(new_fd, buff, 256, 0);
	timerx = time(0);
	printf("Received %d bytes: \"%s\".\r\n", rx_bytes, buff);

	/* Process the message */
	node_msg* rx_msg;
	rx_msg = deserialize(buff);
	while(rx_msg != NULL) {
		switch(rx_msg->operation) {
			case REPORT_TEMPERATURE:
				append_db_data(&temp_data, rx_msg->operand, &their_addr);
				break;
			case TEMP_TIMESTAMP:
				append_db_timestamp(&temp_data, rx_msg->operand);
				break;
			case PROMO_KEY:
				update_promo_key(rx_msg->operand);	
				break;
			case PROMOTE_TO_MASTER:
				become_master();
				break;
			case GET_AVG_TEMP:
				report_average_temperature (new_fd);
				break;
			case REQ_TIME:
				report_time_values(new_fd, timerx);
			default:
				break;
		}
		rx_msg = rx_msg->next;
	}
	/* Cleanup after the thread is finished */
	close(new_fd);
	pthread_exit(NULL);
}
