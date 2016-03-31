/********************************************************************************
 * File Name:	data_type.h
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: TODO:
 *******************************************************************************/

#include <inttypes.h>
#include <time.h>

// /*
//  * structure for data return from sensor 
//  */
// struct sensor_data_element{
//   int storage_flag;
//   int data_flag;
//   uint16_t sen_id;
//   double sen_tem;
//   time_t sen_time;
// };

typedef struct sensor_data_element sensor_data_element_t;
typedef sensor_data_element_t *sensor_data_element_ptr_t;
