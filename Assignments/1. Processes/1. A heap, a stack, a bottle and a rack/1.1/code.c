#include <stdio.h>

int main() {

    foo:

    printf("The code: %p\n", &&foo);
    fgetc(stdin);
    return 0;

}