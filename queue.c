#include <stdio.h>
#include "queue.h"

/* circular queue of free ambulances.
   we use a count variable to tell full from empty */
int amb[MAX_AMB];
int front = 0;
int rear = -1;
int count = 0;

void q_init()
{
  front = 0;
  rear = -1;
  count = 0;
}

int q_is_empty()
{
  if(count == 0) return 1;
  return 0;
}

int q_is_full()
{
   if(count == MAX_AMB) return 1;
   return 0;
}

int q_enqueue(int amb_id)
{
  if(q_is_full()){
      printf("ambulance queue full\n");
      return 0;
  }
  rear = (rear + 1) % MAX_AMB;   /* this wrap is the circular part */
  amb[rear] = amb_id;
  count++;
  return 1;
}

int q_dequeue()
{
  int x;
  if(q_is_empty()) return -1;
  x = amb[front];
  front = (front + 1) % MAX_AMB;
  count--;
  return x;
}

int q_count()
{
  return count;
}

/* used only for printing the free list, i is 0..count-1 */
int q_peek_at(int i)
{
  return amb[(front + i) % MAX_AMB];
}

/* we print these in the menu so the wrap around is visible on screen */
int q_front()
{
  return front;
}

int q_rear()
{
   return rear;
}
