#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float train[][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 6}, {4, 8}};
#define train_count (sizeof(train) / sizeof(train[0]))

float rand_float() { return (float)rand() / (float)RAND_MAX; }

// cost function. Square of errors.
float cost(float w, float b) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x = train[i][0];
        float y = x * w + b;
        float d = y - train[i][1];
        result += d * d;
    }
    result /= train_count;
    return result;
}

int main() {
    srand(time(0));
    // srand(69);
    const float eps = 1e-3;
    const float rate = 1e-3;
    float b = 5.0f;

    float w = rand_float() * 10;

    for (size_t i = 0; i < 80000; i++) {
        float c = cost(w, b);
        float dw = (cost(w + eps, b) - c) / eps;  // derivative limit formula
        float db = (cost(w, b + eps) - c) / eps;  // derivative limit formula
        printf("%f\n", cost(w, b));

        w -= rate * dw;
        b -= rate * db;
    }
    puts("===================================");
    printf("Weight: %f, Bias: %f, Add: %f\n", w, b, w + b);
}
