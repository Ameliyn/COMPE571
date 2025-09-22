#include <stdio.h>
#include <unistd.h>

main() {
    int i;
    int children[3];
    for (i = 0; i < 3; i++) {
        children[i] = fork();
    }
    printf("hello %d %d %d\n", children[0], children[1], children[2]);

}