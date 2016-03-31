/********************************************************************************
 * File Name:	Datamgr.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: .c file of Data manager
 *******************************************************************************/



/*===============================================================================
		Include files and Definitions (defines, typedefs, ...)
================================================================================*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "mylist.h"
#include "myqueue.h"
#include "data_type.h"

#define DEBUG
#define MAP_FILE "room_sensor.map"

int last_avg = 0;
int i;

FILE *fp_text, *fp_bin, *fp_log;


/*
 * structure for data return from sensor 
 */
struct sensor_data_element{
  int storage_flag;
  int data_flag;
  uint16_t sen_id;
  double sen_tem;
  time_t sen_time;
};



struct data_element{
  uint16_t ele_sensor_id;
  uint16_t ele_room_id;
  double run_avg;
  time_t last_mod;
};

typedef struct data_element data_element_t;
typedef data_element_t *data_element_ptr_t;

struct queue {
  element_t *arr; // dynamic array containing data elements
  int current_size; // Counts number of elements in the queue
  int size;	// the size of this queue
  int front, rear;
  // Remark for later: extra fields need to be added here to make it a thread-safe queue as needed for the assigment 
};

struct list_node{
  list_node_t *next;
  list_node_t *prev;
  void *element;
};

typedef struct queue queue_t;

/*------------------------end of Include files and Definitions---------------------------*/




/*==========================================================================================
					Methods
==========================================================================================*/

/*
 * Function: sensorlist_ini
 * ----------------------------
 *   Create a new linklist for later using
 * 
 *   Parameters:
 *
 *   returns: a new empty linklist
 */
list_ptr_t sensorlist_ini(){
  
  list_ptr_t sensor_list = NULL;
  sensor_list = list_create(NULL,NULL,NULL,NULL);
  return sensor_list;
}//end of sensorlist_ini()


/*
 * Function: dm_map_create
 * ----------------------------
 *   Create a linklist to store sensors' id and their corresponding room id one by one 
 * 
 *   Parameters:
 *   1.sensor_list: a new linklist was created before 
 *
 *   returns: the linklist contains all information of working sensors and their corresponding room ids
 */
list_ptr_t dm_map_create(list_ptr_t sensor_list){
  fp_text = fopen(MAP_FILE, "r");
  char buffer[8];
  //int count = 0;
  while(fgets( buffer, 8, fp_text ) != NULL){
    data_element_ptr_t new_ele = malloc(sizeof(data_element_t));
    //data_element_ptr_t new_ele = (data_element_ptr_t)addr[i]; 
    
    if(buffer[1]==32){		//if the second digit of buffer is ".", x.x or x.xx
      new_ele -> ele_room_id = buffer[0]-48;
      if(buffer[3] == 10){
	new_ele -> ele_sensor_id = buffer[2]-48;
      }
      if(buffer[4] == 10){
	new_ele -> ele_sensor_id = (buffer[2]-48)*10+(buffer[3]-48);
      }
    }
    if(buffer[2]==32){
      new_ele -> ele_room_id = 10*(buffer[0]-48)+(buffer[1]-48);
      if(buffer[4] == 10){
	new_ele -> ele_sensor_id = buffer[3]-48;
      }
      if(buffer[5] == 10){
	new_ele -> ele_sensor_id = (buffer[3]-48)*10+buffer[4];
      }							//ASCII 32:space
    }
    //list_ele -> element = new_ele;
    //printf( "the new ele is %p \n", new_ele); 
    list_insert_at_index(sensor_list,new_ele,10000);
    //list_print(sensor_list);
    //printf("room id is %d, sensor id is %d \n",new_ele -> ele_room_id, new_ele -> ele_sensor_id);
    //printf( "%s", buffer);
    //free(new_ele);
    i++;
  }
  
  fclose(fp_text);
  
  return sensor_list;
}//end of dm_map_create()


/*
 * Function: queue_avg
 * ----------------------------
 *   Returns the average value of a whole queue of sensor_data_element 
 * 
 *   Parameters:
 *   1.queue: the queue we are trying to calculate its average temperature
 *
 *   returns: the average temperature in the whole queue
 */
float queue_avg(queue_t *queue){
  float avg = 0.0;
  float avg_sum = 0.0;
  queue_t* this_queue =(queue_t*) queue;
  element_t* a  =  this_queue -> arr;
  element_t ele;
  int j;
  int i1=this_queue->rear;
  int i2=this_queue->front;
  
  
  if(queue_size(queue) ==5){
    for(j=0;j<this_queue->size;j++){
      ele = *(a+j);
      avg_sum = avg_sum+(ele->sen_tem);
    }
    
    avg = avg_sum/queue_size(queue);
    //printf("the avg sum is %f, the size is %d, avg is %f\n",avg_sum,queue_size(queue),avg);
  }
  else{
    if(i1<i2){//normal:front > rear
      for(j=i1;j<=i2;j++){
	ele = *(a+j);
	avg_sum = avg_sum+(ele->sen_tem);
      }
      avg = avg_sum/queue_size(queue);
      //printf("the avg sum is %f, the size is %d, avg is %f\n",avg_sum,queue_size(queue),avg);
    }
    if(i1>i2){//front < rear
      for(j=i1;j < this_queue->size;j++){
	ele = *(a+j);
	avg_sum = avg_sum+(ele->sen_tem);
      }
      for(j=0;j <= i2;j++){
	ele = *(a+j);
	avg_sum = avg_sum+(ele->sen_tem);
      }
      avg = avg_sum/queue_size(queue);
      //printf("the avg sum is %f, the size is %d, avg is %f\n",avg_sum,queue_size(queue),avg);
    }
    
    if((i1==i2)&(i1!=-1)&(i2!=-1)){
      ele = *(a+i1);
      avg = (ele->sen_tem);
    }
    
    if((i1==-1)&(i2==-1)){
      avg = 0 ;
    }
  }
  return avg;
}//end of queue_avg()



/*
 * Function: find_room_by_sensor
 * ----------------------------
 *   Returns the room_id corresponding to the input sensor_id
 *
 *   Parameters:
 *   1.sensor_list: the linklist contains all sensor_id and their corresponding room_id
 *   2.sensor_id: the sensor we are trying to search room id
 *
 *   returns: the room_id corresponding to the input sensor_id
 */
int find_room_by_sensor(list_ptr_t sensor_list,int sensor_id){
  list_node_ptr_t current_ptr = sensor_list -> head;
  int room_id;
  while(current_ptr -> next != NULL){
    data_element_ptr_t da_ele = current_ptr -> element;
    if((da_ele -> ele_sensor_id) == sensor_id){
      //printf("the room %d has sensor %d \n",da_ele->ele_room_id,da_ele ->ele_sensor_id);
      room_id = da_ele->ele_room_id;
      break;
    }
    current_ptr = current_ptr -> next;
  }
  if((current_ptr -> next == NULL)&(current_ptr -> prev != NULL)){
    data_element_ptr_t da_ele = current_ptr -> element;
    room_id = da_ele->ele_room_id;
    //printf("the room %d has sensor %d \n",da_ele->ele_room_id,da_ele ->ele_sensor_id);
  }
  return room_id;
}//end of find_room_by_sensor()



/*
 * Function: element_print
 * ----------------------------
 *   For printing out the list element
 *
 *   Parameters:
 *   1.data_element: data element one is going to print out
 *   
 *
 *   returns: 
 */
void element_print(void* data_element){
  data_element_t* ele_p = data_element;
  printf("room id: %d, sensor id: %d, running average: %f, last modify: %lld  \n ",ele_p -> ele_room_id,ele_p -> ele_sensor_id,ele_p -> run_avg,(long long)ele_p -> last_mod);
}//end of element_print()

/*------------------------end of Methods---------------------------*/
