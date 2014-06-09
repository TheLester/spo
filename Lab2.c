#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
extern int f1(int), f2(int);
int i, j, fd, pid1, pid2;
unsigned char pi1 = '1', pi2 = '2';
void main() {
	setpgrp();
	mode_t mode = S_IRUSR | S_IWUSR;
	pid1 = getpid();
	if ((fd = open("lab2.txt", O_WRONLY | O_CREAT, mode)) < 0) {
		perror("open error");
		exit(1);
	}

	if ((pid2 = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	signal(SIGUSR1, f1);
	if (pid2 == 0) {
		signal(SIGUSR2, f2);
		kill(pid1, SIGUSR1);
		for (;;)
			;
	}
	for (;;)
		;
	exit(0);
}
int f1(int signum) {
	signal(SIGUSR1, f1);
	if (i < 12) {
		i++;
		write(fd, &pi1, 1);
		kill(pid2, SIGUSR2);
	} else
		kill(0, SIGKILL);
	return 0;
}
int f2(int signum) {
	signal(SIGUSR2, f2);
	if (j < 12) {
		j++;
		write(fd, &pi2, 1);
		kill(pid1, SIGUSR1);
	}
	return 0;
}
