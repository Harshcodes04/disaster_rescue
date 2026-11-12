#include <stdio.h>
#include "graph.h"

/* member C test driver */
int main()
{
  int p[MAX_NODES];
  int i,len,c;

  build_city();
  printf("city built. %d junctions, centres :", MAX_NODES);
  for(i=0;i<ncenter;i++) printf(" %d", center[i]);
  printf("\n");

  printf("\ndijkstra from junction 0\n");
  dijkstra(0);
  for(i=0;i<MAX_NODES;i++)
     printf("  to %-3d cost %d\n", i, get_dist(i));

  printf("\npath 0 to 14\n");
  print_path(14);

  printf("\nnearest centre for a victim at junction 12\n");
  c = nearest_center(12);
  printf("  winner is centre %d, %d min away\n", c, get_dist(12));
  len = get_path(12, p);
  printf("  path :");
  for(i=0;i<len;i++) printf(" %d", p[i]);
  printf("   (length %d)\n", len);

  printf("\nnearest centre for a victim at junction 4\n");
  c = nearest_center(4);
  printf("  winner is centre %d, %d min away\n", c, get_dist(4));
  print_path(4);

  graph_free();
  return 0;
}
