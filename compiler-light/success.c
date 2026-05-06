#include <stdio.h>
#include <stdlib.h>

#define MAX 200
#define FACTOR 3
#define LABEL "done"

int base;
float ratio;
int steps = 0;

int multiply(int a, int b) {
    int result;
    result = a * b;
    return result;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) {
        val = lo;
    } else {
        if (val > hi) {
            val = hi;
        }
    }
    return val;
}

int main() {
    int x;
    int y;
    int z;
    int total;
    float avg;

    x = 4;
    y = 7;
    z = multiply(x, y);

    total = z + FACTOR * 2;
    total = clamp(total, 0, MAX);

    while (steps < total) {
        steps = steps + 1;
    }

    if (steps >= MAX) {
        total = total - 1;
    } else {
        total = total + 1;
    }

    return total;
}
