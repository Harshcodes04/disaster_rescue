#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "queue.h"
#include "hash.h"
#include "graph.h"
#include "render.h"

int clock_tick = 0;      /* request number, works as the timestamp */

/* on_call[a] is the victim that ambulance a went to, or -1 if it is parked */
int on_call[MAX_AMB+1];

/* we remember the last route so the map menu can draw it again */
int last_path[MAX_NODES];
int last_len = 0;
int last_victim = -1;
int last_centre = -1;

int read_int()
{
  int x;
  int c;
  int ok = scanf("%d", &x);
  while((c = getchar()) != '\n' && c != EOF);   /* throw away rest of the line */
  if(ok != 1) return -1;
  return x;
}

void pause_here()
{
  int c;
  printf(" press Enter to continue...");
  while((c = getchar()) != '\n' && c != EOF);
}

void setup_system()
{
  int i;
  build_city();
  hp_init();
  ht_init();
  q_init();
  /* all ambulances start parked and free */
  for(i=1;i<=MAX_AMB;i++){
     q_enqueue(i);
     on_call[i] = -1;
  }
  clock_tick = 0;
  last_len = 0;
  last_victim = -1;
  last_centre = -1;
}

void do_register()
{
  struct Request r;
  char name[30];
  int id, loc, sev;

  printf("\n--- new rescue request ---\n");
  printf(" victim id (number) : ");
  id = read_int();
  if(id <= 0){
      printf(" bad id, request cancelled\n\n");
      return;
  }
  if(ht_search(id) != NULL){
      printf(" victim %d is already registered, request cancelled\n\n", id);
      return;
  }
  printf(" name (one word)    : ");
  if(scanf("%29s", name) != 1) return;
  printf(" junction 0 to %d   : ", MAX_NODES-1);
  loc = read_int();
  if(loc < 0 || loc >= MAX_NODES){
      printf(" junction %d is not on the city map, request cancelled\n\n", loc);
      return;
  }
  printf(" severity 3=critical 2=serious 1=minor : ");
  sev = read_int();
  if(sev < 1 || sev > 3){
      printf(" severity must be 1 2 or 3, request cancelled\n\n");
      return;
  }

  if(!ht_insert(id, name, loc, sev)){
      printf(" could not save the victim record\n\n");
      return;
  }
  clock_tick++;
  r.victim_id = id;
  r.location = loc;
  r.severity = sev;
  r.timestamp = clock_tick;
  hp_push(r);

  printf(" saved. victim %d (%s) at junction %d marked %s\n", id, name, loc, sev_name(sev));

  last_victim = loc;
  last_len = 0;
  last_centre = -1;
  map_draw(loc, -1, last_path, 0);
}

void do_dispatch()
{
  struct Request r;
  int a,c,len,t,i;
  int path[MAX_NODES];

  if(hp_empty()){
     printf("\n no pending requests right now\n\n");
     return;
  }
  if(q_is_empty()){
     /* only peek here. the critical request has to stay on top of the heap,
        if we pop it now it is lost */
     r = hp_top();
     printf("\n all %d ambulances are out on a call\n", MAX_AMB);
     printf(" victim %d (%s) is still first in line and was NOT taken off the heap\n",
             r.victim_id, sev_name(r.severity));
     printf(" use option 3 when an ambulance comes back\n\n");
     return;
  }

  r = hp_pop();
  a = q_dequeue();

  c = nearest_center(r.location);
  if(c < 0){
     printf("\n junction %d cannot be reached by any road\n\n", r.location);
     q_enqueue(a);            /* nothing happened so park the ambulance again */
     return;
  }
  len = get_path(r.location, path);
  t = get_dist(r.location);

  ht_update_status(r.victim_id, DISPATCHED, a, t);
  on_call[a] = r.victim_id;

  clear_screen();
  map_draw(r.location, c, path, len);
  printf(" Dispatched AMB-%d -> victim %d (%s)\n", a, r.victim_id, sev_name(r.severity));
  print_path(r.location);
  printf(" AMB-%d is busy now. %d of %d still free\n\n", a, q_count(), MAX_AMB);

  last_victim = r.location;
  last_centre = c;
  last_len = len;
  for(i=0;i<len;i++)
     last_path[i] = path[i];
}

/* the ambulance finished the job and joins the back of the free ring again */
void do_return()
{
  int a,i,n;
  struct Victim *v;

  n = 0;
  printf("\n ambulances out on a call :");
  for(i=1;i<=MAX_AMB;i++){
     if(on_call[i] > 0){
        printf(" AMB-%d(victim %d)", i, on_call[i]);
        n++;
     }
  }
  printf("\n");
  if(n == 0){
     printf(" none of them is out, everybody is already parked\n\n");
     return;
  }
  printf(" which ambulance came back : ");
  a = read_int();
  if(a < 1 || a > MAX_AMB || on_call[a] <= 0){
     printf(" AMB-%d is not out on a call\n\n", a);
     return;
  }
  v = ht_search(on_call[a]);
  ht_update_status(on_call[a], RESCUED, a, -1);
  printf(" victim %d", on_call[a]);
  if(v != NULL) printf(" (%s)", v->name);
  printf(" is now RESCUED\n");
  on_call[a] = -1;
  q_enqueue(a);
  printf(" AMB-%d parked at the back of the ring. %d of %d free\n\n", a, q_count(), MAX_AMB);
}

void do_query()
{
  int id;
  struct Victim *v;
  printf("\n victim id : ");
  id = read_int();
  v = ht_search(id);
  if(v == NULL){
      printf(" no victim with id %d\n\n", id);
      return;
  }
  printf("\n id        : %d\n", v->id);
  printf(" name      : %s\n", v->name);
  printf(" junction  : %d\n", v->location);
  printf(" severity  : %s\n", sev_name(v->severity));
  printf(" status    : %s\n", stat_name(v->status));
  if(v->amb > 0) printf(" ambulance : AMB-%d\n", v->amb);
  else           printf(" ambulance : not assigned yet\n");
  if(v->route_time >= 0) printf(" route time: %d min\n", v->route_time);
  printf(" bucket    : %d  (id %% %d)\n\n", v->id % TABLE_SIZE, TABLE_SIZE);
}

void show_pending()
{
  int i,n;
  struct Request r;
  n = hp_size();
  printf("\n pending requests in the heap : %d\n", n);
  if(n == 0){
      printf(" (nothing waiting)\n\n");
      return;
  }
  printf(" the heap is not a sorted list, only index 0 is sure to be the best one\n");
  printf("\n IDX   VICTIM  NODE  SEVERITY   TIME\n");
  printf(" ------------------------------------\n");
  for(i=0;i<n;i++){
     r = hp_get(i);
     printf(" %-5d %-7d %-5d %-10s %-5d", i, r.victim_id, r.location, sev_name(r.severity), r.timestamp);
     if(i == 0) printf("  <-- next to be dispatched");
     printf("\n");
  }
  printf("\n");
}

void show_ambulances()
{
  int i,n;
  printf("\n free ambulances : %d of %d\n", q_count(), MAX_AMB);
  if(q_is_empty())
     printf(" the free ring is empty, every ambulance is out\n");
  else {
     printf(" waiting order (the one that waited longest comes first) :");
     for(i=0;i<q_count();i++)
        printf(" AMB-%d", q_peek_at(i));
     printf("\n");
  }
  n = 0;
  printf(" out on a call   :");
  for(i=1;i<=MAX_AMB;i++){
     if(on_call[i] > 0){
        printf(" AMB-%d(victim %d)", i, on_call[i]);
        n++;
     }
  }
  if(n == 0) printf(" none");
  printf("\n front index = %d , rear index = %d\n\n", q_front(), q_rear());
}

void show_map()
{
  clear_screen();
  map_draw(last_victim, last_centre, last_path, last_len);
  if(last_len == 0)
     printf(" no route is highlighted yet, dispatch somebody first\n\n");
}

/* a few victims so the demo does not need typing during the presentation */
void load_demo_victims()
{
  struct Request r;
  int i;
  int ids[6]      = {4001, 4002, 4003, 4004, 4005, 4012};
  char *names[6]  = {"Ramesh", "Sita", "Imran", "Priya", "John", "Arun"};
  int locs[6]     = {14, 2, 12, 8, 5, 13};
  int sevs[6]     = {1, 3, 2, 3, 1, 2};

  for(i=0;i<6;i++){
      if(ht_search(ids[i]) != NULL) continue;
      ht_insert(ids[i], names[i], locs[i], sevs[i]);
      clock_tick++;
      r.victim_id = ids[i];
      r.location = locs[i];
      r.severity = sevs[i];
      r.timestamp = clock_tick;
      hp_push(r);
      printf(" added victim %d %-8s at junction %-3d %s\n", ids[i], names[i], locs[i], sev_name(sevs[i]));
  }
  printf("\n 6 victims loaded. 4001 and 4012 both land in bucket %d, that is a collision.\n", 4001 % TABLE_SIZE);
  printf(" press 2 a few times and watch the critical ones leave first.\n");
  printf(" after %d dispatches the ambulances run out, then use option 3.\n\n", MAX_AMB);
}
