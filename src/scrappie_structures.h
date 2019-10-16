#pragma once
#ifndef SCRAPPIE_STRUCTURES_H
#    define SCRAPPIE_STRUCTURES_H

#    include <inttypes.h>
#    include <stddef.h>

typedef struct {
  int64_t start;
  int64_t length;
  double mean;
  double stdv;
  int pos;
  int state;
} event_t;

typedef struct {
  size_t n;
  size_t start;
  size_t end;
  event_t *event;
} event_table;

typedef struct {
  size_t n;
  size_t start;
  size_t end;
  float *raw;
} raw_table;

#endif                          /* SCRAPPIE_DATA_H */
