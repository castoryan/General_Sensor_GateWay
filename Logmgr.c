/********************************************************************************
 * File Name:	Logmgr.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-19
 * E-mail: 	castoryan1991@gmail.com
 * Description: .c file of implementing log event processing
 *******************************************************************************/


/*======================================================================================
		Include files and Definitions (defines, typedefs, ...)
=======================================================================================*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define DEBUG



/*------------------------end of Include files and Definitions---------------------------*/




/*==========================================================================================
					Methods
==========================================================================================*/

/* Function: log_newconnection
 * ----------------------------
 *   Method of reporting connetion creating of sensor event
 * 
 *   Parameters:
 * 	1.seq_num: sequence number of the log event
 * 	2.ts: the happen time of event
 * 	3.fp_log: log file which the event is going to store
 * 	4.room_id: id of the room which event is happening
 * 	5.sensor_id: id of the sensor which event is happening
 *
 *   returns: NULL
 */
void log_newconnection(int seq_num, time_t ts, FILE *fp_log, int room_id,int sensor_id){
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  //sprintf(time, "%s", asctime(gmtime(&ts)));
  
  char roomid[5];			
  sprintf(roomid, "%d", room_id);
  
  char sensorid[5];			
  sprintf(sensorid, "%d", sensor_id);
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<room ",fp_log);
  fputs(roomid, fp_log);
  fputs(" with sensor ",fp_log);
  fputs(sensorid, fp_log);
  fputs(" connected>\n",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}

/* Function: log_close_connection
 * ----------------------------
 *   Method of reporting connetion losting of sensor event
 * 
 *   Parameters:
 * 	1.seq_num: sequence number of the log event
 * 	2.ts: the happen time of event
 * 	3.fp_log: log file which the event is going to store
 * 	4.room_id: id of the room which event is happening
 * 	5.sensor_id: id of the sensor which event is happening
 *
 *   returns: NULL
 */
void log_close_connection(int seq_num, time_t ts, FILE *fp_log,int room_id, int sensor_id){
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  //sprintf(time, "%s", asctime(gmtime(&ts)));
  
  char roomid[5];			
  sprintf(roomid, "%d", room_id);
  
  char sensorid[5];			
  sprintf(sensorid, "%d", sensor_id);
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<room ",fp_log);
  fputs(roomid, fp_log);
  fputs(" with sensor ",fp_log);
  fputs(sensorid, fp_log);
  fputs(" lost connection>\n",fp_log);
#ifdef DEBUG
  printf("event has been saved\n");
#endif
}

/* Function: log_reportcold
 * ----------------------------
 *   Method of reporting over low temperature event
 * 
 *   Parameters:
 * 	1.seq_num: sequence number of the log event
 * 	2.ts: the happen time of event
 * 	3.fp_log: log file which the event is going to store
 * 	4.room_id: id of the room which event is happening
 * 	5.sensor_id: id of the sensor which event is happening
 *
 *   returns: NULL
 */
void log_reportcold(int seq_num, time_t ts, FILE *fp_log, int room_id,int sensor_id){
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  //sprintf(time, "%s", asctime(gmtime(&ts)));
  
  char roomid[5];			
  sprintf(roomid, "%d", room_id);
  
  char sensorid[5];			
  sprintf(sensorid, "%d", sensor_id);
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<room ",fp_log);
  fputs(roomid, fp_log);
  fputs(" with sensor ",fp_log);
  fputs(sensorid, fp_log);
  fputs(" is too cold>\n",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}


/* Function: log_reporthot
 * ----------------------------
 *   Method of reporting over high temperature event
 * 
 *   Parameters:
 * 	1.seq_num: sequence number of the log event
 * 	2.ts: the happen time of event
 * 	3.fp_log: log file which the event is going to store
 * 	4.room_id: id of the room which event is happening
 * 	5.sensor_id: id of the sensor which event is happening
 *
 *   returns: NULL
 */
void log_reporthot(int seq_num, time_t ts, FILE *fp_log,int room_id, int sensor_id){
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  //sprintf(time, "%s", asctime(gmtime(&ts)));
  
  char roomid[5];			
  sprintf(roomid, "%d", room_id);
  
  char sensorid[5];			
  sprintf(sensorid, "%d", sensor_id);
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<room ",fp_log);
  fputs(roomid, fp_log);
  fputs(" with sensor ",fp_log);
  fputs(sensorid, fp_log);
  fputs(" is too hot>\n",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}

/* Function: log_reportfull
 * ----------------------------
 *   Method of reporting circular buffer full event
 * 
 *   Parameters:
 * 	1.event_id: the id of event
 * 	2.seq_num: sequence number of the log event
 * 	3.fp_log: log file which the event is going to store
 *
 *   returns: NULL
 */
void log_reportfull(int seq_num, time_t ts, FILE *fp_log){
  
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  
  
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<circular queue is full: start to overwrite data.>\n ",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}

/* Function: log_SQLconnect
 * ----------------------------
 *   Method of SQL connection connecting event
 * 
 *   Parameters:
 * 	1.event_id: the id of event
 * 	2.seq_num: sequence number of the log event
 * 	3.fp_log: log file which the event is going to store
 *
 *   returns: NULL
 */
void log_SQLconnect(int seq_num, time_t ts, FILE *fp_log){
  
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  
  
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<Connection to SQL server established.>\n",fp_log);
  
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
  
}

/* Function: log_SQLlost
 * ----------------------------
 *   Method of SQL connection lost event
 * 
 *   Parameters:
 * 	1.event_id: the id of event
 * 	2.seq_num: sequence number of the log event
 * 	3.fp_log: log file which the event is going to store
 *
 *   returns: NULL
 */
void log_SQLlost(int seq_num, time_t ts, FILE *fp_log){
  
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  
  
  
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<Connection to SQL server losted.>\n",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}

/* Function: log_SQLunconnect
 * ----------------------------
 *   Method of unable SQL connection event
 * 
 *   Parameters:
 * 	1.event_id: the id of event
 * 	2.seq_num: sequence number of the log event
 * 	3.fp_log: log file which the event is going to store
 *
 *   returns: NULL
 */
void log_SQLunconnect(int seq_num, time_t ts, FILE *fp_log){
  
  char sequence_number[5];			
  sprintf(sequence_number, "%d", seq_num);
  
  char time[26];
  struct tm* tm_info;
  tm_info = localtime(&ts);
  strftime(time, 26, "%Y:%m:%d %H:%M:%S", tm_info);
  
  fputs("<",fp_log);
  fputs(sequence_number, fp_log);
  fputs(">",fp_log);
  fputs("<time is: ",fp_log);
  fputs(time, fp_log);
  fputs(">",fp_log);
  fputs("<Unable to connect to SQL server.>\n",fp_log);
  #ifdef DEBUG
  printf("event has been saved\n");
  #endif
}

/* Function: log_Process
 * ----------------------------
 *   Log event processing method
 * 
 *   Parameters:
 * 	1.event_id: the id of event
 * 	2.seq_num: sequence number of the log event
 * 	3.ts: the happen time of event
 * 	4.fp_log: log file which the event is going to store
 * 	5.room_id: id of the room which event is happening
 * 	6.sensor_id: id of the sensor which event is happening
 *
 *   returns: NULL
 */
void log_Process(double event_id, int seq_num, time_t ts, FILE *fp_log, int room_id, int sensor_id){
  switch((int)(10*event_id)){
    case 11:
      log_newconnection(seq_num,ts,fp_log,room_id,sensor_id);
      break;
    case 12:
      log_close_connection(seq_num,ts,fp_log,room_id,sensor_id);
      break;
    case 21:
      log_reportcold(seq_num,ts,fp_log,room_id,sensor_id);
      break;
    case 22:
      log_reporthot(seq_num,ts,fp_log,room_id,sensor_id);
      break;
    case 23:
      log_reportfull(seq_num,ts,fp_log);
      break;
    case 31:
      log_SQLconnect(seq_num,ts,fp_log);
      break;
    case 32:
      log_SQLlost(seq_num,ts,fp_log);
      break;
    case 33:
      log_SQLunconnect(seq_num,ts,fp_log);
      break;
  }
  
}
