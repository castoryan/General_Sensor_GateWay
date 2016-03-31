/******************************************************************************** 
 * File Name:	Logmgr.h
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-18
 * E-mail: 	castoryan1991@gmail.com
 * Description: Header file of Log manager
 *******************************************************************************/




void log_newconnection(int seq_num, time_t ts, FILE *fp_log, int room_id,int sensor_id);

void log_close_connection(int seq_num, time_t ts, FILE *fp_log,int room_id, int sensor_id);

void log_reportcold(int seq_num, time_t ts, FILE *fp_log, int room_id,int sensor_id);

void log_reporthot(int seq_num, time_t ts, FILE *fp_log,int room_id, int sensor_id);

void log_reportfull(int seq_num, time_t ts, FILE *fp_log);

void log_SQLconnect(int seq_num, time_t ts, FILE *fp_log);

void log_SQLlost(int seq_num, time_t ts, FILE *fp_log);

void log_SQLunconnect(int seq_num, time_t ts, FILE *fp_log);

void log_Process(double event_id, int seq_num, time_t ts, FILE *fp_log, int room_id, int sensor_id);