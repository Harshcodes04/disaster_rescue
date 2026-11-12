#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* TABLE_SIZE is a prime number so ids spread out better */
struct Victim *table[TABLE_SIZE];

int hashfn(int id)
{
   return id % TABLE_SIZE;
}

void ht_init()
{
  int i;
  for(i=0;i<TABLE_SIZE;i++)
     table[i] = NULL;
}

struct Victim *ht_search(int id)
{
  int b = hashfn(id);
  struct Victim *p = table[b];
  while(p != NULL){
     if(p->id == id) return p;
     p = p->next;
  }
  return NULL;
}

int ht_insert(int id, char name[], int location, int severity)
{
  int b;
  struct Victim *nw;
  if(ht_search(id) != NULL) return 0;   /* same id already there */
  nw = (struct Victim *)malloc(sizeof(struct Victim));
  if(nw == NULL){
     printf("out of memory\n");
     return 0;
  }
  nw->id = id;
  strcpy(nw->name, name);
  nw->location = location;
  nw->severity = severity;
  nw->status = WAITING;
  nw->amb = -1;
  nw->route_time = -1;
  /* new node goes at the front of the chain */
  b = hashfn(id);
  nw->next = table[b];
  table[b] = nw;
  return 1;
}

int ht_update_status(int id, int status, int amb, int route_time)
{
  struct Victim *p = ht_search(id);
  if(p == NULL) return 0;
  p->status = status;
  if(amb >= 0) p->amb = amb;
  if(route_time >= 0) p->route_time = route_time;
  return 1;
}

char *sev_name(int s)
{
  if(s == 3) return "CRITICAL";
  if(s == 2) return "SERIOUS";
  if(s == 1) return "MINOR";
  return "?";
}

char *stat_name(int s)
{
  if(s == WAITING) return "WAITING";
  if(s == DISPATCHED) return "DISPATCHED";
  if(s == RESCUED) return "RESCUED";
  return "?";
}

void ht_display_all()
{
  int i;
  int n = 0;
  struct Victim *p;
  printf("\n ID     NAME          NODE  SEVERITY   STATUS       AMB   TIME  BUCKET\n");
  printf(" -----------------------------------------------------------------------\n");
  for(i=0;i<TABLE_SIZE;i++){
      p = table[i];
      while(p != NULL){
          printf(" %-6d %-13s %-5d %-10s %-12s ", p->id, p->name, p->location,
                                                  sev_name(p->severity), stat_name(p->status));
          if(p->amb < 0) printf("-     "); else printf("%-5d ", p->amb);
          if(p->route_time < 0) printf("-     "); else printf("%-5d ", p->route_time);
          printf("%d\n", i);
          n++;
          p = p->next;
      }
  }
  if(n == 0) printf(" (no victims registered yet)\n");
  printf(" -----------------------------------------------------------------------\n");
  printf(" total victims = %d , load factor = %.2f\n\n", n, (float)n/TABLE_SIZE);
}

void ht_free_all()
{
  int i;
  struct Victim *p,*t;
  for(i=0;i<TABLE_SIZE;i++){
     p = table[i];
     while(p != NULL){
        t = p->next;
        free(p);
        p = t;
     }
     table[i] = NULL;
  }
}
