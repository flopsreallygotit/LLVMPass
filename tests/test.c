#include <stdio.h>

int Compute(int a, int b) {
    int sum = 0;
    for (int i = 0; i < a; i++) {
        sum = sum + b;
    }

    return sum;
}

int main(void) {
    int result = Compute(4, 7);
    printf("%d\n", result);
    
    return 1;
}
