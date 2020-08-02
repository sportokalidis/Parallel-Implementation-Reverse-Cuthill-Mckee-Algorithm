/*
 *	File	: queue.h
 *
 *	Title	: Header file of queue
 *
 *
 *	Author	: Portokalidis Stavros
 *
 *	Date	: 25 March 2020
 *
 */



#ifndef QUEUE_H
#define QUEUE_H



#define QUEUESIZE 40000



/*
 * Type definition of Queue
 *
 * buf -> array with workFunctions objects
 * head, tail -> parameters show us the position of first and final object in buf
 * full, empty -> flags show us if the buf is full or empty
 * *mut, *notFull, *notEmpty -> pointers help us to manage multi tasking in producer() and consumer()
 * arriving_time -> array with clock_t values, update the i'st value, when a producer add a object in i'st posision
*/
typedef struct {
  int buf[QUEUESIZE];
  long head, tail;
  int full, empty;

} queue;


/*
 * Constructor function of QUEUE
 *
*/
queue *queueInit (void);


/*
 * Destructor function of QUEUE
 *
*/
void queueDelete (queue *q);


/*
 * Add an object in queue
 *
 * q -> pointer in queue which we will add an objects
 * in -> the object which add
 *
*/
void queueAdd (queue *q, int in);


/*
 * Delete an object in queue
 *
 * q -> pointer in queue which we will delete an objects
 * in -> the object which delete
 *
*/
void queueDel (queue *q, int *out);




#endif
