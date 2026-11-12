#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

struct Edge {
   int dest;
   int weight;
   struct Edge *next;
};

extern struct Edge *adj[MAX_NODES];
extern int center[MAX_CENTERS];
extern int ncenter;

void graph_init();
void add_edge(int a, int b, int w);
void build_city();
int get_weight(int a, int b);
int is_center(int n);

void dijkstra(int src);
int nearest_center(int victim);
int get_path(int dest, int out[]);
void print_path(int dest);
int get_dist(int n);
void graph_free();

#endif
