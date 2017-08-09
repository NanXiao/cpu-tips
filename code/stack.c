#include <stdio.h>

void fun1(void)
{
	double a = 111.111;
	return;
}

void fun2(void)
{
	fun1();
}

int main(void)
{
	fun2();
	return 0;
}