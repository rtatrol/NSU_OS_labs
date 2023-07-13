#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int globalInit = 10;
int globalNotInit;
const int globalConst;
const char str[] = "Hello, world";

void funcA()
{
	int fLocal;
	static int fStat;
	const int fConst;

	printf("local in function: %p\n", &fLocal);
	printf("static in function: %p\n", &fStat);
	printf("const in function: %p\n", &fConst);
	printf("global init: %p\n", &globalInit);
	printf("global not init: %p\n", &globalNotInit);
	printf("global const: %p\n", &globalConst);
}

int *funcD()
{
	int a = 10;
	return &a;
}

void writePhrase(char *buf)
{
	strcpy(buf, str);
}

void funcE()
{
	char *buf = (char *)malloc(800);
	writePhrase(buf);
	printf("%s\n", buf);
	free(buf);
	printf("%s\n", buf);
	char *newBuf = (char *)malloc(800);
	writePhrase(newBuf);
	printf("%s\n", newBuf);
	newBuf += 400;
	free(newBuf);
	printf("%s\n", newBuf);
}

void funcH()
{

	char *env_var = "my_war";

	printf("Initial value of %s: %s\n", env_var, getenv(env_var));

	setenv(env_var, "New Value", 1);

	printf("Modified value of %s: %s\n", env_var, getenv(env_var));
}

int main()
{

	// task a
	// printf("pid: %d\n", getpid());
	// funcA();
	// sleep(20);

	// task d
	//  int* a = funcD();
	//  printf("return local var: %d\n", *a);

	// task e
	// funcE();

	// task h
	funcH();
}
