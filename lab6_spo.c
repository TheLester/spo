/*
 ============================================================================
 Name        : lab6_spo.c
 Author      : Lester
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/sem.h>

int pid2;
int msgid;
key_t key;
int x; //for waiting
struct pid_msgbuf {
	long mtype;
	int pid;
};
int main(void) {

	key = ftok("/home/lester/soft/test", 'S');
	msgid = msgget(key, 0666 | IPC_CREAT);
	int size = sizeof(struct pid_msgbuf) - sizeof(long);
	
	pid2 = fork();
	if (pid2 == 0) { //process2
		struct pid_msgbuf pmb = { 2, getpid() };
		msgsnd(msgid, &pmb, size, 2);
		struct pid_msgbuf pmb_recv;
		sleep(1);
		msgrcv(msgid, &pmb_recv, size, 1, 0);
		printf("%d \n", pmb_recv.pid);
		exit(0);
	}
	pid3 == fork();
	if(pid3 == 0){
	struct pid_msgbuf pmb_recv;
	struct pid_msgbuf pmb = { 1, getpid() };
	msgsnd(msgid, &pmb, sizeof(int), 1);

	msgrcv(msgid, &pmb_recv, size, 2, 0);
	printf(" %d \n", pmb_recv.pid);
	wait(NULL);
	msgctl(msgid, IPC_RMID, NULL);
	exit(0);
}
	sleep(2);
	wait(NULL);
	exit(0);
}
