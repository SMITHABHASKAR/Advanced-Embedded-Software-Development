#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#define MAX 256

int main()
{  
   FILE *fp;
   fp=fopen("crontext.txt","a+");
  char buffer[40];
  struct timeval time;
  int32_t i,j,k,size,l1,h1,l2,h2;
  int32_t * strtbuff = (int*)malloc(sizeof(int32_t)*MAX);
  int32_t * endbuff = (int*)malloc(sizeof(int32_t)*MAX);

//Initiating the process 
	for (int c = 0; c <MAX; c++) {
    		strtbuff[c]= rand() % 100 + 1;
		}
	fprintf(fp,"Unsorted list is : ");
	for( i = 0 ; i<MAX ; i++){
		printf("%d ", strtbuff[i]);
		}
    printf("\n");	

    int sys_status = 0;
	//System call for success case
    sys_status = syscall(398,strtbuff,MAX,endbuff);
         
    if(sys_status == 0) 
    {
	fprintf(fp,"SORTED LIST:\n\r");
	for(i = 0; i < MAX; i++)
	{
		fprintf(fp,"%d, ",endbuff[i]);
	}
	printf("\n");
	printf("Sorting succssful\n");		
    }	
    else 
    {
         printf("System call failed:%ld \n",sys_status);
    }

	//System call for fail case when length is negative
	
	sys_status = syscall(398,strtbuff,-5,endbuff);
	
	if(sys_status == 0) 
    	{
	printf("SORTED LIST:\n");
	for(i = 0; i < MAX; i++)
	{
		printf("%d, ",endbuff[i]);
	}
	printf("\n");
	printf("Sorting succssful\n");		
    	}	
    	else 
    	{
         printf("System call failed:%ld \n",sys_status);
    	}

	//System call for fail case when system call is invalid
	
	sys_status = syscall(400,strtbuff,MAX,endbuff);
	
	if(sys_status == 0) 
    	{
	printf("SORTED LIST:\n");
	for(i = 0; i < MAX; i++)
	{
		printf("%d, ",endbuff[i]);
	}
	printf("\n");
	printf("Sorting succssful\n");		
    	}	
    	else 
    	{
         printf("System call failed:%ld \n",sys_status);
    	}

    fprintf(fp,"Process ID of the calling process= %ld \n", syscall(20));
    fprintf(fp,"Real user ID= %d \n", syscall(24));
    fprintf(fp,"Effective user ID= %d\n\r",syscall(49));
    fprintf(fp,"Current date/time =%d\n\r",syscall(78));

    gettimeofday(&time, NULL);

    strftime(buffer, 40, "Current date/time: %m-%d-%Y/%T",localtime(&time.tv_sec)); 
    fprintf(fp,"%s\n",buffer);
    
    fclose(fp);

    
    return (0);
}
