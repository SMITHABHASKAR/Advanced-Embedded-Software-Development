#include<stdio.h>
#include<stdlib.h>
int main()
{
	
	FILE *fp; //Pointer for file operations
	char in1;
	char c,s[50];
	char *str,*sting;

	printf("Assignment 2 is being executed where we learn how to use buildroot and system calls!");//Task 1
	fp=fopen("Problem 2.txt","w"); //Task 2 & 3
	printf("\n \r Enter the character to be written to the file \n");
	while((in1=getchar())!='\n')
	{
		putc(in1,fp);
	}

	fclose(fp);
	
	fp=fopen("Problem 2.txt","a"); //Opening in append mode
	str=(char *) malloc (50);
	printf("\nEnter a string");
	gets(str);
	fprintf(fp,"\n string entered by the user:%s",str);
	fclose(fp);

	fp=fopen("Problem 2.txt","r");
	c=getc(fp);
	printf("\n The first character from file %c ",c);
	sting=fgets(s,50,fp);
	printf("\n The string from file %s \n",sting);
	fclose(fp);
	return 0;
}


