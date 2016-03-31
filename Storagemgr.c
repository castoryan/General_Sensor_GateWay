/********************************************************************************
 * File Name:	Storagemgr.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: .c file of Storage manager in charge of SQL connection
 *******************************************************************************/

#ifndef _SENSOR_DB_H_
#define _SENSOR_DB_H_


#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "db_config.h"
#include <string.h>


//#define DEBUG

#define DATA_FILE "sensor_data"

#define TABLE_NAME "YanQinrui"

#ifndef TABLE_NAME
#endif


void finish_with_error(MYSQL *con){
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

/*
 * initialize the connection to MYSQL DB_DATABASE;
 * para flag: for the choice of cleaning the existed table;
 * flag = 1: clear the existed table;
 * flag = 0: keep the data already in table;
 */
MYSQL *init_connection(char clear_up_flag){
  MYSQL *con = mysql_init(NULL); 
  
  if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASW, DB_DATABASE, 0, NULL, 0) == NULL) 
  {
    fprintf(stderr, "%s\n", mysql_error(con));
    printf("connected fail\n");
    mysql_close(con);
    return NULL;
  }
  else{
    printf("connected to the MYSQL server sucessfully\n");
  }
  
  if(clear_up_flag == 1){
    //Drop the old table
    if (mysql_query(con, "DROP TABLE IF EXISTS "TABLE_NAME)){//mysql_query return 0: sucess
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" dropped fail\n");
      #endif
    }
    else{
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" dropped sucessfully\n");
      #endif
    }
    
    //Create a new table
    if (mysql_query(con, "CREATE TABLE "TABLE_NAME"(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, sensor_id INT, sensor_value  DECIMAL(4,2), timestamp TIMESTAMP)")) {
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" created fail\n");
      finish_with_error(con);
      #endif
    }
    else{
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" created sucessfully\n");
      #endif
    }
  }
  else{//clear_up_flag == 0
    if (mysql_query(con, "CREATE TABLE "TABLE_NAME"(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, sensor_id INT, sensor_value  DECIMAL(4,2), timestamp TIMESTAMP)")) {
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" has existed\n");
      #endif
      
    }
    else{
      #ifdef DEBUG
      printf("TABLE "TABLE_NAME" created sucessfully\n");
      #endif
    }
  }
  
  return con;
}




void disconnect(MYSQL *conn){
  mysql_close(conn);
}

int insert_sensor(MYSQL *conn, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
  
  
  char id_s[5];
  char va_s[5];
  sprintf(id_s, "%d", id); 
  sprintf(va_s, "%f", value); 
  
  char str[80];
  strcpy(str,"INSERT INTO "TABLE_NAME"(sensor_id,sensor_value) ");
  strcat(str,"VALUES( ");
  strcat(str,id_s);
  strcat(str,",");
  strcat(str,va_s);
  strcat(str,")");
  #ifdef DEBUG
  printf("%s \n",str);
  #endif
  
  if (mysql_query(conn, str)){
    finish_with_error(conn);
    return 1;
  }
  return 0;
}

int insert_sensor_from_file(MYSQL *conn, FILE *sensor_data){
  
  
  uint16_t sensor_id;
  double temperature;
  time_t timestamp;
  
  
  while(feof(sensor_data) == 0){
    fread(&sensor_id,sizeof(sensor_id),1,sensor_data);
    
    fread(&temperature,sizeof(temperature),1,sensor_data);
    
    fread(&timestamp,sizeof(timestamp),1,sensor_data);
    #ifdef DEBUG
    printf(" %d", sensor_id);
    printf(" %f", temperature);
    printf(" %s", asctime(gmtime(&timestamp)));
    #endif
    
    
    char id_s[5];
    char va_s[5];
    sprintf(id_s, "%d", sensor_id); 
    sprintf(va_s, "%f", temperature); 
    
    char str[80];
    strcpy(str,"INSERT INTO "TABLE_NAME"(sensor_id,sensor_value) ");
    strcat(str,"VALUES( ");
    strcat(str,id_s);
    strcat(str,",");
    strcat(str,va_s);
    strcat(str,")");
    
    #ifdef DEBUG
    printf("%s \n",str);
    #endif
    
    
    if (mysql_query(conn, str)){
      finish_with_error(conn);
      return 1;
    }
  }
  return 0;
}


MYSQL_RES *find_sensor_all(MYSQL *conn){
  char query[512];
  sprintf(query,"SELECT * FROM YanQinrui");
  
  
  if (mysql_query(conn, query)) 
  {
    finish_with_error(conn);
  }
  
  
  MYSQL_RES *result = mysql_store_result(conn);
  
  if (result == NULL) 
  {
    finish_with_error(conn);
  }
  return result;
  //   int num_fields = mysql_num_fields(result);
  //   
  //   MYSQL_ROW row;
  //   int ii = 0;
  //   while ((row = mysql_fetch_row(result))) 
  //   { 
  //     for(ii = 0; ii < num_fields; ii++) 
  //     { 
  //       printf("%s ", row[ii] ? row[ii] : "NULL"); 
  //     } 
  //     printf("\n"); 
  //   }
  
}


MYSQL_RES *find_sensor_by_value(MYSQL *conn, sensor_value_t value_t){
  char query[100];
  char id_s[5];
  sprintf(id_s, "%f", value_t);
  
  strcpy(query,"SELECT * FROM "TABLE_NAME" WHERE sensor_value = ");
  strcat(query,id_s);
  
  if (mysql_query(conn, query)) {
    finish_with_error(conn);
  }
  
  MYSQL_RES *result = mysql_store_result(conn);
  
  if (result == NULL) {
    finish_with_error(conn);
  }
  return result;
  
}


MYSQL_RES *find_sensor_exceed_value(MYSQL *conn, sensor_value_t value_t){
  
  char query[100];
  char id_s[5];
  sprintf(id_s, "%f", value_t);
  
  
  strcpy(query,"SELECT * FROM "TABLE_NAME" WHERE sensor_value > ");
  strcat(query,id_s);
  
  if (mysql_query(conn, query)) 
  {
    finish_with_error(conn);
  }
  
  
  MYSQL_RES *result = mysql_store_result(conn);
  
  if (result == NULL) 
  {
    finish_with_error(conn);
  }
  return result;
}


MYSQL_RES *find_sensor_by_timestamp(MYSQL *conn, sensor_ts_t ts_t){
  
  char query[100];
  char time_s[40];
  struct tm tm1;
  tm1 = *localtime(&ts_t); 
  
  //printf("%s \n",tm1->tm_year);
  
  sprintf( time_s, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",  tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min,tm1.tm_sec);  
  //printf("%s \n",time_s);
  
  strcpy(query,"SELECT * FROM "TABLE_NAME" WHERE timestamp = '");
  strcat(query,time_s);
  strcat(query,"'");
  #ifdef DEBUG
  printf("%s \n",query);
  #endif
  
  if (mysql_query(conn, query)) 
  {
    finish_with_error(conn);
  }
  
  
  MYSQL_RES *result = mysql_store_result(conn);
  
  if (result == NULL) 
  {
    finish_with_error(conn);
  }
  return result;
  
}

/*
 * Write a SELECT query to return all sensor measurement recorded later than timestamp 'ts_t'
 * return MYSQL_RES with all the results
 */
MYSQL_RES *find_sensor_later_timestamp(MYSQL *conn, sensor_ts_t ts_t){
  char query[100];
  char time_s[40];
  struct tm tm1;
  tm1 = *localtime(&ts_t); 
  
  //printf("%s \n",tm1->tm_year);
  
  sprintf( time_s, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",  tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min,tm1.tm_sec);  
  //printf("%s \n",time_s);
  
  strcpy(query,"SELECT * FROM "TABLE_NAME" WHERE timestamp > '");
  strcat(query,time_s);
  strcat(query,"'");
  #ifdef DEBUG
  printf("%s \n",query);
  #endif
  
  if (mysql_query(conn, query)) 
  {
    finish_with_error(conn);
  }
  
  
  MYSQL_RES *result = mysql_store_result(conn);
  
  if (result == NULL) 
  {
    finish_with_error(conn);
  }
  return result;
}

/*
 * Return the number of records contained in the result
 */
int get_result_size(MYSQL_RES *result){
  int rows = mysql_num_rows(result);
  return rows;
  //   char query[512];
  //   sprintf(query,"SELECT count(*) FROM YanQinrui");
  //  
  //   
  //   if (mysql_query(con, query)) 
  //   {
  //     finish_with_error(con);
  //   }
  
  /*  
   *  result = mysql_store_result(con);
   *  
   *  if (result == NULL) 
   *  {
   *    finish_with_error(con);
}*/
  //return result -> row_count;
}

/*
 * Print all the records contained in the result
 */
void print_result(MYSQL_RES *result){
  
  int num_fields = mysql_num_fields(result);
  
  MYSQL_ROW row;
  int ii = 0;
  while ((row = mysql_fetch_row(result))) 
  { 
    for(ii = 0; ii < num_fields; ii++) 
    { 
      printf("%s ", row[ii] ? row[ii] : "NULL"); 
    } 
    printf("\n"); 
  }
  
}

#endif

