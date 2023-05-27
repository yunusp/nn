#include <stddef.h>
#ifndef NN_H_
#define NN_H_

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

#ifdef NN_IMPLEMENTATION

Mat mat_alloc()

#endif // NN_IMPLEMENTATION
