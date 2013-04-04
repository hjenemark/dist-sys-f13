
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sens_list.h"
#include "sense_db.h"

int append_temp_db(node_sens** list, char* node_id, time_t timestamp, int sens_value){
    node_sens* current = NULL;

    
    while(current != NULL){
        if(value_expired(current->timestamp)){
        //if((time(NULL) - current->timestamp) > TEMP_EXPIRED_TIME){
            remove_node_sens(list, current);
        }else{
        }
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
