/*
 ============================================================================
 Name        : lab_8.c
 Author      : Lester
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
void main() {
	jmp_buf jump;
	char str[255];
	int jmp = setjmp(jump);
	printf("Type your string (only 5 symbols):\n");
	scanf("%s", &str);
	if (strlen(str) != 5) {
		printf("Not 5 symbols. \n");
		longjmp(jump, 1);
	}
	printf("Your message - %s \n", str);
}
