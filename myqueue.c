/********************************************************************************
 * File Name:	myqueue.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: .c file of implementing data structure of circular queue
 *******************************************************************************/


/*======================================================================================
		Include files and Definitions (defines, typedefs, ...)
=======================================================================================*/
#include <stdio.h>
#include "myqueue.h"
#include <stdlib.h>

//#define DEBUG_PRINTF /* define here a debug macro */

/* The caller of the queue is responsible for implementing the functions below */
extern void qu_element_print(void* qu_element);
extern void element_copy(element_t *dest_element, element_t src_element);
extern void element_free(element_t *element);

/*
 * The real definition of 'struct queue'
 */
struct queue {
  element_t *arr; // dynamic array containing data elements
  int current_size; // Counts number of elements in the queue
  int size;	// the size of this queue
  int front, rear;
  int using_flag;
};

/*------------------------end of Include files and Definitions---------------------------*/



/*==========================================================================================
					Methods
==========================================================================================*/

/*
 * Function: queue_create
 * ----------------------------
 *   For creating a new and empty queue
 *
 *   Parameters:
 *
 *   returns: a pointer of the new queue
 */
queue_t* queue_create(){
  queue_t* q = malloc(QUEUE_SIZE*sizeof(queue_t));
  if(q == NULL){
    printf("STOP THE PROGRAM \n");
  }
  q -> arr = malloc(QUEUE_SIZE*sizeof(queue_t));	//queue need memory
  q -> current_size = 0;
  q -> size  = QUEUE_SIZE;
  q -> front = 0;
  q -> rear  = 0;
  #ifdef DEBUG_PRINTF
  printf("\n");
  printf("queue creted sucessfully \n");
  printf("the adress of q -> arr is %p \n",q->arr);
  printf("the size of q -> size is %d \n",q->size);
  #else
  
  #endif
  
  return q;
}//end of queue_create()


/*
 * Function: queue_free
 * ----------------------------
 *   For freeing the memory of a existed queue
 *
 *   Parameters:
 *
 *   returns: 
 */
void queue_free(queue_t** queue){
  queue_t** this_queue = (queue_t**) queue;
  free((*this_queue) -> arr);
  (*this_queue) -> arr = NULL;
  free(*(this_queue));
  *(this_queue) = NULL;
  #ifdef DEBUG_PRINTF
  printf("\n");
  printf("queue freed sucessfully \n");
  #else
  
  #endif
}//end of queue_free()


/*
 * Function: queue_enqueue
 * ----------------------------
 *   Inserting a new element into existed queue
 *
 *   Parameters: 
 * ` 	1.queue: the queue which one is going to insert the new element in
 *   	2.element:the element which one is going to insert
 *
 *   returns: 
 */
void queue_enqueue(queue_t* queue, element_t element){
  
  //TODO:First need to check if the queue is being used;

  queue_t* this_queue = (queue_t*) queue;
  int i;
  i = (this_queue -> front+1)%(this_queue -> size);
  
  if(this_queue->current_size == 0){
    this_queue -> rear = 0;
    this_queue -> front = 0;
    this_queue -> current_size = 1;
    i = 0;
  }
  else{
    if(this_queue -> current_size < this_queue -> size){//queue is not full
      this_queue -> current_size++;
      this_queue -> front = (this_queue -> front+1)%(this_queue -> size);
    }
    else{//queue is full
      this_queue -> front = (this_queue -> front+1)%(this_queue -> size);
      this_queue -> rear = (this_queue -> front+1)%(this_queue -> size);
    }
  }
  this_queue -> arr[i]=element;
  #ifdef DEBUG_PRINTF
  printf("\n");
  printf("element inserted sucessfully \n");
  printf("the i is %d \n",i);
  printf("the inserted adress of element is %p \n",this_queue -> arr+i);
  //printf("the new element is %d \n",element);
  printf("the rear of queue is %d \n",this_queue->rear);
  printf("the front of queue is %d \n",this_queue->front);
  printf("the current_size of queue is %d \n",this_queue->current_size);
  #else
  
  #endif
}//end of queue_enqueue()



/*
 * Function: queue_size
 * ----------------------------
 *   Calculating the size of a existed queue
 *
 *   Parameters: 
 * ` 	1.queue: the queue which one is going to calculate its size
 *
 *   returns: size of the input queue
 */
int queue_size(queue_t* queue){
  queue_t* the_queue = (queue_t*) queue;
  #ifdef DEBUG_PRINTF
  printf("the current_size of queue is %d \n",the_queue->current_size);
  #else
  
  #endif
  return the_queue -> current_size;
}//end of queue_size()


/*
 * Function: queue_isfull
 * ----------------------------
 *   Checking if the queue is full
 *
 *   Parameters: 
 * ` 	1.queue: the queue which one is going to check if full
 *
 *   returns: 
 * 	1 indicates the input queue is full;
 * 	0 indicates the input queue is not full;
 */
int queue_isfull(queue_t* queue){
  queue_t* the_queue = (queue_t*) queue;
  #ifdef DEBUG_PRINTF
  printf("the current_size of queue is %d \n",the_queue->current_size);
  #else
  
  #endif
  if((the_queue -> current_size)==QUEUE_SIZE){
      return 1;
  }
  else{
    return 0;
  }

}//end of queue_isfull()


/*
 * Function: queue_top
 * ----------------------------
 *   Get the top element of a existed queue
 *
 *   Parameters: 
 * ` 1.queue: the queue which one is going to get top element
 *
 *   returns: pointer of the top element of inserted queue
 */
element_t* queue_top(queue_t* queue){
  queue_t* this_queue = (queue_t*) queue;
  element_t* element_pointer = this_queue -> arr + this_queue -> rear;
  #ifdef DEBUG_PRINTF
  //printf("the top element of queue is %d \n",*element_pointer);
  #else
  
  #endif
  return element_pointer;
}//end of queue_top()

/*
 * Function: queue_dequeue
 * ----------------------------
 *   Removing the top element of a existed queue
 *
 *   Parameters: 
 * ` 1.queue: the queue which one is going to remove top element
 *
 *   returns:
 */
void queue_dequeue(queue_t* queue){
  queue_t* this_queue = (queue_t*) queue;
  
  if(this_queue -> current_size == 0){
    this_queue -> front = -1;
    this_queue -> rear = -1;
    this_queue -> current_size = 0;
    #ifdef DEBUG_PRINTF
    printf("the queue is empty, nothing could be removed \n");
    #else
    
    #endif
  }
  else{
    int i;
    i = (this_queue -> rear)%(this_queue -> size);
    this_queue -> arr[i] = 0;
    
    if(this_queue -> current_size > 0){
      this_queue -> current_size--;
      
      
      
      if(this_queue->rear == this_queue->size-1){
	this_queue -> rear = 0;
      }
      else{
	this_queue -> rear = i+1;
      }
      
      if(this_queue -> current_size == 0){
	this_queue -> front = -1;
	this_queue -> rear = -1;
      }
      #ifdef DEBUG_PRINTF
      printf("\n");
      printf("element removed sucessfully \n");
      printf("the rear of queue is %d \n",this_queue->rear);
      printf("the front of queue is %d \n",this_queue->front);
      printf("the current_size of queue is %d \n",this_queue->current_size);
      #else
      
      #endif
    }
  }
}//end of queue_dequeue()


/*
 * Function: queue_print
 * ----------------------------
 *   Printing out a existed queue
 *
 *   Parameters: 
 * ` 1.queue: the queue which one is going to print out
 *
 *   returns:
 */
void queue_print(queue_t *queue){
  printf("\n");
  printf("printing...... \n");
  queue_t* this_queue =(queue_t*) queue;
  element_t* a  =  this_queue -> arr;
  element_t ele;
  int j;
  int i1=this_queue->rear;
  int i2=this_queue->front;

  
  if(i1<i2){	//normal:rear < front
    //printf("i1>i2 \n");
    for(j=i1;j<=i2;j++){
      //printf("the address of element is %p \n",a+j);
      printf("the %d element is ",j);
      ele = *(a+j);
      qu_element_print(ele);
    }
  }
  if(i1>i2){ //rear > front
    //printf("i1<i2 \n");
    for(j=i1;j < this_queue->size;j++){
      //printf("the address of element is %p \n",a+j);
      printf("the %d element is ",j);
      ele = *(a+j);
      qu_element_print(ele);
    }
    for(j=0;j <= i2;j++){		
      //printf("the address of element is %p \n",a+j);
      printf("the %d element is ",j);
      ele = *(a+j);
      qu_element_print(ele);
    }
  }
  if((i1==i2)&(i1!=-1)&(i2!=-1)){	//only one element
    //printf("the address of element is %p \n",a+i1);
    //printf("i1=i2 \n");
    printf("the %d element is ",i1);
    ele = *(a+i1);
    qu_element_print(ele);
  }
  
  if((i1==-1)&(i2==-1)){
    printf("the queue is empty \n");
  }
  
  #ifdef DEBUG_PRINTF
  printf("elements printed sucessfully \n");
  #else
  
  #endif
}//end of queue_print()

/*------------------------------------------end of Methods------------------------------------------*/

// int main(){
//   element_t el = 'U';
//   queue_t* qu = queue_create();
//   printf("the adress of qu is %p \n",qu);
//   int kk;
//   for(kk=0;kk<12;kk++){
//     queue_enqueue(qu,el);
// 
//   }
//   
//   for(kk=0;kk<2;kk++){
//     queue_dequeue(qu);
//   }
//   
//   queue_print(qu);
//   queue_free(&qu);
//   //printf("AFTER FREE \n");
//   //queue_print(qu);
//   
//   char* k = (char*)qu;
//   //dump_mem(k,100);
//   return 0;
// }

