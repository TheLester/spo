#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SHMKEY 77
int p1(int signum);
int p2(int signum);

int pid2, pid1;
int shmid;
int *shmem;
int myNum[6] = { 5, 5, 8, 5, 4, 5 };
int main(void) {
	pid1 = getpid();
	signal(SIGUSR1, p1);
	pid2 = fork();
//pid2
	if (pid2 == 0) {
		pid2 = getpid();
		signal(SIGUSR1, p2);
		int i;
		int A[6];
		for (i = 0; i < 6; i++)
			A[i] = myNum[i];
		pause();
		shmid = shmget(SHMKEY, (sizeof(int)), 0666);
		shmem = (int *) shmat(shmid, 0, 0);
		printf("P2 -A = {%d,%d,%d,%d,%d,%d}\n", A[0], A[1], A[2], A[3], A[4],
				A[5]);
		//int i;
		*shmem = &A;
		printf("p1 addr=0x%x\n", &A);
		sleep(1);
		kill(pid1, SIGUSR1);
		pause();
		printf("P2-Step mode\n");
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		sleep(2);
		kill(pid2, SIGUSR1);

		printf("P2 -new A = {%d,%d,%d,%d,%d,%d}\n", A[0], A[1], A[2], A[3],
				A[4], A[5]);
		sleep(1);
		kill(pid1, SIGUSR1);
		exit(0);
	}
//pid1
	else if (pid2 > 0) {
		int memaddr[6];
		shmid = shmget(SHMKEY, (sizeof(int)), 0666 | IPC_CREAT);
		shmem = (int *) shmat(shmid, 0, 0);
		sleep(1);
		kill(pid2, SIGUSR1);
		pause();
		(*memaddr) = *shmem;
		printf("p2 addr=0x%x\n", &memaddr);
		sleep(1);
		kill(pid2, SIGUSR1);
		sleep(3);
		printf("P1 -trace ,changing А\n");
		int j;
		for (j = 0; j < 6; j++) {
			if (!(myNum[j] % 2)) {
				if (ptrace(PTRACE_POKETEXT, pid2, &memaddr[j], myNum[j] / 2)
						!= 0)
					printf("Error : %s\n", strerror(errno));
			} else {
				if (ptrace(PTRACE_POKETEXT, pid2, &memaddr[j], myNum[j]) != 0)
					printf("Error : %s\n", strerror(errno));
			}
		}
		ptrace(PTRACE_DETACH, pid2, NULL, NULL);
		pause();
		return 0;
	}
}
int p1(int signum) {
	printf("P1 got a signal\n");
	return 0;
}

int p2(int signum) {
	printf("P2 got a signal\n");
	return 0;
}

