#include <stdio.h>
#include <stdlib.h>
int countlen(char *ptr);
int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		puts("wrong number of arguments");
		exit(0);
	}
	
	char *ch;
	long num = 0;
	ch = argv[1];
	num = countlen(ch);
	printf("%lu\n", num);
	return(0);
}
int countlen(char *ptr)
{
	int j = 0;
	while ( ptr[j] != '\0')
	{
		j++;
	}
	return(j);
}
