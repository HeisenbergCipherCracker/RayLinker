#include <stdio.h>

int main() {
    int rows = 9;
    int cols = 9;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i % 3 == 0) {
                printf("___ ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }

    return 0;
}