/*
 * File:    queue.h
 * Purpose: Implement a first in, first out linked list
 *
 * Notes:
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#define NULL 0
/********************************************************************/

typedef enum{	half_sec_reached,
							power_key_pressed, 
							setup_key_pressed, 
							switch_key_pressed, 
							minus_key_pressed, 
							plus_key_pressed,
} Key_Event;
/* 
 * Individual queue node
 */
typedef struct NODE
{
    struct NODE *next;
    Key_Event event;
} QNODE;

/* 
 * Queue Struture - linked list of qentry items 
 */
typedef struct
{
    QNODE *head;
    QNODE *tail;
} QUEUE;



/*
 * Functions provided by queue.c
 */
void
queue_init(QUEUE *);

int
queue_isempty(QUEUE *);

void
queue_add(QUEUE *, QNODE *);

QNODE*
queue_remove(QUEUE *);

QNODE*
queue_peek(QUEUE *);

void
queue_move(QUEUE *, QUEUE *);

/********************************************************************/

#endif /* _QUEUE_H_ */
