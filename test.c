#include <stdio.h>

int Compute(int a, int b) {
    int x = a + b;
    int y = x * 2;
    int z = y - a;
    
    return z;
}

int main(void) {
    int result = Compute(3, 5);
    printf("%d\n", result);

    return 0;
}