/********************************************************************************
 * File Name:	mylist.c
 * Author: 	Qinrui Yan
 * Organizaion: KU_Leuven-GroupT
 * Date: 	2015-06-01
 * E-mail: 	castoryan1991@gmail.com
 * Description: .c file of dynamic data structure linked list
 *******************************************************************************/

#include "mylist.h"
#include "stdlib.h"
#include "stdio.h"



extern void element_print(void* data_element);

int list_error;


struct list_node{
  list_node_t *next;
  list_node_t *prev;
  void *element;
};




/*==========================================================================================*/
/*=======================================Start==============================================*/
/*==========================================================================================*/


// Returns a pointer to a newly-allocated list.
// Returns NULL if memory allocation failed and list_errno is set to LIST_MEMORY_ERROR 
list_ptr_t list_create( // callback functions
void (*element_copy)(element_ptr_t *dest_element, element_ptr_t src_element),
			void (*element_free)(element_ptr_t *element),
			int (*element_compare)(element_ptr_t x, element_ptr_t y),
			void (*element_print)(element_ptr_t element)
){
  list_t *new_list;
  new_list = malloc(sizeof(list_t));
  new_list -> head = NULL;
  new_list -> size = 0;
  new_list -> print_func = element_print;
  return new_list;
}


//return the last node of list
list_node_ptr_t get_last(list_node_ptr_t header){
  int count = 0;
  list_node_ptr_t node_current = header;
  
  while(node_current -> next != NULL){
    node_current = node_current -> next;
    count++;
  }
  return node_current;
}

//return the node at specify index
list_node_ptr_t get_index(list_node_ptr_t header,int index){
  int count = 0;
  list_node_ptr_t node_current = header;
  
  for(count = 0; count < index-1; count++){
    node_current = node_current -> next;
  }
  return node_current;
  
}


// Every list node and node element of the list needs to be deleted (free memory)
// The list itself also needs to be deleted (free all memory) and set to NULL
void my_list_free( list_ptr_t* list ){
  
  list_node_ptr_t header_ptr = (*list) -> head;		//find the address of header
  list_node_ptr_t node_ptr_last = get_last(header_ptr);//find the last node
  list_node_ptr_t node_current_ptr = node_ptr_last;	//start from the last one
  
  while(node_current_ptr -> prev != NULL){
    #ifdef DEBUG
    printf("\n");
    printf("the  current ptr is %p\n",node_current_ptr);
    printf("the next of current ptr is %p\n",node_current_ptr->next);
    printf("the element of current ptr is %p\n",node_current_ptr->element);
    printf("the prev of current ptr is %p\n",node_current_ptr->prev);
    #endif
    free(node_current_ptr->element);
    node_current_ptr = node_current_ptr -> prev;
    free(node_current_ptr -> next);
    
  }
  printf("\n");
  #ifdef DEBUG
  printf("the current ptr is %p\n",node_current_ptr);
  printf("the next of current ptr is %p\n",node_current_ptr->next);
  printf("the element of current ptr is %p\n",node_current_ptr->element);
  printf("the prev of current ptr is %p\n",node_current_ptr->prev);
  #endif
  free(header_ptr->element);
  free((*list)->head);
  free(*list);
}


// Returns the number of elements in 'list'.
int list_size( list_ptr_t list ){
  return list -> size;
}


// Inserts a new list node containing 'element' in 'list' at position 'index'  and returns a pointer to the new list.
// Remark: the first list node has index 0.
// If 'index' is 0 or negative, the list node is inserted at the start of 'list'. 
// If 'index' is bigger than the number of elements in 'list', the list node is inserted at the end of 'list'.
// Returns NULL if memory allocation failed and list_errno is set to LIST_MEMORY_ERROR 
list_ptr_t list_insert_at_index( list_ptr_t list, element_ptr_t element, int index){
  list_node_ptr_t node_ptr = malloc(sizeof(list_node_t));

  if(list -> head == NULL){
    node_ptr -> prev = NULL;
    node_ptr -> next = NULL;
    node_ptr -> element = element;
    list -> head = node_ptr;
    list -> size ++;
  }
  else{
    if(index <= 0){
      node_ptr -> next = list -> head;					//the next node is previous header
      node_ptr -> element = element;
      list -> head = node_ptr;
      list -> size ++;
    }
    else if((index>0)&(index<=list->size)){
      
      list_node_ptr_t node_ptr_prev = get_index(list -> head,index-1);	//find the (index-1)th node
      list_node_ptr_t node_ptr_next = get_index(list -> head,index);	//find the (index)th node
      
      //insert the new node between the index and index-1 node
      node_ptr_prev -> next = node_ptr;				
      node_ptr_next -> prev = node_ptr;
      node_ptr -> prev = node_ptr_prev;
      node_ptr -> next = node_ptr_next;
      
      node_ptr -> element = element;
      list -> size ++;
    }
    else if(index>(list->size)){
      list_node_ptr_t header_ptr = list -> head;		//find the address of header
      list_node_ptr_t node_ptr_last = get_last(header_ptr);
      
      node_ptr_last -> next = node_ptr;
      node_ptr -> prev = node_ptr_last;
      
      node_ptr -> element = element;
      list -> size ++;
    }
  }
  
  return list;
}


// list_ptr_t list_insert_at_reference( list_ptr_t list, void* element, list_node_ptr_t reference ){
//   list_node_ptr_t current_element = list_get_element_at_index(index);
//   
//   list_node_ptr_t node_new = malloc(sizeof(list_node_t))；
//   list -> copy_func(&(new_node -> element),element);
//   
//   new_node -> next = current_node -> next; 
//   new_node -> prev = current_node;
//   current_node -> next = new_node;
//   
//   new_node-> prev = new_node;
//   return NULL;
// }

// Removes the list node at index 'index' from 'list'. NO free() is called on the element pointer of the list node. 
// If 'index' is 0 or negative, the first list node is removed. 
// If 'index' is bigger than the number of elements in 'list', the last list node is removed.
// If the list is empty, return list and list_errno is set to LIST_EMPTY_ERROR (to see the difference with removing the last element from a list)
list_ptr_t list_remove_at_index( list_ptr_t list, int index){
  list_node_ptr_t header_ptr = list -> head;
  
  
  if(list -> size == 0){
    list_error = LIST_EMPTY_ERROR;
    return list;
  }
  
  if(index <= 0){
    list_node_ptr_t node_current_ptr = header_ptr;
    list_node_ptr_t node_next_ptr = node_current_ptr->next;
    
    node_current_ptr -> prev = NULL;
    free(node_current_ptr);
    list -> head = node_next_ptr;
    list -> size--;
    
  }
  else if(index > list -> size){
    list_node_ptr_t node_current_ptr = get_last(header_ptr);
    list_node_ptr_t node_ptr_prev = node_current_ptr -> prev;
    
    free(node_current_ptr);
    node_ptr_prev -> next = NULL;
    list -> size--;
  }
  else{
    
    list_node_ptr_t node_current_ptr = get_index(header_ptr,index);
    list_node_ptr_t node_prev_ptr = get_index(header_ptr,index-1);
    list_node_ptr_t node_next_ptr = get_index(header_ptr,index+1);
    
    node_prev_ptr -> next = node_next_ptr;
    node_next_ptr -> prev = node_prev_ptr;
    
    free(node_current_ptr);
    list -> size--;
  }
  
  return list;
}



// Deletes the list node at index 'index' in 'list'. 
// A free() is called on the element pointer of the list node to free any dynamic memory allocated to the element pointer. 
// If 'index' is 0 or negative, the first list node is deleted. 
// If 'index' is bigger than the number of elements in 'list', the last list node is deleted.
// If the list is empty, return list and list_errno is set to LIST_EMPTY_ERROR (to see the difference with freeing the last element from a list)
list_ptr_t list_free_at_index( list_ptr_t list, int index){
  list_node_ptr_t header_ptr = list -> head;
  
  if(list -> size == 0){
    list_error = LIST_EMPTY_ERROR;
    return list;
  }
  
  if(index <= 0){
    list_node_ptr_t node_current_ptr = header_ptr;
    list_node_ptr_t node_ptr_next = node_current_ptr -> next;
    node_ptr_next -> prev = NULL;
    list -> head = node_ptr_next;
    free(node_current_ptr);
  }
  else if(index > list->size){
    list_node_ptr_t node_current_ptr = get_last(header_ptr);
    list_node_ptr_t node_ptr_prev = node_current_ptr -> prev;
    node_ptr_prev -> next = NULL;
    free(node_current_ptr);
    
  }
  else{
    list_node_ptr_t node_current_ptr = get_index(header_ptr,index);
    free(node_current_ptr -> element);
    free(node_current_ptr);
  }
  return list;
}


// Returns a reference to the list node with index 'index' in 'list'. 
// If 'index' is 0 or negative, a reference to the first list node is returned. 
// If 'index' is bigger than the number of list nodes in 'list', a reference to the last list node is returned. 
// If the list is empty, NULL is returned.
list_node_ptr_t list_get_reference_at_index( list_ptr_t list, int index ){
  
  if(list -> size == 0){
    return NULL;
  }  
  
  if(index <= 0){
    return list -> head;
  }
  else if(index > (list->size)){
    list_node_ptr_t node_ptr_last = get_last(list -> head);
    return node_ptr_last;
  }
  else{
    
    int count = 0;
    list_node_ptr_t node_current = list -> head;
    
    for(count = 0; count < index-1; count++){
      node_current = node_current -> next;
    }
    return node_current;
  }
  
}

// Returns the list element contained in the list node with index 'index' in 'list'. 
// If 'index' is 0 or negative, the element of the first list node is returned. 
// If 'index' is bigger than the number of elements in 'list', the element of the last list node is returned.
// If the list is empty, NULL is returned.
element_ptr_t list_get_element_at_index( list_ptr_t list, int index ){
  
  int count = 0;
  list_node_ptr_t node_current = list -> head;
  element_ptr_t element;
  
  for(count = 0; count < index-1; count++){
    node_current = node_current -> next;
  }
  element = node_current -> element;
  return element;
  
}

// Returns an index to the first list node in 'list' containing 'element'.  
// If 'element' is not found in 'list', -1 is returned.
int list_get_index_of_element( list_ptr_t list, element_ptr_t element ){
  
  int count = 0;
  list_node_ptr_t node_current = list -> head;
  
  while(node_current -> next != NULL){
    
    if(node_current -> element == element){
      return count;
    }
    node_current = node_current -> next;
    count++;
  }
  return -1;
}

// for testing purposes: print the entire list on screen
void list_print( list_ptr_t list ){
  int count = 0;
  list_node_ptr_t header_ptr = list -> head;
  list_node_ptr_t node_current_ptr  = header_ptr;
  
  
  if(list -> size == 0){
    printf("the list is empty\n");
  }
  else{
    printf("Linked List Printing... \n");
    printf("the address of list is %p \n",list);
    printf("the size of list is %d \n",list -> size);
    while(node_current_ptr -> next != NULL){
      count++;
      printf("the address of %d element is %p \n",count,node_current_ptr->element);
      //printf("the room_id of %d element is %d \n",count,node_current_ptr->element);
      element_print(node_current_ptr->element);
      node_current_ptr = node_current_ptr -> next;
    }
    printf("the address of %d element is %p \n",count+1,node_current_ptr->element);
    element_print(node_current_ptr->element);
  }
  
}


#ifdef LIST_EXTRA
// Inserts a new list node containing 'element' in the 'list' at position 'reference'  and returns a pointer to the new list. 
// If 'reference' is NULL, the element is inserted at the end of 'list'.

list_ptr_t list_insert_at_reference( list_ptr_t list, element_ptr_t element, list_node_ptr_t reference ){
  list_node_ptr_t header_ptr = list -> head;
  list_node_ptr_t node_new;
  
  int count = 0;
  list_node_ptr_t node_current = header;
  
  while(node_current -> next != NULL){
    if(node_current  == reference){
      list_node_ptr_t node_next_ptr = node_current -> next;
      
      node_next_ptr -> prev = node_new;
      node_new -> prev = node -> current;
      node_current -> next = node_new
      node_new -> next = node_next_ptr;
      
      return list;
    }
    node_current = node_current -> next;
    count++;
  }
  
  if(reference == NULL){
    list_node_ptr_t node_ptr_last = get_last(header_ptr);
    node_ptr_last -> next = node_new;
    node_new -> prev = node_ptr_last;
    node_new -> element = element;
    return list
  }
  return list;
}



// Inserts a new list node containing 'element' in the sorted 'list' and returns a pointer to the new list. 
// The 'list' must be sorted before calling this function. 
// The sorting is done in ascending order according to a comparison function.  
// If two members compare as equal, their order in the sorted array is undefined.
list_ptr_t list_insert_sorted( list_ptr_t list, element_ptr_t element ){
  
}


// Removes the list node with reference 'reference' in 'list'. 
// NO free() is called on the element pointer of the list node. 
// If 'reference' is NULL, the last list node is removed.
// If the list is empty, return list and list_errno is set to LIST_EMPTY_ERROR
list_ptr_t list_remove_at_reference( list_ptr_t list, list_node_ptr_t reference ){
  list_node_ptr_t header_ptr = list -> head;
  list_node_ptr_t node_new;
  
  int count = 0;
  list_node_ptr_t node_current = header;
  
  while(node_current -> next != NULL){
    if(node_current  == reference){
      list_node_ptr_t node_next_ptr = node_current -> next;
      
      node_next_ptr -> prev = node_new;
      node_new -> prev = node -> current;
      node_current -> next = node_new
      node_new -> next = node_next_ptr;
      
      return list;
    }
    node_current = node_current -> next;
    count++;
  }
}


list_ptr_t list_free_at_reference( list_ptr_t list, list_node_ptr_t reference ){
  list_node_ptr_t node_next = list_get_next_reference(list,reference);
  list_node_ptr_t node_prev = list_get_previous_reference(list,reference);
  list_node_ptr_t node_current = node_next -> prev;
  node_next -> prev = node_prev;
  node_prev -> next = node_next;
  free(node_current);
}


list_ptr_t list_remove_element( list_ptr_t list, element_ptr_t element ){
  return list_ptr_t list_free_element(list,element);
}


list_node_ptr_t list_get_first_reference( list_ptr_t list ){
  if(list -> head != NULL ){
    list_node_ptr_t node_ptr_head = list -> head;
    return node_ptr_head;
  }
  else{
    return NULL;
  }  
}


list_node_ptr_t list_get_last_reference( list_ptr_t list ){
  list_node_ptr_t node_current = list -> head;
  if(list -> size > 0){
    while(node_current -> next != NULL){
      node_current = node_current -> next;
    }
    if(node_current == NULL){
      return node_current;
    }
    else{
      return NULL;
    }
  }
  else{
    return NULL;
  }
}


list_node_ptr_t list_get_next_reference( list_ptr_t list, list_node_ptr_t reference ){
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      if(node_current  == reference){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current  == reference){
      return node_current -> next;
    }
    else{		//no node in list match reference
      return NULL;
    }
  }
  else{			//list is null
    return NULL;
  }
}


list_node_ptr_t list_get_previous_reference( list_ptr_t list, list_node_ptr_t reference ){
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      if(node_current  == reference){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current  == reference){
      return node_current -> prev;
    }
    else{		//no node in list match reference
      return NULL;
    }
  }
  else{			//list is null
    return NULL;
  }
}


element_ptr_t list_get_element_at_reference( list_ptr_t list, list_node_ptr_t reference ){
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      if(node_current  == reference){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current  == reference){
      return node_current -> element;
    }
    else{		//no node in list match reference
      return NULL;
    }
  }
  else{			//list is null
    return NULL;
  }
}


list_node_ptr_t list_get_reference_of_element( list_ptr_t list, element_ptr_t element ){
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      if(node_current -> element == element){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current -> element == element){
      return node_current;
    }
    else{		//no node in list match element
      return NULL;
    }
  }
  else{			//list is null
    return NULL;
  }
}


int list_get_index_of_reference( list_ptr_t list, list_node_ptr_t reference ){
  int count = 0;
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      count++;
      if(node_current  == reference){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current  == reference){
      return count;
    }
    else{		//no node in list match reference
      return NULL;
    }
  }
  else{			//list is null
    return NULL;
  }
}

// Finds the first list node in 'list' that contains 'element' and deletes the list node from 'list'. 
// A free() is called on the element pointer of the list node to free any dynamic memory allocated to the element pointer. 
list_ptr_t list_free_element( list_ptr_t list, element_ptr_t element ){
  list_node_ptr_t node_current = list -> head;
  list_node_ptr_t node_next;
  list_node_ptr_t node_prev;
  if(list->size > 0){
    while(node_current -> next != NULL){
      if(node_current -> element == element){
	break;
      }
      node_current = node_current -> next;
    }
    
    if(node_current -> element == element){
      node_next = node_current -> next;
      node_prev = node_current -> prev;
      
      node_next -> prev = node_prev;
      node_prev -> next = node_next;
      free(node_current);
      return list;
    }
    else{
      return NULL;
    }
  }
  else{
    return NULL;
  }
}

#endif

