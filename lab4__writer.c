#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
	int fd, i;
	char *myfifo = "/tmp/myfifo";
	/* create the FIFO (named pipe) */
	mkfifo(myfifo, 0666);

	/* write "Hi" to the FIFO */
	fd = open(myfifo, O_WRONLY);
	for (i = 1; i < 6; i++) {
		write(fd, &i, 4);
		if (i != 5)
			sleep(5);
	}
	close(fd);
	/* remove the FIFO */
	unlink(myfifo);

	return 0;
}
