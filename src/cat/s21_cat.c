#include "../common/cat/defines.h"
#include "../common/cat/works.h"

int main(int argc, char **argv) {
  flags_cat flags = (flags_cat){0};
  read_flag(argc, argv, &flags);
  read_file(argc, argv, &flags);
}
