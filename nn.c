#include <stdlib.h>
#include <time.h>
#define NN_IMPLEMENTATION
#include "nn.h"

int main() {
  srand(time(0));
  Mat a = mat_alloc(2, 2);
  mat_fill(a, 1);
  Mat b = mat_alloc(2, 2);
  mat_fill(b, 3);
  mat_sum(a, b);
  mat_print(a);
  return 0;
}
