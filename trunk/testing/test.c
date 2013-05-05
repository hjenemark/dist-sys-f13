/**
 * \file main.c
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "test.h"
#include "../msg_list.h"
#include "../sens_list.h"
#include "../common.h"

int main(int argc, char *argv[]){
    printf("Starting node...\n");
    printf("Adding nodes to list...\n");

    //node_sens* list = NULL;

    /*add_node_msg(&list,12, strlen("192.168.125.123"),"192.168.125.123");
    add_node_msg(&list,123, strlen("192.168.125.124"),"192.168.125.124");
    add_node_msg(&list,1234, strlen("192.168.125.125"),"192.168.125.125");

    print_node_list_msg(list);

    char* data = serilization(list);
    clear_list_msg(&list);
    print_node_list_msg(list);

    printf("Serialized data: %s\n", data);

    node_msg* new_list = deserialize(data);

    print_node_list_msg(new_list);
    clear_list_msg(&new_list);
    print_node_list_msg(new_list);*/


	int32_t buflen,timenow, realtime;
	char buffer[15];
	struct node_message *node_msg_v = NULL;
	
	buflen = snprintf(buffer, 15, "%d", 8);
	add_node_msg (&node_msg_v, REPORT_TEMPERATURE, buflen, buffer);

	timenow = time(0);
	//timeoffset = get_time_offset();
	realtime = timenow;// + timeoffset;
	printf("[Temp] Real time at admin node: %d\r\n", realtime);

	buflen = snprintf(buffer, 15, "%d", realtime);
	add_node_msg (&node_msg_v, TEMP_TIMESTAMP, buflen, buffer);

	char *msg=NULL;
	msg = serilization(node_msg_v);
	int32_t len = strlen(msg);
    //char* encodedstr = encode_message(REPORT_TEMPERATURE,8,"192.168.125.125" );

    printf("encoded msg: %s\nlength: %d\n", msg, len);
    //list = (node_sens*)decode_message( encodedstr);
    //free(encodedstr);

    node_msg* newlist = deserialize(msg);

    print_node_list_msg(newlist);
    clear_list_msg(&newlist);

    return 0;
}
