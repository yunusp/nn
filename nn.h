#include <stddef.h>
#include <stdio.h>
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

#define MAT_AT(m, i, j) (m).es[(i) * (m).cols + (j)]

float rand_float(void);

Mat mat_alloc(size_t rows, size_t cols);
void mat_rand(Mat m, float low, float high);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_print(Mat a);

#endif // NN_H_

#define NN_IMPLEMENTATION

// NOLINTBEGIN(misc-definitions-in-headers)
#ifdef NN_IMPLEMENTATION

float rand_float(void) { return (float)rand() / (float)RAND_MAX; }

Mat mat_alloc(size_t rows, size_t cols) {
  Mat m;
  m.rows = rows;
  m.cols = cols;
  m.es = (float *)NN_MALLOC(sizeof(*m.es) * rows * cols);
  assert(m.es != NULL);
  return m;
}

void mat_rand(Mat m, float low, float high) {
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      MAT_AT(m, i, j) = rand_float() * (high - low) + low;
    }
  }
}

void mat_fill(Mat m, float n) {
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      MAT_AT(m, i, j) = n;
    }
  }
}

void mat_dot(Mat dst, Mat a, Mat b) {
  (void)dst;
  (void)a;
  (void)b;
}

void mat_sum(Mat dst, Mat a) {
  NN_ASSERT(dst.rows == a.rows);
  NN_ASSERT(dst.cols == a.cols);
  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      MAT_AT(dst, i, j) += MAT_AT(a, i, j);
    }
  }
}

void mat_print(Mat m) {
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      printf("%f ", MAT_AT(m, i, j));
    }
    printf("\n");
  }
}

#endif // NN_IMPLEMENTATION
// NOLINTEND(misc-definitions-in-headers)
