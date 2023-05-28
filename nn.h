#include <math.h>
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

float rand_float(void);
float sigmoidf(float x);
#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

typedef struct {
  size_t rows;
  size_t cols;
  size_t stride;
  float *es;
} Mat;

#define MAT_AT(m, i, j) (m).es[(i) * (m).stride + (j)]
#define MAT_PRINT(m) mat_print(m, #m, 0)

Mat mat_alloc(size_t rows, size_t cols);
void mat_rand(Mat m, float low, float high);
Mat mat_row(Mat m, size_t row);
void mat_copy(Mat dst, Mat src);
void mat_fill(Mat m, float n);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_sig(Mat m);
void mat_print(Mat a, const char *name, size_t padding);

typedef struct {
  size_t count;
  Mat *ws;
  Mat *bs;
  Mat *as; // This is always count + 1
} NN;

NN nn_alloc(size_t *arch, size_t arch_count);
void nn_print(NN nn, const char *name);
#define NN_PRINT(nn) nn_print(nn, #nn)
#endif // NN_H_

#define NN_IMPLEMENTATION

// NOLINTBEGIN(misc-definitions-in-headers)
#ifdef NN_IMPLEMENTATION

float rand_float(void) { return (float)rand() / (float)RAND_MAX; }
float sigmoidf(float x) { return 1.f / (1.f + expf(-x)); }

Mat mat_alloc(size_t rows, size_t cols) {
  Mat m;
  m.rows = rows;
  m.cols = cols;
  m.stride = cols;
  m.es = (float *)NN_MALLOC(sizeof(*m.es) * rows * cols);
  NN_ASSERT(m.es != NULL);
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
  NN_ASSERT(a.cols == b.rows);
  size_t n = a.cols;
  NN_ASSERT(dst.rows == a.rows);
  NN_ASSERT(dst.cols == b.cols);

  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      MAT_AT(dst, i, j) = 0;
      for (size_t k = 0; k < n; ++k) {
        MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
      }
    }
  }
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

Mat mat_row(Mat m, size_t row) {
  return (Mat){
      .rows = 1,
      .cols = m.cols,
      .stride = m.stride,
      .es = &MAT_AT(m, row, 0),
  };
}

void mat_copy(Mat dst, Mat src) {
  NN_ASSERT(dst.rows == src.rows);
  NN_ASSERT(dst.cols == src.cols);

  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      MAT_AT(dst, i, j) = MAT_AT(src, i, j);
    }
  }
}

void mat_sig(Mat m) {
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      MAT_AT(m, i, j) = sigmoidf(MAT_AT(m, i, j));
    }
  }
}

void mat_print(Mat m, const char *name, size_t padding) {
  printf("%*s", (int)padding, "");
  printf("%*s%s = [\n", (int)padding, "", name);
  for (size_t i = 0; i < m.rows; ++i) {
    printf("%*s", (int)padding, "");
    for (size_t j = 0; j < m.cols; ++j) {
      printf("%f ", MAT_AT(m, i, j));
    }
    printf("\n");
  }
  printf("%*s]\n", (int)padding, "");
}

NN nn_alloc(size_t *arch, size_t arch_count) {
  NN_ASSERT(arch_count > 0);
  NN nn;
  nn.count = arch_count - 1;
  nn.ws = (Mat *)NN_MALLOC(sizeof(*nn.ws) * nn.count);
  NN_ASSERT(nn.ws != NULL);
  nn.bs = (Mat *)NN_MALLOC(sizeof(*nn.bs) * nn.count);
  NN_ASSERT(nn.bs != NULL);
  nn.as = (Mat *)NN_MALLOC(sizeof(*nn.as) * (nn.count + 1));
  NN_ASSERT(nn.as != NULL);

  nn.as[0] = mat_alloc(1, arch[0]);
  for (size_t i = 1; i < arch_count; ++i) {
    nn.ws[i - 1] = mat_alloc(nn.as[i - 1].cols, arch[i]);
    nn.bs[i - 1] = mat_alloc(1, arch[i]);
    nn.as[i] = mat_alloc(1, arch[i]);
  }

  return nn;
}
void nn_print(NN nn, const char *name) {
  char buf[256];
  printf("%s = [\n", name);
  Mat *ws = nn.ws;
  Mat *bs = nn.bs;
  for (size_t i = 0; i < nn.count; ++i) {
    snprintf(buf, sizeof(buf), "ws%zu", i);
    mat_print(ws[i], buf, 4);
    snprintf(buf, sizeof(buf), "bs%zu", i);
    mat_print(bs[i], buf, 4);
  }
  printf("]\n");
}
#endif // NN_IMPLEMENTATION
// NOLINTEND(misc-definitions-in-headers)
