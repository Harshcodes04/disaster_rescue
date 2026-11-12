#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

void q_init();
int q_enqueue(int amb_id);
int q_dequeue();
int q_is_empty();
int q_is_full();
int q_count();
int q_peek_at(int i);
int q_front();
int q_rear();

#endif
