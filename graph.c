#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "heap.h"

struct Edge *adj[MAX_NODES];
int center[MAX_CENTERS];
int ncenter = 0;

int dist[MAX_NODES];
int parent[MAX_NODES];
int done[MAX_NODES];

void graph_init()
{
  int i;
  for(i=0;i<MAX_NODES;i++)
      adj[i] = NULL;
  ncenter = 0;
}

/* roads go both ways so we add the edge in both lists */
void add_edge(int a, int b, int w)
{
  struct Edge *e1,*e2;
  if(a < 0 || a >= MAX_NODES || b < 0 || b >= MAX_NODES) return;
  e1 = (struct Edge *)malloc(sizeof(struct Edge));
  e1->dest = b;
  e1->weight = w;
  e1->next = adj[a];
  adj[a] = e1;

  e2 = (struct Edge *)malloc(sizeof(struct Edge));
  e2->dest = a;
  e2->weight = w;
  e2->next = adj[b];
  adj[b] = e2;
}

int get_weight(int a, int b)
{
  struct Edge *e = adj[a];
  while(e != NULL){
     if(e->dest == b) return e->weight;
     e = e->next;
  }
  return 0;
}

int is_center(int n)
{
  int i;
  for(i=0;i<ncenter;i++)
     if(center[i] == n) return 1;
  return 0;
}

/* the demo city. 15 junctions and 22 two way roads.
   weights are travel minutes, not screen distance */
void build_city()
{
  graph_init();

  /* top row */
  add_edge(0,1,4);
  add_edge(1,2,6);
  add_edge(2,3,5);
  add_edge(3,4,7);

  /* middle row */
  add_edge(5,6,3);
  add_edge(6,7,8);
  add_edge(7,8,2);
  add_edge(8,9,6);

  /* bottom row */
  add_edge(10,11,5);
  add_edge(11,12,4);
  add_edge(12,13,9);
  add_edge(13,14,3);

  /* the vertical roads */
  add_edge(0,5,9);
  add_edge(5,10,4);
  add_edge(1,6,2);
  add_edge(6,11,7);
  add_edge(2,7,5);
  add_edge(7,12,3);
  add_edge(3,8,6);
  add_edge(8,13,4);
  add_edge(4,9,8);
  add_edge(9,14,5);

  /* rescue centres */
  ncenter = 0;
  center[ncenter] = 0;  ncenter++;
  center[ncenter] = 9;  ncenter++;
  center[ncenter] = 11; ncenter++;
}

void dijkstra(int src)
{
  int i,u,d;
  struct Edge *e;
  for(i=0;i<MAX_NODES;i++){
     dist[i] = INF;
     parent[i] = -1;
     done[i] = 0;
  }
  dist[src] = 0;
  mh_init();
  mh_push(0, src);

  while(!mh_empty()){
     mh_pop(&d, &u);
     if(done[u]) continue;     /* old copy of this node, just skip it */
     done[u] = 1;
     e = adj[u];
     while(e != NULL){
         if(dist[u] + e->weight < dist[e->dest]){
             dist[e->dest] = dist[u] + e->weight;
             parent[e->dest] = u;
             mh_push(dist[e->dest], e->dest);
         }
         e = e->next;
     }
  }
}

/* push every centre at distance 0 and run once.
   then walk back from the victim, whoever we land on is the nearest centre */
int nearest_center(int victim)
{
  int i,u,d,c;
  struct Edge *e;
  for(i=0;i<MAX_NODES;i++){
     dist[i] = INF;
     parent[i] = -1;
     done[i] = 0;
  }
  mh_init();
  for(i=0;i<ncenter;i++){
      dist[center[i]] = 0;
      mh_push(0, center[i]);
  }

  while(!mh_empty()){
    mh_pop(&d, &u);
    if(done[u]) continue;
    done[u] = 1;
    e = adj[u];
    while(e != NULL){
        if(dist[u] + e->weight < dist[e->dest]){
            dist[e->dest] = dist[u] + e->weight;
            parent[e->dest] = u;
            mh_push(dist[e->dest], e->dest);
        }
        e = e->next;
    }
  }

  if(dist[victim] >= INF) return -1;
  c = victim;
  while(parent[c] != -1)
     c = parent[c];
  return c;
}

int get_path(int dest, int out[])
{
  int tmp[MAX_NODES];
  int n = 0;
  int i,v;
  if(dest < 0 || dest >= MAX_NODES) return 0;
  if(dist[dest] >= INF) return 0;
  v = dest;
  while(v != -1){
     tmp[n] = v;
     n++;
     v = parent[v];
  }
  /* tmp is dest..source so flip it */
  for(i=0;i<n;i++)
     out[i] = tmp[n-1-i];
  return n;
}

void print_path(int dest)
{
  int p[MAX_NODES];
  int len,i;
  len = get_path(dest, p);
  if(len == 0){
      printf(" no road reaches junction %d\n", dest);
      return;
  }
  printf(" Route: ");
  for(i=0;i<len;i++){
     printf("%d", p[i]);
     if(i < len-1) printf(" -> ");
  }
  printf("   (total %d min)\n", dist[dest]);
}

int get_dist(int n)
{
  return dist[n];
}

void graph_free()
{
  int i;
  struct Edge *e,*t;
  for(i=0;i<MAX_NODES;i++){
     e = adj[i];
     while(e != NULL){
        t = e->next;
        free(e);
        e = t;
     }
     adj[i] = NULL;
  }
}
