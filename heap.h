#ifndef HEAP_H
#define HEAP_H

#include "common.h"

/* max heap of rescue requests */
void hp_init();
int hp_push(struct Request r);
struct Request hp_pop();
struct Request hp_top();
int hp_empty();
int hp_size();
struct Request hp_get(int i);

/* small min heap of (distance,node) used by dijkstra */
void mh_init();
void mh_push(int dist, int node);
void mh_pop(int *dist, int *node);
int mh_empty();

#endif
