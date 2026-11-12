#ifndef HASH_H
#define HASH_H

#include "common.h"

void ht_init();
int ht_insert(int id, char name[], int location, int severity);
struct Victim *ht_search(int id);
int ht_update_status(int id, int status, int amb, int route_time);
void ht_display_all();
void ht_free_all();

char *sev_name(int s);
char *stat_name(int s);

#endif
