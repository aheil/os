// compile using gcc -pthread fork3.c
// -pthread Adds support for multithreading with the pthreads library. This option sets flags for both the preprocessor and linker.

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main() {

    int fd = open("foo.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    int fid = fork();

    if (fid == 0) {
        // child process
        sleep(2); // for testing
        printf("hello\n");  
        lseek(fd, 0, SEEK_SET);
        char x = '1';
        write(fd, &x, 1);
    } else {
        // parent process
        char buf[2]; // min 2 due to atoi security issues
        buf[1] = '\0';
        int s = 0;
        while (s < 1) {
            lseek(fd, 0, SEEK_SET);
            read(fd, &buf, 1);
            s = atoi(buf);
            //printf("%d\n", s);
            //printf("%c\n", buf[0]);
         }
        printf("goodbye\n");
        close(fd);
    }
}
