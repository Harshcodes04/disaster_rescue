#ifndef COMMON_H
#define COMMON_H

#define MAX_NODES 15
#define MAX_AMB 5
#define TABLE_SIZE 11
#define MAX_REQ 100
#define MAX_MH 500
#define MAX_CENTERS 5

#define ROWS 19
#define COLS 72

#define INF 999999

enum { WAITING, DISPATCHED, RESCUED };

struct Request {
  int victim_id;
  int location;
  int severity;
  int timestamp;
};

struct Victim {
  int id;
  char name[30];
  int location;
  int severity;
  int status;
  int amb;
  int route_time;
  struct Victim *next;
};

#endif
