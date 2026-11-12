#include <stdio.h>
#include "queue.h"

/* member A test driver. fill it, empty it, then push past the wrap point */
int main()
{
  int i,x;
  q_init();
  printf("empty at start ? %d\n", q_is_empty());

  printf("\nfilling the queue\n");
  for(i=1;i<=MAX_AMB;i++){
     q_enqueue(i);
     printf("  put AMB-%d   count=%d\n", i, q_count());
  }
  printf("full now ? %d\n", q_is_full());

  printf("\none extra enqueue should be refused\n");
  q_enqueue(99);

  printf("\nemptying the queue\n");
  while(!q_is_empty()){
      x = q_dequeue();
      printf("  got AMB-%d   count=%d\n", x, q_count());
  }
  printf("empty ? %d\n", q_is_empty());
  printf("dequeue on empty returns %d\n", q_dequeue());

  /* this is the real test. indexes must come back to slot 0 */
  printf("\nnow going round the wrap point\n");
  for(i=1;i<=MAX_AMB;i++) q_enqueue(i);
  for(i=0;i<3;i++){
      x = q_dequeue();
      q_enqueue(x);
      printf("  took AMB-%d and parked it again, count=%d\n", x, q_count());
  }
  printf("  order now :");
  for(i=0;i<q_count();i++) printf(" AMB-%d", q_peek_at(i));
  printf("\n");
  printf("\nif the order is 4 5 1 2 3 the wrap works\n");
  return 0;
}
