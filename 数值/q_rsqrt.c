#include <stdio.h>

float Q_rsqrt(float number) {
    long i;
    float x, y;
    const float threehalfs = 1.5f;

    x = number * 0.5f;
    y = number;
    i = *(long*) &y;
    i = 0x5f3759df - ( i >> 1 );
    y = *(float*) &i;
    y = y * (threehalfs - (x * y * y));
    y = y * (threehalfs - (x * y * y));
    return y;
}

int main(void) {
    float x = 0.0;
    scanf("%f", &x);
    printf("rsqrt: %f\n", Q_rsqrt(x));
    return 0;
}