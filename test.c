#include <stdio.h>

int main() {
    FILE *file = fopen("logo.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    fclose(file);
    return 0;
}