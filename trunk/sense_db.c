
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "sens_list.h"
#include "sense_db.h"

//uint32_t append_temp_db(node_sens** list, char* node_id, time_t timestamp, int32_t sens_value){
uint32_t append_temp_db(node_sens** list, int32_t temperature, int32_t timestamp, struct sockaddr_storage *node_addr){
	printf("[DEBUG]Request to add:\r\n");
	printf("[DEBUG]Temerature:%d\r\n", temperature);
	printf("[DEBUG]Timestamp:%d\r\n", timestamp);
	char ipstr[INET6_ADDRSTRLEN];
	struct sockaddr_in *s = (struct sockaddr_in *)node_addr;
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	printf("[DEBUG]Sensor IP address: %s\n", ipstr);

	node_sens* current = *list;
    bool not_found = true;
    struct sockaddr_storage *current_addr, *new_addr;
    new_addr = node_addr;

    while(current != NULL){

        current_addr = current->node_addr;
        if(value_expired(current->timestamp)){
            remove_node_sens(list, current);
        //}else if(strncmp(current->node_addr, node_addr, NODE_ID_LENGTH) == 0){
        //printf("- Node: %s\n", inet_ntop(AF_INET, &(ip4addr->sin_addr), char_buff, INET_ADDRSTRLEN));
        }else if(compare_addr(current_addr, new_addr)){
            current->timestamp = timestamp;
            current->temp_val = temperature;
            not_found = false;
        }
        current = current->next;
    }
    if(not_found){
        add_node_sens(list, temperature, timestamp, node_addr);
    }


    printf("[DEBUG] append_temp_db - print list\r\n");
    print_node_list(*list);

    return 0;
}

bool compare_addr(struct sockaddr_storage* current, struct sockaddr_storage* new){
    char *current_ip, *new_ip;
    if(current->ss_family == AF_INET){
        current_ip = (char*)malloc(sizeof(char)*INET_ADDRSTRLEN);
        inet_ntop(AF_INET, 
                &(((struct sockaddr_in*)current)->sin_addr), 
                current_ip, INET_ADDRSTRLEN);
    } else if(current->ss_family == AF_INET6){
        current_ip = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
        inet_ntop(AF_INET, 
                &(((struct sockaddr_in6*)current)->sin6_addr), 
                current_ip, INET6_ADDRSTRLEN);
    }
    if(new->ss_family == AF_INET){
        new_ip = (char*)malloc(sizeof(char)*INET_ADDRSTRLEN);
        inet_ntop(AF_INET, 
                &(((struct sockaddr_in*)new)->sin_addr), 
                new_ip, INET_ADDRSTRLEN);
    } else if(new->ss_family == AF_INET6){
        new_ip = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
        inet_ntop(AF_INET, 
                &(((struct sockaddr_in6*)new)->sin6_addr), 
                new_ip, INET6_ADDRSTRLEN);
    }
    if(strcmp(current_ip, new_ip) == 0){
        return true;
    } else {
        return false;
    }
}
bool value_expired(int32_t timestamp){
    if((time(NULL) - timestamp) > TEMP_EXPIRED_TIME){
        return true;
    } else {
        return false;
    }
}

int32_t average_temp(node_sens* list){
    int32_t count = 0;
    int32_t sum = 0;
    node_sens* current = list;

    //print_node_list(list);

    while(current != NULL){
        if(!value_expired(current->timestamp)){
            sum += current->temp_val;
            count++;
        }
        current = current->next;
    }
	if (count == 0) {
		printf("[DEBUG] DB Empty\r\n");
		return 0;
	}
	else {
		printf("[DEBUG] Returning temerature\r\n");
		return (int32_t)sum/count;
	}
}
