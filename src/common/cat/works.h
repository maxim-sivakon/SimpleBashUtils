#ifndef WORKS_H
#define WORKS_H

#include "defines.h"

void read_flag(int argc, char **argv, flags_cat *flags) {
  char takeFlag;
  int getTakeFlag = 0;
  const struct option longFlag[] = {{"number-nonblank", 0, 0, 'b'},
                                    {"number", 0, 0, 'n'},
                                    {"squeeze-blank", 0, 0, 's'},
                                    {NULL, 0, NULL, 0}};
  while ((takeFlag = getopt_long(argc, argv, "+bestnvTE", longFlag,
                                 &getTakeFlag)) != -1) {
    if (takeFlag == 'b') {
      flags->b = 1;
    } else if (takeFlag == 'e') {
      flags->v = 1;
      flags->E = 1;
    } else if (takeFlag == 'n') {
      flags->n = 1;
    } else if (takeFlag == 's') {
      flags->s = 1;
    } else if (takeFlag == 't') {
      flags->T = 1;
      flags->v = 1;
    } else if (takeFlag == 'T') {
      flags->T = 1;
    } else if (takeFlag == 'E') {
      flags->E = 1;
    } else if (takeFlag == 'v') {
      flags->v = 1;
    }
  }
}
void read_file(int argc, char **argv, flags_cat *flags) {
  int id = optind;
  while (id < argc) {
    FILE *f = fopen(argv[id], "r");
    if (f) {
      int enter = 1, count = 1, str_count = 1, str_count_b = 1, enter_count = 0;
      char current;
      while ((current = fgetc(f)) != EOF) {
        enter_count = current == '\n' ? enter_count + 1 : 0;
        writeFlags(count, enter, &str_count, enter_count, current, &str_count_b,
                   &flags);
        count++;
        if (current == '\n') {
          str_count_b++;
          str_count++;
          enter = 1;
        } else {
          enter = 0;
        }
      }
    }
    fclose(f);
    id++;
  }
}

void writeFlags(int count, int enter, int *str_count, int enter_count,
                char current, int *str_count_b, flags_cat **flags) {
  if ((*flags)->b && enter == 1) {
    if (enter == 1 && current != '\n') {
      printf("%6d\t", *str_count_b);
    } else {
      if (enter == 1 && current == '\n') {
        *str_count_b -= 1;
      }
      if (count == 1) {
        printf("%6d\t", *str_count_b);
      }
    }
  }

  if ((*flags)->n && (count == 1 || enter == 1) && (!(*flags)->b)) {
    if ((*flags)->s) {
      if (enter_count < 3) {
        printf("%6d\t", *str_count);
      }
    } else {
      printf("%6d\t", *str_count);
    }
  }
  if ((*flags)->E && current == '\n') {
    if ((*flags)->b || (*flags)->s) {
      if (enter_count < 3) {
        printf("$");
      } else {
        if ((*flags)->b && !(*flags)->s) printf("$");
      }
    } else {
      printf("$");
    }
  }
  if ((*flags)->T && current == '\t') {
    printf("^I");
  } else {
    if ((*flags)->s && enter_count < 3) {
      if ((*flags)->v && (((int)current >= 0 && (int)current < 32 &&
                           (int)current != 10 && (int)current != 9) ||
                          (int)current == 127)) {
        if ((int)current == 127) {
          printf("^?");
        } else {
          printf("^%c", (char)((int)current + 64));
        }
      } else {
        printf("%c", current);
      }
    } else {
      if (!(*flags)->s) {
        if ((*flags)->v && (((int)current >= 0 && (int)current < 32 &&
                             (int)current != 10 && (int)current != 9) ||
                            (int)current == 127)) {
          if ((int)current == 127) {
            printf("^?");
          } else
            printf("^%c", (char)((int)current + 64));
        } else {
          printf("%c", current);
        }
      } else {
        *str_count -= 1;
      }
    }
  }
}

#endif