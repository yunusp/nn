#include <stddef.h>
#include <stdlib.h>
#ifndef NN_H_
#define NN_H_

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif // NN_ASSERT

#ifndef NN_MALLOC
#define NN_MALLOC malloc
#endif // NN_MALLOC

typedef struct {
  size_t rows;
  size_t cols;
  float *es;
} Mat;

Mat mat_alloc(size_t rows, size_t cols);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_print(Mat a);

#endif // NN_H_

#define NN_IMPLEMENTATION
// NOLINTBEGIN
#ifdef NN_IMPLEMENTATION

Mat mat_alloc(size_t rows, size_t cols) {
  Mat m;
  m.rows = rows;
  m.cols = cols;
  m.es = (float *)NN_MALLOC(sizeof(*m.es) * rows * cols);
  assert(m.es != NULL);
  return m;
}

void mat_dot(Mat dst, Mat a, Mat b) {
  (void)dst;
  (void)a;
  (void)b;
}

void mat_sum(Mat dst, Mat a) {
  (void)dst;
  (void)a;
}

void mat_print(Mat a) { (void)a; }

#endif // NN_IMPLEMENTATION
// NOLINTEND
