
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
uint32_t append_temp_db(node_sens** list, int32_t temperature, int32_t timestamp, struct sockaddr *node_addr){
    node_sens* current = *list;
    bool not_found = true;
    struct sockaddr_in *current_addr, *new_addr;
    new_addr = (struct sockaddr_in*)node_addr;

    while(current != NULL){

        current_addr = (struct sockaddr_in*)(current->node_addr);
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

    return 0;
}

bool compare_addr(struct sockaddr_in* current, struct sockaddr_in* new){
    char* char_buff0 = (char*)malloc(sizeof(char)*(INET_ADDRSTRLEN + 1));
    char* char_buff1 = (char*)malloc(sizeof(char)*(INET_ADDRSTRLEN + 1));
    bool result = false;
    if(strncmp(inet_ntop(AF_INET, &(current->sin_addr), char_buff0, INET_ADDRSTRLEN),
                inet_ntop(AF_INET, &(new->sin_addr), char_buff1, INET_ADDRSTRLEN), INET_ADDRSTRLEN) == 0)
    {
        result = true;
    } else {
        result = false;
    }
    free(char_buff0);
    free(char_buff1);
    return result;

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
