#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <time.h>
#define MAX 256

int main()
{  
   
  
  int32_t i,j,k,size,l1,h1,l2,h2;
  int32_t * strtbuff = (int*)malloc(sizeof(int32_t)*MAX);
  int32_t * endbuff = (int*)malloc(sizeof(int32_t)*MAX);

//Initiating the process 
	for (int c = 0; c <MAX; c++) {
    		strtbuff[c]= rand() % 100 + 1;
		}
	printf("Unsorted list is : ");
	for( i = 0 ; i<MAX ; i++){
		printf("%d ", strtbuff[i]);
		}
    printf("\n");	

    int sys_status = 0;

    sys_status = syscall(398,strtbuff,MAX,endbuff);
         
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

    printf("Current process ID: %ld \n", syscall(39));
    printf("Current user ID: %ld \n", syscall(102));
    
    return (0);
}
