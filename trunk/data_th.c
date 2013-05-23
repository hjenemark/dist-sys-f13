#include "data_th.h"
#include "globals.h"

struct context {
	int32_t socket;
	struct network_params *np; 
};

node_sens *temp_database;

void *data_network_thread_entry()
{
	printf("[Data] Starting data thread!\r\n");
	int32_t socketfd, new_fd;
	struct peer_net_params pnp;
	socklen_t sin_size;
	struct sockaddr_storage their_addr;
	pnp.family = AF_INET;
	char s[INET6_ADDRSTRLEN];

	struct context con;
	

	socketfd = get_socket(&np, &pnp, DATA_LISTEN);

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

		pthread_t worker_thread;
		con.socket = new_fd;
		//con.np = np;

		if(pthread_create(
			&worker_thread, NULL, 
			data_network_worker, (void *)&con)) {
				printf("Worker thread creation failed!\r\n");
				exit(EXIT_FAILURE);
		}
        

	}
	close(socketfd);
	pthread_exit(NULL);
}

void report_average_temperature(int32_t socket)
{
	printf("[Admin] Average T requested!\r\n");

	int32_t temperature;
	temperature = average_temp(temp_database);
	printf("[Admin] Average T is %d!\r\n", temperature);
	
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
	printf("[Data] New PROMO key received!\r\n");
	printf("[Data] Old PROMO key: %d \r\n", current_master_id);
	int32_t new_key;
	new_key = atoi(keydata);

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

	node_is_master = 1;
	if(np.net_mode == OS_PROVIDED_IP) 
		strcpy(admin_net_params.ipstr, "127.0.0.1");
	else
		strcpy(admin_net_params.ipstr, np.ipstr);
	
	pthread_mutex_unlock (&mutex_master_params);
	pthread_mutex_unlock (&mutex_adminp);
}

void append_db_timestamp(node_sens* temp_database, struct temp_storage *temp_data, char *operand)
{
	printf ("[Data] Appending timestamp to temporary DB.\r\n");
	int32_t timestamp;
	timestamp = atoi(operand);
	temp_data->timestamp = timestamp;
	if(temp_data->temperature != 0) {
		printf ("[Data] Appending temporary DB to master DB.\r\n");
		append_temp_db(
			&temp_database,
		    temp_data->temperature,
		    temp_data->timestamp,
		    temp_data->node_addr);
		memset(temp_data, '\0', sizeof(struct temp_storage));
	}
}

void append_db_data(node_sens* temp_database, struct temp_storage *temp_data, char *operand , struct sockaddr *node_addr)
{
	printf ("[Data] Appending temperature to temporary DB.\r\n");
	int32_t temperature;
	temperature = atoi(operand);
	temp_data->temperature = temperature;
	temp_data->node_addr = node_addr;
	if(temp_data->timestamp !=0) {
		printf ("[Data] Appending temporary DB to master DB.\r\n");
		append_temp_db(
			&temp_database,
		    temp_data->temperature,
		    temp_data->timestamp,
		    temp_data->node_addr);
		memset(temp_data, '\0', sizeof(struct temp_storage));
	}
}

void *data_network_worker(void *con)
{
	//close(socketfd); // child doesn't need the listener
	struct context *con1;
	con1 = (struct context *)con;
	int32_t new_fd;
	new_fd = con1->socket;
	struct temp_storage temp_data;
	char buff[256];
	int rx_bytes;
	
	struct sockaddr_storage their_addr;
	socklen_t their_len = sizeof their_addr;
	getpeername(new_fd, (struct sockaddr*)&their_addr, &their_len);
		
	rx_bytes = recv(new_fd, buff, 256, 0);
	printf("Received %d bytes: \"%s\".\r\n", rx_bytes, buff);
	node_msg* rx_msg;
	rx_msg = deserialize(buff);
	//print_node_list_msg(rx_msg);
	while(rx_msg != NULL) {
		switch(rx_msg->operation) {
			case REPORT_TEMPERATURE:
				append_db_data(temp_database, &temp_data, rx_msg->operand, (struct sockaddr *)&their_addr);
				break;
			case TEMP_TIMESTAMP:
				append_db_timestamp(temp_database, &temp_data, rx_msg->operand);
				break;
			case PROMO_KEY:
				update_promo_key(rx_msg->operand);	
				break;
			case PROMOTE_TO_MASTER:
				become_master();
				break;
			case GET_AVG_TEMP:
				report_average_temperature (new_fd);
				//remove_node_msg(list, *list);
				break;
			default:
				break;
		}
		rx_msg = rx_msg->next;
	}
	close(new_fd);
	pthread_exit(NULL);
}