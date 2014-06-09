#include <stdio.h>
#include <fcntl.h>
int fd;
int countlines();
void main(int argc, char *argv[]) {
	if (argc < 2)
		puts("Format: rw filename");
	else {
		fd = open(argv[1], O_CREAT | O_RDWR);
		write(fd, countlines(), sizeof(int));
		close(fd);
	}
}

int countlines() {
	int ch, lines = 0;
	FILE *fp = fdopen(fd, "r+");

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n')
			lines++;
	}
	fseek(fp, 0, SEEK_SET);
	fprintf(fp,"%d",lines);
	fclose(fp);
	return lines;
}
