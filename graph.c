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
