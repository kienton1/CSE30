#include <stdio.h>

int main()
{
	int a = 3;
	int b = 4;
	int *p1 = &a;
	int **p2 = &p1;
	*p1 = 2;
	*p2 = NULL;
	printf("%d", *p1);
	printf("%p", *p2);
	return 0;
}
