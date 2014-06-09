#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

#define	SEMKEY	65

void process1();
void process2();
void process3();
void process4();
int num_of_digits(int num);
int p1(int signum);
int p4(int signum);

int semid;
int A, B, C;
int received_A, received_B, received_C;
int pid1, pid2, pid3, pid4;
int msgid;
key_t key;
struct pid_msgbuf {
	long mtype;
	int pid;
};

struct sembuf sop;
FILE *file = NULL;

void v(int sid) {
	sop.sem_op = 1;
	semop(sid, &sop, 1);
}
void p(int sid) {
	sop.sem_op = -1;
	semop(sid, &sop, 1);
}
int p1(int signum) {
	printf("P1 got a signal\n");
	return 0;
}
int p4(int signum) {
	printf("P4 got a signal\n");
	return 0;
}
void main(void) {
	key = ftok("/home/lester/soft/test", 'S');
	msgid = msgget(key, 0666 | IPC_CREAT);

	short arg[1];
	arg[0] = 0;
	semid = semget(SEMKEY, 1, 0777 | IPC_CREAT);
	semctl(semid, 0, SETALL, arg);
	sop.sem_num = 0;
	sop.sem_flg = 0;
	file = fopen("res.txt", "w+");
	pid1 = getpid();
	printf("Process p1, pid = %d:\n", pid1);
	pid2 = fork();
	if (pid2 == 0) {
		printf("Process p2, pid = %d:\n", getpid());
		pid3 = fork();
		if (pid3 == 0) {
			printf("Process p3, pid = %d:\n", getpid());
			process3();
		} else if (pid3 > 0) {			// в pid2
			process2();
		}
	} else if (pid2 > 0) {				// в pid1
		pid4 = fork();
		if (pid4 == 0) {				// в pid4
			struct pid_msgbuf pmb = { 1, getpid() };
			msgsnd(msgid, &pmb, sizeof(struct pid_msgbuf) - sizeof(long), 1);//отправка ид 4 процесса
			signal(SIGUSR2, p4);
			printf("Process p4, pid = %d:\n", getpid());
			process4();
		} else if (pid4 > 0) {
			signal(SIGUSR1, p1);
			process1();
		}
	}
	wait(NULL);
	msgctl(msgid, IPC_RMID, NULL);
	semctl(semid, IPC_RMID, NULL);
	exit(0);
}
void process4() {
	pause();
	FILE *fp4 = fopen("4.txt", "r");
	fscanf(fp4, "%d", &C);
	fprintf(file, "%d", C);
	fseek(file, -num_of_digits(C), SEEK_CUR);//сдвиг курсора на количество цифр числа
	printf("P4 Sent C-%d\n", C);
	v(semid);
	exit(0);
}
void process2() {
	sleep(1);
	FILE *fp2 = fopen("2.txt", "r");
	fscanf(fp2, "%d", &A);
	fprintf(file, "%d", A);
	printf("P2 Sent A-%d\n", A);
	fseek(file, -num_of_digits(A), SEEK_CUR);			//отправка ид 4 процесса
	v(semid);
	wait(NULL);
	exit(0);
}
void process3() {
	struct pid_msgbuf pmb_recv;
	p(semid);
	fscanf(file, "%d", &received_A);
	fseek(file, 0, SEEK_END);				//сдвиг курсора на конец записи
	printf("P3 Got A:%d\n", received_A);
	kill(pid1, SIGUSR1);
	p(semid);
	fscanf(file, "%d", &received_B);
	fseek(file, 0, SEEK_END);
	printf("P3 Got B:%d\n", received_B);
	msgrcv(msgid, &pmb_recv, sizeof(struct pid_msgbuf) - sizeof(long), 1, 0);//прием ид 4 процесса
	kill(pmb_recv.pid, SIGUSR2);
	p(semid);
	fscanf(file, "%d", &received_C);
	fseek(file, 0, SEEK_END);
	printf("P3 Got C:%d\n", received_C);
	printf("Result of (A+B)*C=%d\n", (received_A + received_B) * received_C);//вывод результата на консоль
	exit(0);
}
void process1() {
	pause();
	printf("Type B:\n");
	scanf("%d", &B);
	fprintf(file, "%d", B);
	printf("P1 Sent B- %d", B);
	fseek(file, -num_of_digits(B), SEEK_CUR);
	v(semid);
}
int num_of_digits(int num) {
	int count = 0;
	while (num != 0) {
		num /= 10;
		++count;
	}
	return count;
}
