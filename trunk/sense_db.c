
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "sens_list.h"
#include "sense_db.h"

uint32_t append_temp_db(node_sens** list, char* node_id, time_t timestamp, int32_t sens_value){
    node_sens* current = *list;
    bool not_found = true;
    
    while(current != NULL){
        if(value_expired(current->timestamp)){
            remove_node_sens(list, current);
        }else if(strncmp(current->node_id, node_id, NODE_ID_LENGTH) == 0){
            current->timestamp = timestamp;
            current->temp_val = sens_value;
            not_found = false;
        }
        current = current->next;
    }
    if(not_found){
        add_node_sens(list, node_id, timestamp, sens_value);
    }

    return 0;
}

bool value_expired(time_t timestamp){
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
    return (int32_t)sum/count;
}
