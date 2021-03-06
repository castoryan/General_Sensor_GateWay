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



// Ip address of the database
#define DB_HOST "127.0.0.1"
// username of your database
#define DB_USER "root"
// password of your database
#define DB_PASW "root"
// Name of the local database, if no database with this name exists, the program will crash, so please doulbe check if this database name have been created in your server.
#define DB_DATABASE "testdb"


#endif /* _CONFIG_H_ */

