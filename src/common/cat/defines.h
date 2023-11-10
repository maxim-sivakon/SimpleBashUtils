#ifndef DEFINES_H
#define DEFINES_H
#include <getopt.h>

#include "stdio.h"

typedef struct flags_cat {
  int b;
  int n;
  int s;
  int e;
  int t;
  int v;
  int T;
  int E;
} flags_cat;

void read_flag(int argc, char **argv, flags_cat *flags);
void read_file(int argc, char **argv, flags_cat *flags);
void writeFlags(int count, int enter, int *str_count, int enter_count,
                char current, int *str_count_b, flags_cat **flag);

#endif