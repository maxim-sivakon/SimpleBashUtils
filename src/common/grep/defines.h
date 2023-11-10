#ifndef DEFINES_H
#define DEFINES_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_SEARCH_LENGTH 100

typedef struct flags_grep {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flags_grep;

char *find_options(int argc, char **argv, flags_grep *flags, int *count,
                   char *samples);

void run_utilit(int argc, char **argv, char *samples, int count,
                flags_grep *flags);

char **sample_into_array(char *optarg, int count, char **sample_array);

char **no_flags_sample_found(char **argv, char **sample_array);

char *samples_connect(char **samples, int count_samples, char *sample_link,
                      flags_grep *flags);

char **file_matrix(int argc, char **argv, char **files, int count_sample,
                   int *i, flags_grep *flags);

void open_file_func(char *sample_link, char **file, int curent_file,
                    int count_files, flags_grep *flags);

void print_rezult(char *curent_string, char **files, int curent_file,
                  int count_string, int count_files, flags_grep *flags);

void flag_filter(char *curent_string, int print_status, char **file,
                 int curent_file, int *count_str_smp, int count_files, int *fl,
                 int count_string, int size_line, regex_t expr,
                 flags_grep *flags);

char *samples_for_flag_f(char *sample_link, char *optarg);

void flag_o_print(char *curent_string, char **file, int curent_file,
                  int count_string, int count_files, int size_line,
                  regex_t expr, flags_grep *flags);

void grep_file(char *filename, char *search_term);

#endif