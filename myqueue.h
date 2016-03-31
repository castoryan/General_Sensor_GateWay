/********************************************************************************
 * File Name:	myqueue.h
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: Header file of implementing data structure of circular queue
 *******************************************************************************/

/*===============================================================================
		Include files and Definitions(defines, typedefs, ...)
================================================================================*/

#ifndef MYQUEUE_
#define MYQUEUE_

#include "data_type.h"
typedef struct queue queue_t;

/*
 ** The default queue size is 5
 */
#ifndef QUEUE_SIZE
    #define QUEUE_SIZE 5
#endif

/*
 **  Make some changes here to define the type of element that will be stored in the queue
 */

// typedef struct sensor_data_element sensor_data_element_t;
// typedef sensor_data_element_t *sensor_data_element_ptr_t;

typedef sensor_data_element_ptr_t element_t;
//typedef int element_t;

/*------------------------end of Include files and Definitions---------------------------*/


/*
 **  Creates (memory allocation!) and initializes the queue and prepares it for usage
 **  Return a pointer to the newly created queue
 **  Returns NULL if queue creation failed
 */
queue_t* queue_create();

/*  
 **  Add an element to the queue
 **  Does nothing if queue is full
 */
void queue_enqueue(queue_t* queue, element_t element);

/*
 **  Delete the queue from memory; set queue to NULL
 **  The queue can no longer be used unless queue_create is called again
 */
void queue_free(queue_t** queue);

/*
 **  Return the number of elements in the queue
 */
int queue_size(queue_t* queue);

/*
 **  Return if the queue is full
 */
int queue_isfull(queue_t* queue);

/*
 **  Return a pointer to the top element in the queue
 **  Returns NULL if queue is empty
 */
element_t* queue_top(queue_t* queue);

/*
 **  Remove the top element from the queue
 **  Does nothing if queue is empty
 */
void queue_dequeue(queue_t* queue);

/*
 **  Print all elements in the queue, starting from the front element
 */
void queue_print(queue_t *queue);

#endif //MYQUEUE_


