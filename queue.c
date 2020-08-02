/*
 *	File	: queue.c
 *
 *	Title	: Implementations of queue
 *
 *
 *	Author	: Portokalidis Stavros
 *
 *	Date	: 25 March 2020
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "queue.h"



queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;


  return (q);
}



void queueDelete (queue *q)
{
  free (q);
}



void queueAdd (queue *q, int in)
{
  q->buf[q->tail] = in;

  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;


  return;
}



void queueDel (queue *q, int *out)
{
    *out = q->buf[q->head];

    q->head++;
    if (q->head == QUEUESIZE)
      q->head = 0;
    if (q->head == q->tail)
      q->empty = 1;
    q->full = 0;

  return;
}
