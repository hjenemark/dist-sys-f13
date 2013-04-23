/**
 * \file msg_list.c
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "msg_list.h"

char* serilization(node_msg* list){
    int32_t count = 0;
    node_msg* current = list;
    char* serial_result = NULL;
    int32_t marker = 0, t_count; 
    int32_t t_int_buff[4];
    char t_buff[4];
    int32_t temp_int;
    int8_t i;

    while(current != NULL){
        count += 2*4;
        count += current->op_size;
        //printf("New Count: %d\n", count);
        current = current->next;
    }

    serial_result = (char*)calloc(count + 1, sizeof(char));
    //printf("serial_result length: %d\n", strlen(serial_result));

    current = list;
    while(current != NULL){
        //t_count = snprintf((serial_result + marker), count, "%u%u%s", current->operation, current->op_size, current->operand);
        //marker += t_count;
        //printf("t_count: %d\n ser: %s\n",t_count, serial_result);
        t_int_buff[0] = current->operation;
        t_int_buff[1] = current->op_size;
        for(i = 0; i < 2; i++){
            t_count = 0;
            t_buff[0] = '0';
            t_buff[1] = '0';
            t_buff[2] = '0';
            t_buff[3] = '0';
            temp_int = t_int_buff[i];
            t_buff[t_count] = '0' + (temp_int % 10);
            while(temp_int > 9){
                t_count++;
                temp_int = temp_int / 10;
                t_buff[t_count] = '0' + (temp_int % 10);
            }
            t_count = 3;
            while(t_count >= 0){
                strncpy((serial_result + marker), &t_buff[t_count], 1);
                marker += 1;
                t_count--;
            }
        }

        strncpy((serial_result + marker), current->operand, current->op_size);
        marker += current->op_size;
        

        //printf("Serial result: %s\n", serial_result);
        //printf("serial_result length: %d\n", strlen(serial_result));

        current = current->next;
    }

    //printf("result length: %d\n", strlen(serial_result));
    //printf("result: %s\n", serial_result);

    return serial_result;
}

node_msg* deserialize(char* data){
    node_msg* new_list = NULL;
    uint32_t t_operation = 0, t_op_size = 0;
    char* t_operand;
    int32_t marker = 0, i, end;

    end = strlen(data);

    //printf("-- De-serializing --\n");
    
    while(marker < end){
        t_operation = 0;
        t_op_size = 0;
        for(i = 3; i >=0; i--){
            t_operation += (uint32_t)pow((double)10, (double) i) * (uint32_t)(data[marker] - '0');
            marker += 1;
        }
        for(i = 3; i >=0; i--){
            t_op_size += (uint32_t)pow((double)10, (double) i) * (uint32_t)(data[marker] - '0');
            marker += 1;
        }

        //t_operand = (char*)calloc(sizeof(char), t_op_size);
        t_operand = (char*)calloc(t_op_size+1, sizeof(char));
        strncpy(t_operand, &data[marker], t_op_size);
        marker += t_op_size;
    
        add_node_msg(&new_list, t_operation, t_op_size, t_operand);
        free(t_operand);
    }

    //printf("-- End of de-serializing --\n");
    return new_list;
}

uint32_t clear_list_msg(node_msg** list){
    while(*list != NULL){
        remove_node_msg(list, *list);
    }
    return 0;
}

uint32_t remove_node_msg(node_msg** list, node_msg* lnode){
    node_msg* current = NULL;

    if(*list == lnode){
        *list = lnode->next;
    } else {
        current = *list;
        while(current->next != lnode){
            current = current->next;
        }
        if(lnode->next == NULL){
            current->next = NULL;
        } else {
            current->next = current->next->next;
        }
    }
    
    free(lnode->operand);
    free(lnode);
    return 0;
}


uint32_t add_node_msg(node_msg** list, uint32_t operation, uint32_t op_size, char* operand){
    node_msg* newnode;
    newnode = (node_msg*)malloc(sizeof(node_msg));

    if(newnode == NULL){
        return -1;
    }

    newnode->operation = operation;
    newnode->operand = (char*)calloc(op_size+1 , sizeof(char));
    memcpy(newnode->operand, operand, op_size);
    newnode->op_size = op_size;

    newnode->next = *list;
    *list = newnode;
    //printf("[adding] Node added with operand: %s\n", newnode->operand);

    return 0;
}

void print_node_list_msg(node_msg* list){
    node_msg *current = list;
    printf("[Printing list]\n");
    if(list == NULL){
        printf("list empty\n");
    } else {
        while(current != NULL){
            printf("---\n");
            printf("- operation: %i\n", current->operation);
            printf("- operand: %s\n", current->operand);
            printf("- op_size: %i\n", current->op_size);
            printf("---\n");
    
            current = current->next;
        }
    }
    printf("[End of printing]\n");
}
