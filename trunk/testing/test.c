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

    node_sens* list = NULL;

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
    char* encodedstr = encode_message(REPORT_TEMPERATURE,8,"192.168.125.125" );

    printf("encoded msg: %s\n", encodedstr);
    list = (node_sens*)decode_message( encodedstr);
    free(encodedstr);

    clear_list_sens(&list);
    //print_node_list_temp(list);
    //deserialize(encodedstr);


    return 0;
}
