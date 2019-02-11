#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	
	FILE *fp; //Pointer for file operations
	char in1;
	char c,s[50];
	char *str,*sting;

	printf("Assignment 2 is being executed where we learn how to use buildroot and system calls!");//Task 1
	fp=fopen("Problem 2.txt","w"); //Task 2 & 3
	
	if(fp==NULL)
	{
		printf("File hasn't been created \n\r");
	}
	printf("\n \r Enter the character to be written to the file \n");
	while((in1=getchar())!='\n')
	{
		putc(in1,fp);
	}

	fclose(fp);
	
	fp=fopen("Problem 2.txt","a"); //Opening in append mode
	
	fflush(fp);
	fclose(fp);

	fp=fopen("Problem 2.txt","r");
	c=getc(fp);
	printf("\n The first character from file %c ",c);
	char * string1=(char *)malloc(sizeof(char)*50);
	char string2[100];
	string1 =fgets(string2,50,fp);
	printf("\n The string from file %s \n",string1);
	fclose(fp);
	return 0;
}


