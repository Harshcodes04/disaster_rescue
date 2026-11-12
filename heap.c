#include <stdio.h>
#include "heap.h"

struct Request heap[MAX_REQ];
int hcount = 0;

/* this is the only place where we decide who goes first.
   bigger severity first, if same severity then the older request first */
int better(struct Request a, struct Request b)
{
  if(a.severity > b.severity) return 1;
  if(a.severity < b.severity) return 0;
  if(a.timestamp < b.timestamp) return 1;
  return 0;
}

void hp_init()
{
  hcount = 0;
}

void sift_up(int i)
{
  int p;
  struct Request t;
  while(i > 0){
    p = (i-1)/2;
    if(better(heap[i], heap[p])){
        t = heap[i];
        heap[i] = heap[p];
        heap[p] = t;
        i = p;
    }
    else
      break;
  }
}

void sift_down(int i)
{
int l,r,big;
struct Request t;
  while(1){
    l = 2*i+1;
    r = 2*i+2;
    big = i;
    if(l < hcount && better(heap[l], heap[big])) big = l;
    if(r < hcount && better(heap[r], heap[big])) big = r;
    if(big == i) break;
    t = heap[i];
    heap[i] = heap[big];
    heap[big] = t;
    i = big;
  }
}

int hp_push(struct Request r)
{
  if(hcount >= MAX_REQ){
     printf("heap is full, cannot take more requests\n");
     return 0;
  }
  heap[hcount] = r;
  hcount++;
  sift_up(hcount-1);
  return 1;
}

struct Request hp_pop()
{
  struct Request top;
  top = heap[0];
  /* last one comes to the root so the tree stays complete */
  heap[0] = heap[hcount-1];
  hcount--;
  if(hcount > 0) sift_down(0);
  return top;
}

struct Request hp_top()
{
  return heap[0];
}

int hp_empty()
{
   if(hcount == 0) return 1;
   return 0;
}

int hp_size()
{
  return hcount;
}

struct Request hp_get(int i)
{
  return heap[i];
}

/* ---------------- min heap for dijkstra ---------------- */

int mh_dist[MAX_MH];
int mh_node[MAX_MH];
int mcount = 0;

void mh_init()
{
   mcount = 0;
}

void mh_push(int dist, int node)
{
  int i,p,t;
  if(mcount >= MAX_MH) return;
  mh_dist[mcount] = dist;
  mh_node[mcount] = node;
  i = mcount;
  mcount++;
  while(i > 0){
     p = (i-1)/2;
     if(mh_dist[i] < mh_dist[p]){
        t = mh_dist[i]; mh_dist[i] = mh_dist[p]; mh_dist[p] = t;
        t = mh_node[i]; mh_node[i] = mh_node[p]; mh_node[p] = t;
        i = p;
     }
     else break;
  }
}

void mh_pop(int *dist, int *node)
{
  int i,l,r,small,t;
  *dist = mh_dist[0];
  *node = mh_node[0];
  mh_dist[0] = mh_dist[mcount-1];
  mh_node[0] = mh_node[mcount-1];
  mcount--;
  i = 0;
  while(1){
      l = 2*i+1;
      r = 2*i+2;
      small = i;
      if(l < mcount && mh_dist[l] < mh_dist[small]) small = l;
      if(r < mcount && mh_dist[r] < mh_dist[small]) small = r;
      if(small == i) break;
      t = mh_dist[i]; mh_dist[i] = mh_dist[small]; mh_dist[small] = t;
      t = mh_node[i]; mh_node[i] = mh_node[small]; mh_node[small] = t;
      i = small;
  }
}

int mh_empty()
{
  if(mcount == 0) return 1;
  return 0;
}
