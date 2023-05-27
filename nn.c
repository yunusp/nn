#define NN_IMPLEMENTATION
#include "nn.h"

int main() {
  Mat m = mat_alloc(2, 2);
  mat_rand(m, 0.0f, 1.0f);
  mat_print(m);
  return 0;
}
