#include "../common/grep/defines.h"
#include "../common/grep/works.h"

int main(int argc, char **argv) {
  flags_grep flags = (flags_grep){0};
  int sample_count = 0;
  char *samples = NULL;
  if (argc < 3) {
    exit(0);
  } else {
    samples = calloc(1, sizeof(char));
    samples = find_options(argc, argv, &flags, &sample_count, samples);
    run_utilit(argc, argv, samples, sample_count, &flags);
    free(samples);
    samples = NULL;
  }

  return 0;
}
