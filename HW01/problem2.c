#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		puts("wrong number of arguments");
		exit(0);
	}
	
	char *ch;
	ch = argv[1];
	countlen(ch);
	return(0);
}
countlen(char *ptr)
{
	int j = 0;
	while ( ptr[j] != '\0')
	{
		j++;
	}
	printf("%d\n", j);
}
