#include <stdio.h>
#include <stdlib.h>

extern void strcopy(void *, void*);

void main(void)
{
	char a[100];
	strcopy(a, "Hello World");
	printf("%s\n",a);
}
