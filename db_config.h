/********************************************************************************
 * File Name:	db_config.h
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: configurtion file of Mysql database connection and some basic structure
  *******************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_


#include <stdint.h>
#include <time.h>

/*===============================================================================
		definitions (defines, typedefs, ...)
================================================================================*/

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;    
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time()

typedef struct{
	sensor_id_t id;
	sensor_value_t value;
	sensor_ts_t ts;
}sensor_data_t, * sensor_data_ptr_t;
			
#define DB_HOST "studev.groept.be"
#define DB_USER "a13_syssoft"
#define DB_PASW "a13_syssoft"
#define DB_DATABASE "a13_syssoft"

// #define DB_HOST "127.0.0.1"
// #define DB_USER "root"
// #define DB_PASW "root"
// #define DB_DATABASE "testdb"


#endif /* _CONFIG_H_ */

