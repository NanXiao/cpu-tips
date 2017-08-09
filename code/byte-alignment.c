#include <stdlib.h>

int main(void)
{
	char *p1 = calloc(10, sizeof(char));
	int *p2 = (int *)(p1 + 1);
	*p2 = 0x0A0B0C0D;
	return 0;
}