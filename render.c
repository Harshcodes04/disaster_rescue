#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"
#include "graph.h"

/* the screen is just a big char array, we write letters in it and print it */
char canvas[ROWS][COLS];

struct Coord {
   int x;
   int y;
};

/* these are screen positions only. dijkstra never looks at them.
   taken from the paper map we drew on day 1 */
struct Coord node_pos[MAX_NODES] = {
   {4,2},  {20,2},  {36,2},  {52,2},  {66,2},
   {4,9},  {20,9},  {36,9},  {52,9},  {66,9},
   {4,16}, {20,16}, {36,16}, {52,16}, {66,16}
};

void clear_screen()
{
#ifdef _WIN32
  system("cls");
#else
  printf("\033[2J\033[H");
#endif
}

void clear_canvas()
{
  int i,j;
  for(i=0;i<ROWS;i++)
     for(j=0;j<COLS;j++)
        canvas[i][j] = ' ';
}

void put_label(int x, int y, char *s)
{
  int i;
  int n = strlen(s);
  int sx = x - n/2;
  for(i=0;i<n;i++){
      if(sx+i >= 0 && sx+i < COLS && y >= 0 && y < ROWS)
          canvas[y][sx+i] = s[i];
  }
}

/* on the paper map roads are only straight or exact 45 degree,
   so we just move x and y one step at a time */
char road_char(int a, int b)
{
  int dx = node_pos[b].x - node_pos[a].x;
  int dy = node_pos[b].y - node_pos[a].y;
  if(dy == 0) return '-';
  if(dx == 0) return '|';
  if((dx > 0 && dy > 0) || (dx < 0 && dy < 0)) return '\\';
  return '/';
}

void draw_edge(int a, int b, char ch)
{
  int x,y,bx,by,mx,my;
  char buf[8];
  x  = node_pos[a].x;
  y  = node_pos[a].y;
  bx = node_pos[b].x;
  by = node_pos[b].y;
  mx = (x + bx)/2;
  my = (y + by)/2;

  while(x != bx || y != by){
     if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
         canvas[y][x] = ch;
     if(x < bx) x++; else if(x > bx) x--;
     if(y < by) y++; else if(y > by) y--;
  }
  /* travel minutes sit in the middle of the road */
  sprintf(buf, "%d", get_weight(a,b));
  put_label(mx, my, buf);
}

void map_draw(int victim, int centre, int path[], int len)
{
  int i,a;
  char buf[8];
  struct Edge *e;

  clear_canvas();

  /* all the roads first */
  for(a=0;a<MAX_NODES;a++){
     e = adj[a];
     while(e != NULL){
        if(a < e->dest)            /* each road is stored twice, draw it once */
            draw_edge(a, e->dest, road_char(a, e->dest));
        e = e->next;
     }
  }

  /* now paint over the route with # */
  for(i=0;i+1<len;i++)
      draw_edge(path[i], path[i+1], '#');

  /* labels go last so a road never cuts through them */
  for(i=0;i<MAX_NODES;i++){
      if(i == victim) sprintf(buf,"[V]");
      else if(is_center(i)) sprintf(buf,"[R]");
      else sprintf(buf,"(%d)", i);
      put_label(node_pos[i].x, node_pos[i].y, buf);
  }

  printf("\n");
  for(i=0;i<ROWS;i++){
     for(a=0;a<COLS;a++)
        putchar(canvas[i][a]);
     putchar('\n');
  }
  printf(" [R] rescue centre   [V] victim   # route   number on road = travel minutes\n");
  printf(" rescue centres at junctions :");
  for(i=0;i<ncenter;i++) printf(" %d", center[i]);
  printf("\n");
  if(centre >= 0 && len > 0)
      printf(" ambulance starts from centre %d and goes to junction %d\n", centre, path[len-1]);
  printf("\n");
}
