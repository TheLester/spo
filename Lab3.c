#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	int fd[2];
	pipe(fd);
	if (fork() == 0) {
		int val = 0, counter, temp_var = 0;
		int arr[4];
		close(fd[0]); // writing only, so close read-descriptor.
		while (1) {
			val++;
			if (temp_var == 4) {
				arr[temp_var] = val;
				write(fd[1], &arr, 16);
				printf("Process2(%d) send values: %d,%d,%d,%d\n", getpid(),
						arr[0], arr[1], arr[2], arr[3]);
				temp_var = 0;
				sleep(1);
			}
			arr[temp_var] = val;
			temp_var++;
		}

	} else {
		int arr2[4];
		close(fd[1]); //reading only, so close the write-descriptor
		while (1) {
			sleep(1);
			read(fd[0], &arr2, 16);
			printf("Process1(%d) received values: %d,%d,%d,%d\n", getpid(),
					arr2[0], arr2[1], arr2[2], arr2[3]);

		}
	}
	return 0;
}

