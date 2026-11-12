#include <stdio.h>
#include "render.h"
#include "graph.h"

/* member A test driver. the map is drawn with a FAKE path that we type by hand,
   so this works even before dijkstra is written */
int main()
{
  int fake[4] = {0, 1, 6, 11};
  int none[1] = {0};

  build_city();

  printf("\n1) plain city, nobody on it\n");
  map_draw(-1, -1, none, 0);

  printf("\n2) victim sitting at junction 11, no route yet\n");
  map_draw(11, -1, none, 0);

  printf("\n3) hard coded fake route 0 -> 1 -> 6 -> 11\n");
  map_draw(11, 0, fake, 4);
  printf(" by hand that road costs 4 + 2 + 7 = 13 min\n");
  printf(" the # marks must sit exactly on those three roads\n\n");

  graph_free();
  return 0;
}
