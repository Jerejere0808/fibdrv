#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

static inline long long get_nanotime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

int main()
{
    char buf[500];
    // char write_buf[] = "testing writing";
    int offset = 1000; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    FILE *data = fopen("data.txt", "w");

    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    if (!data) {
        perror("Failed to open data text");
        exit(2);
    }

    /*for (int i = 0; i <= offset; i++) {
        long long t1, t2;
        lseek(fd, i, SEEK_SET);
        t1 = write(fd, write_buf, 0);
        t2 = write(fd, write_buf, 1);
        printf("%d %lld %lld\n", i, t1, t2);
        fprintf(data, "%d %lld %lld\n", i, t1, t2);
    }*/

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        long long start = get_nanotime();
        long long sz = read(fd, buf, 500);
        long long utime = get_nanotime() - start;
        // printf("%lld %lld %lld\n", sz, utime, utime - sz);
        fprintf(data, "%d %lld %lld %lld\n", i, sz, utime, utime - sz);
        printf("fib(%d): %s\n", i, buf);
    }

    /*for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 100);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%lld.\n",
               i, sz);
    }*/

    close(fd);
    fclose(data);
    return 0;
}
