#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "heap.h"
#include "queue.h"
#include "hash.h"
#include "graph.h"
#include "render.h"

/* these all live in dispatch.c */
void setup_system();
void do_register();
void do_dispatch();
void do_return();
void do_query();
void show_pending();
void show_ambulances();
void show_map();
void load_demo_victims();
int read_int();
void pause_here();

void menu()
{
  printf("=====================================================\n");
  printf("     DISASTER RESCUE COORDINATION SYSTEM\n");
  printf("=====================================================\n");
  printf("  1. register a new rescue request\n");
  printf("  2. dispatch next ambulance\n");
  printf("  3. an ambulance came back (rescue finished)\n");
  printf("  4. victim status query\n");
  printf("  5. show pending requests\n");
  printf("  6. show free ambulances\n");
  printf("  7. show city map\n");
  printf("  8. show all victims\n");
  printf("  9. load demo victims\n");
  printf("  0. exit\n");
  printf("-----------------------------------------------------\n");
  printf(" your choice : ");
}

int main()
{
  int ch;
  setup_system();
  printf("\n city loaded : %d junctions, rescue centres at", MAX_NODES);
  for(ch=0;ch<ncenter;ch++) printf(" %d", center[ch]);
  printf(", %d ambulances free\n", MAX_AMB);
  pause_here();

  while(1){
      clear_screen();
      menu();
      ch = read_int();

      if(ch == 1){ do_register(); pause_here(); }
      else if(ch == 2){ do_dispatch(); pause_here(); }
      else if(ch == 3){ do_return(); pause_here(); }
      else if(ch == 4){ do_query(); pause_here(); }
      else if(ch == 5){ show_pending(); pause_here(); }
      else if(ch == 6){ show_ambulances(); pause_here(); }
      else if(ch == 7){ show_map(); pause_here(); }
      else if(ch == 8){ ht_display_all(); pause_here(); }
      else if(ch == 9){ printf("\n"); load_demo_victims(); pause_here(); }
      else if(ch == 0){
          printf("\n shutting down the control room. bye.\n");
          break;
      }
      else {
         printf("\n %d is not on the menu\n\n", ch);
         pause_here();
      }
  }

  ht_free_all();
  graph_free();
  return 0;
}
