#include <stdio.h>
#include "hash.h"

/* member A test driver. 22 victims, some of them on purpose in the same bucket */
int main()
{
  int i,miss;
  struct Victim *v;
  /* 4001 4012 4023 all give the same id%11, same for 4002 4013 4024 */
  int ids[22] = {4001,4012,4023,4002,4013,4024,4003,4014,4004,4015,4005,
                 4016,4006,4017,4007,4018,4008,4019,4009,4020,4010,4021};

  ht_init();
  printf("TABLE_SIZE = %d (prime)\n\n", TABLE_SIZE);
  for(i=0;i<22;i++){
      ht_insert(ids[i], "victim", i % MAX_NODES, (i % 3) + 1);
      printf(" inserted %d  -> bucket %d\n", ids[i], ids[i] % TABLE_SIZE);
  }

  printf("\nsearching every one of them back\n");
  miss = 0;
  for(i=0;i<22;i++){
      v = ht_search(ids[i]);
      if(v == NULL){
          printf("  MISSING %d\n", ids[i]);
          miss++;
      }
  }
  if(miss == 0) printf("  all 22 found, chaining works\n");

  printf("\nduplicate insert of 4001 should fail : %d\n", ht_insert(4001,"copy",0,1));
  printf("search for an id we never put in (9999) : %p\n", (void *)ht_search(9999));

  printf("\nupdate 4001 to RESCUED with AMB-3 and 12 min : %d\n",
          ht_update_status(4001, RESCUED, 3, 12));
  v = ht_search(4001);
  printf("4001 is now %s amb %d time %d\n", stat_name(v->status), v->amb, v->route_time);

  ht_display_all();
  ht_free_all();
  return 0;
}
