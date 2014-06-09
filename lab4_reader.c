#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    int recieved_val;

    /* open, read, and display the message from the FIFO */
    fd = open("/tmp/myfifo", O_RDONLY);
    while ( read(fd, &recieved_val, 4) != 0)
           printf("Received: %d\n", recieved_val);
    close(fd);

    return 0;
}
