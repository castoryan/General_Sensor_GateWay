/********************************************************************************
 * File Name:	Main.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-24
 * E-mail: 	castoryan1991@gmail.com
 * Description: This is the main .c file of final project of 
 * 		System_Programming 2015 in KU_Leuven-GroupT, ICT branch.
 *******************************************************************************/

//#define DEBUG


/*===============================================================================
 *		Include files and Definitions (defines, typedefs, ...)
 * ================================================================================*/
#define _BSD_SOURCE // for usleep

#define SET_MAX_TEMP 20.1	//Up limit of temperature
#define SET_MIN_TEMP 19.9	//Down limit of temperature
#define PORT 1234
#define SET_TIMEOUT 3

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "data_type.h"


//for connection
#include <signal.h>
#include <poll.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "tcpsocket.h"


#include <unistd.h> 


//for data
#include "Datamgr.h"
#include "myqueue.h"
#include "mylist.h"



//for storage
#include <my_config.h>
#include <mysql.h>
#include "db_config.h"
#include "Storagemgr.h"

//for log
#include "Logmgr.h"




FILE *fp_log;			//log file to save log events

//for circular buffer
queue_t* buffer_queue;
//for log buffer
queue_t* log_queue;


//threads of this program
pthread_t connection;
pthread_t data;
pthread_t storage;
pthread_t log_process;
pthread_t main_process;

pthread_mutex_t mut_log;		//lock of threads
pthread_mutex_t mut;		//lock of threads
pthread_cond_t cond_log;		//condition variable
pthread_cond_t cond;		//condition variable

static list_ptr_t mysensor_list;


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


int Run_control = 1;		//Control flag of the whole system


/*------------------------end of Include files and Definitions---------------------------*/





/*==========================================================================================
 *					Methods
 * ==========================================================================================*/


/* Function: runHandler
 * ----------------------------
 *   Capture the Ctrl+C signal, and stop running of the system
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void runHandler(int dummy) {
  //signal(SIGINT, runHandler);
  pthread_cond_broadcast(&cond);	//release condition variable
  pthread_cond_signal(&cond_log);
  Run_control = 0;
}//end of runHandler




/* Function: log_event
 * -----------------------------------
 *   put events into log_queue
 * 
 *   Parameters:
 * 	event:identity number of the upcoming event
 * 		1.1: a new connection of <sensor_id> incoming
 * 		1.2: the connection of <sensor_id> was closed
 * 		2.1: the <room_id> of <sensor_id> is too cold
 * 		2.2: the <room_id> of <sensor_id> is too hot
 * 		2.3: circular queue is full, start overwriting
 * 		3.1: connection to SQL server established
 * 		3.2: connection to SQL server lost
 * 		3.3: unable to connect SQL server
 * 	sensor_id:the the sensor which this event occur
 * 	timestamp:happen time of this event
 *
 *   returns: NULL
 */
void log_event(double event,int sensor_id,time_t timestamp){
  
  
  sensor_data_element_ptr_t log_data = malloc(sizeof(sensor_data_element_t));
  log_data -> storage_flag = 0;
  log_data -> data_flag = 0;
  log_data -> sen_id = sensor_id;
  log_data -> sen_tem = event;	// the identity number of event
  log_data -> sen_time = timestamp;
  
  #ifdef DEBUG
  printf("event happens, event id is %f\n",event);
  printf("addr of logdata is %p\n",log_data);
  #endif
  
  pthread_mutex_trylock(&mut_log); 
  queue_enqueue(log_queue,log_data);
  pthread_mutex_unlock(&mut_log);
  
  pthread_cond_signal(&cond_log);
}//end of log_event


/* Function: thread_connection
 * ----------------------------
 *   Thread method for sensor connection
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void *thread_connection(){
  //printf("thread_connection : I'm thread_connection\n");
  
  
  Socket server, client;
  uint16_t sensor_id;
  double temperature;
  time_t timestamp = 0;
  int bytes;
  int amount_of_clients = 0;
  int num = 0;
  
  struct pollfd * fds = malloc(sizeof(struct pollfd)*amount_of_clients);
  Socket* socket_list = malloc(sizeof(Socket)*amount_of_clients);
  time_t* lasttimestamp = malloc(sizeof(time_t));
  
  
  lasttimestamp[0] = 0;
  // open server socket
  server = tcp_passive_open( PORT );
  
  socket_list[0] = server;
  fds[0].fd = get_socket_descriptor(server);	//
  fds[0].events = POLLIN;			//the event to be listened
  
  printf("\n");
  
  while(Run_control){
    
    int poll_result = poll(fds,1+amount_of_clients,500);
    //if poll_result > 0, there are the number of poll_result fds changed state;
    //if poll_result == 0, nothing changed
    //if poll_result < 0 , some errors occur.
    
    
    
    
    if(poll_result > 0){
      if(fds[0].revents == POLLIN){
	amount_of_clients ++;
	client = tcp_wait_for_connection(server);
	//realloc fds
	void* fds_tmp = realloc(fds,sizeof(struct pollfd)*(amount_of_clients));
	if(fds_tmp == NULL){
	  printf("realloc fds failed");
	}
	else{
	  fds = fds_tmp;
	}
	
	//realloc socket_list
	void* socket_list_tmp = realloc(socket_list,sizeof(Socket)*(amount_of_clients));
	if(socket_list_tmp == NULL){
	  printf("realloc socket_list failed");
	}
	else{
	  socket_list = socket_list_tmp;
	}
	
	
	//realloc lasttimestamp
	void* lasttimestamp_tmp = realloc(lasttimestamp,sizeof(time_t)*(amount_of_clients));
	if(lasttimestamp_tmp == NULL){
	  printf("realloc lasttimestamp failed");
	}
	else{
	  lasttimestamp = lasttimestamp_tmp;
	}
	
	bytes = tcp_receive( client, (void *)&sensor_id, sizeof(sensor_id));
	assert( (bytes == sizeof(sensor_id)) || (bytes == 0) );
	bytes = tcp_receive( client, (void *)&temperature, sizeof(temperature));
	assert( (bytes == sizeof(temperature)) || (bytes == 0) );
	bytes = tcp_receive( client, (void *)&timestamp, sizeof(timestamp));
	assert( (bytes == sizeof(timestamp)) || (bytes == 0) );
	log_event(1.1,sensor_id,timestamp);//incoming connection
	
	socket_list[amount_of_clients] = client;
	fds[amount_of_clients].fd = get_socket_descriptor(client);
	fds[amount_of_clients].events = POLLIN;
      }
      
      // clients action
      for(int i = 1; i <= amount_of_clients; i++){
	if(fds[i].revents == POLLIN){
	  bytes = tcp_receive( socket_list[i], (void *)&sensor_id, sizeof(sensor_id));
	  // bytes == 0 indicates tcp connection teardown
	  assert( (bytes == sizeof(sensor_id)) || (bytes == 0) );	
	  bytes = tcp_receive( socket_list[i], (void *)&temperature, sizeof(temperature));
	  assert( (bytes == sizeof(temperature)) || (bytes == 0) );
	  bytes = tcp_receive( socket_list[i], (void *)&timestamp, sizeof(timestamp));
	  assert( (bytes == sizeof(timestamp)) || (bytes == 0) );
	  
	  //the lasttimestamp value of first time 
	  if(lasttimestamp[i] < 100){
	    lasttimestamp[i] = timestamp;
	  }

	  int pasttime = timestamp - lasttimestamp[i];
	  lasttimestamp[i] = timestamp; 

	  //when a sensor_node does not response in a certain time,  server would terminate it.
	  if(pasttime > SET_TIMEOUT){
	    fds[i].events = 0;
	    fds[i].fd = 0;
	    fds[i].revents = 0;
	    amount_of_clients--;
	    //printf("outgoing client connection of sensor %d\n",sensor_id);
	    printf("the %d connection is timeout \n",sensor_id);
	    lasttimestamp[i] = 0;
	    tcp_close(&socket_list[i]);
	  }
	  
	  //when a sensor_node quit, it sends sensor_id = 0, so that server could terminate it.
	  if(temperature == 100.001){
	    fds[i].events = 0;
	    fds[i].fd = 0;
	    fds[i].revents = 0;
	    amount_of_clients--;
	    lasttimestamp[i] = 1;
	    printf("the sensor %d is out of connect \n",sensor_id);
	    tcp_close(&socket_list[i]);

	    log_event(1.2,sensor_id,timestamp);//connection out
	    
	  }
	  else{
	    if (bytes){
	      num++;
	      if(queue_isfull(buffer_queue) == 1){
		log_event(2.3,sensor_id,timestamp);//queue full
	      }
	      
	      sensor_data_element_ptr_t temp_data = malloc(sizeof(sensor_data_element_t));
	      temp_data -> storage_flag = 0;
	      temp_data -> data_flag = 0;
	      temp_data -> sen_id = sensor_id;
	      temp_data -> sen_tem = temperature;
	      temp_data -> sen_time = timestamp;
	      
	      pthread_mutex_lock(&mut); 			    //lock other threads
	      queue_enqueue(buffer_queue,temp_data);
	      
	      printf("\n");
	      printf("%d sensor id = %" PRIu16 " - temperature = %4.2f - timestamp = %s",num ,sensor_id, temperature, asctime(gmtime(&timestamp)));
	      
	      pthread_cond_broadcast(&cond);	//release condition variable
	      pthread_mutex_unlock(&mut);
	    }
	  }
	  
	}
      }
      
    }
    
   
  }
  
  for(int i = 0; i <= amount_of_clients; i++){
    //free(&socket_list[i]);
    //tcp_close(&socket_list[i]);
    //free(&fds[i]);
  }
  
  //freedom the malloced memory  
  free(fds);
  free(socket_list);
  free(lasttimestamp);
  
  usleep(10000);	/* to allow socket to drain */
  if(client != NULL){
    tcp_close( &client );
  }
  tcp_close( &server );


#ifdef DEBUG
	printf("thread_connection() has quited\n");
#endif
  pthread_exit(NULL);
}//end of thread_connection()


/* Function: thread_data
 * ----------------------------
 *   Thread method for data managing
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void *thread_data(){

  int room_id;
  
  queue_t* sensor_id_qu[list_size(mysensor_list)+1]; // Array of queues.
  for(int j=0; j<list_size(mysensor_list)+1; j++){
    sensor_id_qu[j] = queue_create();
  }
  
 while(Run_control){ 
   pthread_mutex_lock(&mut);
   pthread_cond_wait(&cond,&mut);	//Waiting for condition variable
    
    if(queue_size(buffer_queue)!=0){
      sensor_data_element_ptr_t* data_ele_ptr = queue_top(buffer_queue);
      sensor_data_element_ptr_t data_ele =*data_ele_ptr;
      
      if(data_ele->data_flag == 0){//the top element wasn't used
	queue_enqueue(sensor_id_qu[data_ele->sen_id],data_ele);
	float run_avg = queue_avg(sensor_id_qu[data_ele->sen_id]);
#ifdef DEBUG
	printf("the running avg of sensor %d is %f\n", data_ele->sen_id, run_avg);
#endif	
	room_id = find_room_by_sensor(mysensor_list,data_ele->sen_id);
	
	if(run_avg > SET_MAX_TEMP){
	  fprintf(stderr, "room %d is too hot\n", room_id);  
	  log_event(2.1,data_ele->sen_id,data_ele->sen_time);//too hot
	}
	if(run_avg < SET_MIN_TEMP){
	  fprintf(stderr, "room %d is too cold\n", room_id);
	  log_event(2.2,data_ele->sen_id,data_ele->sen_time);//too cold
	}
      }

      data_ele->data_flag = 1;//set data flag of this element; 
    }
    else{
      //printf("buffer is empty!!!!!!\n");
    }
   pthread_mutex_unlock(&mut);
 }
  
  
  for(int j=0; j<list_size(mysensor_list)+1; j++) {
    queue_free(&sensor_id_qu[j]);
  }
  
#ifdef DEBUG
	printf("thread_data() has quited\n");
#endif
  pthread_exit(NULL);
}//end of thread_data()


/* Function: thread_storage
 * ----------------------------
 *   Thread method for information storage
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void *thread_storage(){
  //printf("thread_storage : I'm thread_storage\n");
  time_t ts;
  time (&ts);
  mysql_library_init(0, NULL, NULL);
  MYSQL *con;
  con = init_connection(1);//1 means drop the existed table, 0 means use existed table;
  
  //Checking if connection success
  int connect_info = mysql_ping(con);//0 means connection well, 1 means errors occur
  if(connect_info != 0){
    log_event(3.3,0,ts);//Connect to SQL fail
  }
  else{
    log_event(3.1,0,ts);//Connection success
  }
  
  
  while(Run_control){
    
    pthread_mutex_lock(&mut);
    pthread_cond_wait(&cond,&mut);	//Waiting for condition variable
    
    //Checking the status of connection
    if(mysql_ping(con) != 0){
      log_event(3.2,0,ts);//Connection lost
    }
    
    if(queue_size(buffer_queue)!=0){
      sensor_data_element_ptr_t* data_ele_ptr = queue_top(buffer_queue);
      sensor_data_element_ptr_t data_ele =*data_ele_ptr;
      
      if(data_ele->storage_flag == 0){//the top element wasn't used
	insert_sensor(con,data_ele->sen_id,data_ele->sen_tem,time(&data_ele->sen_time));
      }
      
      data_ele->storage_flag = 1;//set data flag of this element;
      
    }
    
    pthread_mutex_unlock(&mut);
  }
  disconnect(con);
  time(&ts);
  //log_event(3.2,0,ts);//Connection lost
  mysql_library_end();
#ifdef DEBUG
	printf("thread_storage() has quited\n");
#endif
  pthread_exit(NULL);
}//end of thread_storage()


/* Function: thread_log
 * ----------------------------
 *   Thread method for log processing
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void *thread_log(){
  //Get current time
  time_t nowtime;
  time(&nowtime);
  char times[26];
  struct tm* tm_info;
  tm_info = localtime(&nowtime);
  strftime(times, 26, "%Y_%m_%d_%H_%M_%S", tm_info);
  
  char logfilename[80];
  strcpy(logfilename,"./log_folder/gateway");
  strcat(logfilename,times);
  strcat(logfilename,".log");
  
  fp_log = fopen(logfilename,"w");
  int seq_number = 0;
  time_t lasttime = 0;
  double last_event =0.0;
  
  
  
  while(Run_control){
    pthread_mutex_lock(&mut_log);
    pthread_cond_wait(&cond_log,&mut_log);	//Waiting for condition variable
    
    //printf("size of log queue is %d\n",queue_size(log_queue));
    if(queue_size(log_queue) != 0){
      element_t* ele_top = queue_top(log_queue);
      if((((*ele_top) -> sen_time) != lasttime)||((*ele_top) -> sen_tem != last_event)){
#ifdef DEBUG
	printf("addr of eletop is %p\n",*ele_top);
#endif
	double event_id = (*ele_top) -> sen_tem;
	int seq_num = seq_number;
	time_t ts = (*ele_top) -> sen_time;
	int sensor_id = (*ele_top) -> sen_id;
	int room_id = find_room_by_sensor(mysensor_list,sensor_id);
	
	log_Process(event_id, seq_num, ts, fp_log, room_id, sensor_id);
	
	last_event = (*ele_top) -> sen_tem;
	lasttime = (*ele_top) -> sen_time;
	free(*ele_top);
	queue_dequeue(log_queue);
	seq_number++;
      }
    }
      //usleep(100000);
    pthread_mutex_unlock(&mut_log);
  }
  usleep(10000);
  queue_dequeue(log_queue);
  while(queue_size(log_queue) != 0){
    
    element_t* ele_top = queue_top(log_queue);
    double event_id = (*ele_top) -> sen_tem;
    int seq_num = seq_number;
    time_t ts = (*ele_top) -> sen_time;
    int sensor_id = (*ele_top) -> sen_id;
    int room_id =0;// find_room_by_sensor(mysensor_list,sensor_id);
    log_Process(event_id, seq_num, ts, fp_log, room_id, sensor_id);
    queue_dequeue(log_queue);
    seq_number++;
  }
  time_t ts2;
  time(&ts2);
  log_Process(3.2, seq_number, ts2, fp_log, 1, 1);
  fclose(fp_log);
#ifdef DEBUG
	printf("thread_log() has quited\n");
#endif
  pthread_exit(NULL);
}


/* Function: thread_main
 * ----------------------------
 *   Thread method for main queues processing
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
void *thread_main(){
  //Main thread, handle the buffer_queue
  while(Run_control){

    
    if(queue_size(buffer_queue)!=0){
      sensor_data_element_ptr_t* data_ele_ptr = queue_top(buffer_queue);
      sensor_data_element_t data_ele =**data_ele_ptr;
      //check if the top of queue has been read and saved
      if((data_ele.storage_flag == 1) & (data_ele.storage_flag == 1)){
	    
	//pthread_mutex_lock(&mut);
	//pthread_cond_wait(&cond,&mut);	//Waiting for condition variable
	queue_dequeue(buffer_queue);
	//pthread_mutex_unlock(&mut);
	
      }
    }
    
    usleep(50000);
  }
#ifdef DEBUG
	printf("thread_main() has quited\n");
#endif
  pthread_exit(NULL);
}//end of thread_main()


/* Function: qu_element_print
 * ----------------------------
 *   for printing out elements in queues
 * 
 *   Parameters:
 *   	1.qu_element:the element one is going to print which is in queue
 *
 *   returns: NULL
 */
void qu_element_print(void* qu_element){
  sensor_data_element_t* data_ele = qu_element; 
  printf(" storage flag is %d, data flag is %d %d, %f ,%s ",data_ele->storage_flag,
	 data_ele->data_flag,
	 data_ele->sen_id,
	 data_ele->sen_tem,
	 asctime(gmtime(&(data_ele->sen_time))));
}//end of qu_element_print()



/*--------------------------------end of Methods--------------------------------------*/




/*=====================================================================================
 *					Main
 * =======================================================================================*/

/* Function: main
 * ----------------------------
 *   Main Function of the whole project
 * 
 *   Parameters:NULL
 *
 *   returns: NULL
 */
int main(){
  printf("=========================================================\n");
  printf("================Sensor Gateway, version 2.0==============\n");
  printf("====================Author:Yan Qinrui====================\n");
  printf("=========================================================\n");
  printf("\n");
  printf("\n");
  printf("\n");
  
  
  //allocate memory for buffer
  buffer_queue = queue_create();
  log_queue = queue_create();
  #ifdef DEBUG
  printf("buffer_queue is on address %p\n",buffer_queue);
  printf("log_queue is on address %p\n",log_queue);
  #endif
  
  printf("\n");
  printf("\n");
  printf("Waiting for sensor connection.......\n");
  
  //initialize the sensor->room list
  
  mysensor_list = sensorlist_ini();
  mysensor_list = dm_map_create(mysensor_list);
  
  //initialize thread lock
  pthread_mutex_init(&mut,NULL);
  pthread_mutex_init(&mut_log,NULL);
  //initialize condition variable
  pthread_cond_init(&cond,NULL);
  pthread_cond_init(&cond_log,NULL);
  
  
  //initialize three main threads
  pthread_create(&main_process, NULL, thread_main, NULL);
  pthread_create(&connection, NULL, thread_connection, NULL);
  pthread_create(&data, NULL, thread_data, NULL);
  pthread_create(&storage, NULL, thread_storage, NULL);
  pthread_create(&log_process, NULL, thread_log, NULL);
  
  signal(SIGINT, runHandler);
  
  //waiting for the terminations of threads
  pthread_join(main_process,NULL);
  pthread_join(connection,NULL);
  pthread_join(data,NULL);
  pthread_join(storage,NULL);
  pthread_join(log_process,NULL);
  //thread_wait();
  
  
  //release memory of buffer
  queue_free(&buffer_queue);
  queue_free(&log_queue);
  
  my_list_free(&mysensor_list);
  
  pthread_mutex_destroy(&mut);
  pthread_mutex_destroy(&mut_log);
  pthread_cond_destroy(&cond);
  pthread_cond_destroy(&cond_log);
  
  
  printf("=========================================================\n");
  printf("================Program Exited Successfully==============\n");
  printf("=========================================================\n");
  exit(0);
  return 0;
}//end of main()


/*--------------------------------end of Main--------------------------------------*/



