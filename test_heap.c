#include <stdio.h>
#include "heap.h"

/* member B test driver. push mixed severities, pop everything,
   output must be severity high to low and fifo inside the same severity */
int main()
{
  struct Request r;
  int i;
  int sev[10]  = {1, 3, 2, 1, 3, 2, 3, 1, 2, 2};
  int vid[10]  = {101,102,103,104,105,106,107,108,109,110};

  hp_init();
  printf("pushing 10 requests\n");
  for(i=0;i<10;i++){
     r.victim_id = vid[i];
     r.location = i;
     r.severity = sev[i];
     r.timestamp = i+1;
     hp_push(r);
     printf("  push victim %d severity %d time %d\n", vid[i], sev[i], i+1);
  }
  printf("heap size = %d\n", hp_size());
  printf("top of heap is victim %d\n", hp_top().victim_id);

  printf("\npopping everything\n");
  while(!hp_empty()){
      r = hp_pop();
      printf("  victim %d  severity %d  time %d\n", r.victim_id, r.severity, r.timestamp);
  }
  printf("empty ? %d\n", hp_empty());
  printf("\nexpected order : 102 105 107 103 106 109 110 101 104 108\n");

  /* the min heap that dijkstra borrows */
  printf("\nmin heap check\n");
  mh_init();
  mh_push(7,0);
  mh_push(2,1);
  mh_push(9,2);
  mh_push(2,3);
  mh_push(4,4);
  while(!mh_empty()){
     int d,n;
     mh_pop(&d,&n);
     printf("  dist %d node %d\n", d, n);
  }
  printf("distances must come out smallest first\n");
  return 0;
}
