/********************************************************************************
 * File Name:	Datamgr.h
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: Header file of Data manager
 *******************************************************************************/

#include "mylist.h"
#include "myqueue.h"

typedef struct sensor sensor_t;
typedef sensor_t *sensor_ptr_t;


/*typedef struct sensor_data sensor_data_t;
typedef sensor_data_t *sensor_data_ptr_t;*/

list_ptr_t sensorlist_ini();

float queue_avg(queue_t *queue);

void dm_sensor_data_get(list_ptr_t sensor_list);

list_ptr_t dm_map_create(list_ptr_t sensor_list);

int find_room_by_sensor(list_ptr_t sensor_list,int sensor_id);