#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int x;

    x = 100;

    fork();

    x++;

    printf("%d\n", x);
}
