#ifndef WORKS_H
#define WORKS_H

#include "defines.h"

char *find_options(int argc, char **argv, flags_grep *flags, int *count,
                   char *samples) {
  int takeFlag = 0, opt = 0, tmp = 0;
  char *simple_link = samples;
  char **sample_array = calloc(1, sizeof(char *));

  while ((takeFlag = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, &opt)) !=
         -1) {
    if (takeFlag == 'e') {
      flags->e = 1;
      *count += 1;
    } else if (takeFlag == 'i') {
      flags->i = 1;
    } else if (takeFlag == 'v') {
      flags->v = 1;
    } else if (takeFlag == 'c') {
      flags->c = 1;
    } else if (takeFlag == 'l') {
      flags->l = 1;
    } else if (takeFlag == 'n') {
      flags->n = 1;
    } else if (takeFlag == 'h') {
      flags->h = 1;
    } else if (takeFlag == 's') {
      flags->s = 1;
    } else if (takeFlag == 'f') {
      flags->f = 1;
      simple_link = samples_for_flag_f(simple_link, optarg);
      char *search_term_file = argv[2];
      FILE *fp = fopen(search_term_file, "r");
      if (fp == NULL) {
        fprintf(stderr, "Error opening %s\n", search_term_file);
      }
      char search_term[MAX_SEARCH_LENGTH];
      while (fgets(search_term, MAX_SEARCH_LENGTH, fp) != NULL) {
        search_term[strcspn(search_term, "\n")] = '\0';
        for (int i = 3; i < argc; i++) {
          grep_file(argv[i], search_term);
        }
      }
    } else if (takeFlag == 'o') {
      flags->o = 1;
    } else {
      fprintf(stderr, "ERROR");
    }

    if (*count > tmp) {
      sample_array = sample_into_array(optarg, *count, sample_array);
      tmp = *count;
    }
  }

  if (!*count && !flags->f)
    sample_array = no_flags_sample_found(argv, sample_array);

  simple_link = samples_connect(sample_array, *count, simple_link, flags);

  if (*count) {
    for (int i = 0; i < *count; i++) {
      free(sample_array[i]);
    }
  } else {
    free(sample_array[0]);
  }
  free(sample_array);
  sample_array = NULL;

  return simple_link;
}

void run_utilit(int argc, char **argv, char *samples, int count,
                flags_grep *flags) {
  int count_files = 0;
  char **files = NULL;
  files = calloc(1, sizeof(char *));
  files = file_matrix(argc, argv, files, count, &count_files, flags);
  for (int i = 0; i < count_files; i++) {
    open_file_func(samples, files, i, count_files, flags);
  }
  if (!files[0] && !flags->s) printf("No files");
  for (int i = 0; i < count_files; i++) {
    free(files[i]);
  }
  free(files);
  files = NULL;
}

void grep_file(char *filename, char *search_term) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening %s\n", filename);
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    if (strstr(line, search_term) != NULL) {
      printf("%s", line);
    }
  }

  fclose(fp);
}

char **sample_into_array(char *optarg, int count, char **sample_array) {
  char **array = sample_array;
  if (count != 1) array = realloc(array, (count) * sizeof(char *));
  int len = strlen(optarg);
  array[count - 1] = calloc(len + 1, sizeof(char));
  strcpy(array[count - 1], optarg);
  return array;
}

char **no_flags_sample_found(char **argv, char **sample_array) {
  char **array = sample_array;
  int len = strlen(argv[optind]);
  *array = calloc(len + 1, sizeof(char));
  strcpy(*array, argv[optind]);
  return array;
}

char *samples_connect(char **samples, int count_samples, char *sample_link,
                      flags_grep *flags) {
  char *sample = sample_link;
  int len_sample = 0, count = 0;
  char * or = "|";
  count = samples[0] && count_samples == 0 ? 1 : count_samples;
  if (flags->f) {
    len_sample += strlen(or);
    len_sample += strlen(sample);
    sample = realloc(sample, len_sample + 1 * sizeof(char));
    strcat(sample, or);
  }
  for (int i = 0; i < count; i++) {
    len_sample += strlen(samples[i]);
    sample = realloc(sample, len_sample + 1 * sizeof(char));
    strcat(sample, samples[i]);
    if (i != count - 1 && count > 1) {
      len_sample += strlen(or);
      sample = realloc(sample, len_sample + 1 * sizeof(char));
      strcat(sample, or);
    }
  }
  return sample;
}

char **file_matrix(int argc, char **argv, char **files, int count, int *i,
                   flags_grep *flags) {
  char **file = files;
  int len, j;
  if (count == 0) {
    if (flags->f)
      j = 0;
    else
      j = 1;
  } else
    j = 0;
  for (; *i + j + optind < argc; *i += 1) {
    file = realloc(file, (*i + 1) * sizeof(char *));
    len = strlen(argv[*i + j + optind]);
    file[*i] = calloc(len + 1, sizeof(char));
    strcat(file[*i], argv[*i + j + optind]);
  }
  file = realloc(file, *i * sizeof(char *));
  return file;
}

char *samples_for_flag_f(char *sample_link, char *optarg) {
  char *copy_sample_link = sample_link;
  int size = strlen(copy_sample_link);
  char * or = "|", *sample = NULL;
  FILE *f = fopen(optarg, "r");
  if (f) {
    size_t size_sample = 0;
    while (getline(&sample, &size_sample, f) != -1) {
      if (sample[strlen(sample) - 1] == '\n' && strlen(sample) > 2) {
        sample[strlen(sample) - 1] = '\0';
      }
      if (size) {
        size += strlen(or) + size_sample;
        copy_sample_link = realloc(copy_sample_link, size * sizeof(char));
        strcat(copy_sample_link, or);
        strcat(copy_sample_link, sample);
      } else {
        size = strlen(sample);
        copy_sample_link = realloc(copy_sample_link, size * sizeof(char));
        strcpy(copy_sample_link, sample);
      }
    }
    free(sample);
    sample = NULL;
  } else {
    fprintf(stderr, "%s: no such file\n", optarg);
  }
  fclose(f);
  return copy_sample_link;
}

void open_file_func(char *sample_link, char **file, int curent_file,
                    int count_files, flags_grep *flags) {
  char *curent_string = NULL;
  int status = 0, count_string = 0, count_str_smp = 0, fl = 0, cflag = 0,
      size_line;
  regex_t expr;
  size_t size = 0;
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  FILE *f = fopen(file[curent_file], "r");
  if (f) {
    cflag = flags->i ? REG_NEWLINE | REG_EXTENDED | REG_ICASE
                     : REG_NEWLINE | REG_EXTENDED;
    if (regcomp(&expr, sample_link, cflag) == 0) {
      while ((size_line = getline(&curent_string, &size, f)) != -1) {
        status = regexec(&expr, curent_string, nmatch, pmatch, 0);
        count_string++;
        flag_filter(curent_string, status, file, curent_file, &count_str_smp,
                    count_files, &fl, count_string, size_line, expr, flags);
        if (fl == 1 && flags->l && flags->c) break;
      }
      if (flags->c) {
        if (count_files > 1 && !flags->h) printf("%s:", file[curent_file]);
        printf("%d\n", count_str_smp);
      }
      if (flags->l) {
        if (fl) printf("%s\n", file[curent_file]);
      }
      regfree(&expr);
    }
    fclose(f);
  } else {
    if (!flags->s)
      fprintf(stderr, "grep: %s: No such file or directory\n",
              file[curent_file]);
  }
  free(curent_string);
  curent_string = NULL;
}

void flag_filter(char *curent_string, int print_status, char **file,
                 int curent_file, int *count_str_smp, int count_files, int *fl,
                 int count_string, int size_line, regex_t expr,
                 flags_grep *flags) {
  if (print_status == flags->v) {
    if ((!flags->c && !flags->l) && (!flags->o || (flags->o && flags->v))) {
      print_rezult(curent_string, file, curent_file, count_string, count_files,
                   flags);
    } else if ((!flags->c && !flags->l) && (flags->o && !flags->v)) {
      flag_o_print(curent_string, file, curent_file, count_string, count_files,
                   size_line, expr, flags);
    } else if (flags->l && flags->c) {
      *fl = 1;
      *count_str_smp = 1;
    } else {
      *fl = 1;
      *count_str_smp += 1;
    }
  }
}

void print_rezult(char *curent_string, char **files, int curent_file,
                  int count_string, int count_files, flags_grep *flags) {
  if (curent_string[strlen(curent_string) - 1] == '\n') {
    if (flags->n) {
      if (count_files > 1 && !flags->h) printf("%s:", files[curent_file]);
      printf("%d:%s", count_string, curent_string);
    } else {
      if (count_files > 1 && !flags->h) printf("%s:", files[curent_file]);
      printf("%s", curent_string);
    }
  } else {
    if (flags->n) {
      if (count_files > 1 && !flags->h) printf("%s:", files[curent_file]);
      printf("%d:%s\n", count_string, curent_string);
    } else {
      if (count_files > 1 && !flags->h) printf("%s:", files[curent_file]);
      printf("%s\n", curent_string);
    }
  }
}

void flag_o_print(char *curent_string, char **file, int curent_file,
                  int count_string, int count_files, int size_line,
                  regex_t expr, flags_grep *flags) {
  regmatch_t pmatch[1];
  const size_t nmatch = 1;
  int status = regexec(&expr, curent_string, nmatch, pmatch, 0);
  int raz = 0, size_new_string = 0;
  char *only_sample = NULL, *str_without_cur_sample = NULL;
  if (!status) {
    if (pmatch->rm_eo <= size_line) {
      raz = (pmatch->rm_eo - pmatch->rm_so);
      only_sample = calloc(raz + 1, sizeof(char));
      for (int i = pmatch->rm_so, j = 0; i < pmatch->rm_eo; i++, j++) {
        only_sample[j] = curent_string[i];
      }
      print_rezult(only_sample, file, curent_file, count_string, count_files,
                   flags);
      free(only_sample);
      only_sample = NULL;
      flags->n = 0;
      if ((pmatch->rm_eo + 1) >= size_line) {
        free(only_sample);
        only_sample = NULL;
      } else {
        str_without_cur_sample = calloc(1, sizeof(char));
        for (int i = 0; i < pmatch->rm_so; i++) {
          size_new_string++;
          str_without_cur_sample = realloc(
              str_without_cur_sample, (size_new_string + 1) * sizeof(char));
          str_without_cur_sample[i] = curent_string[i];
        }
        for (int i = pmatch->rm_eo + 1; i < size_line; i++) {
          str_without_cur_sample = realloc(
              str_without_cur_sample, (size_new_string + 1) * sizeof(char));
          str_without_cur_sample[size_new_string] = curent_string[i];
          size_new_string++;
        }
        str_without_cur_sample =
            realloc(str_without_cur_sample, (size_new_string) * sizeof(char));
        flag_o_print(str_without_cur_sample, file, curent_file, count_string,
                     count_files, size_line, expr, flags);
        free(str_without_cur_sample);
        str_without_cur_sample = NULL;
      }
    }
  }
}

#endif