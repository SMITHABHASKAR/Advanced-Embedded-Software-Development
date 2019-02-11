#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX 256

int main()
{
	int32_t i,j,k,size,l1,h1,l2,h2;
	int32_t * strtbuff = (int*)malloc(sizeof(int32_t)*MAX);
    	int32_t * endbuff = (int*)malloc(sizeof(int32_t)*MAX);

	for (int c = 0; c <MAX; c++) {
    		strtbuff[c]= rand() % 100 + 1;
		}
	printf("Unsorted list is : ");
	for( i = 0 ; i<MAX ; i++){
		printf("%d ", strtbuff[i]);
		}

	/*l1 lower bound of first pair and so on*/
	for(size=1; size < MAX; size=size*2 )
	{
		l1=0;
		k=0;  /*Index for endbuff strtbuff*/
		while( l1+size < MAX)
		{
			h1=l1+size-1;
			l2=h1+1;
			h2=l2+size-1;
			/*h2 exceeds the limlt of strtbuff */
			if( h2>=MAX ) 
				h2=MAX-1;
			
			/*Merge the two pairs with lower limits l1 and l2*/
			i=l1;
			j=l2;
			
			while(i<=h1 && j<=h2 )
			{
				if( strtbuff[j] <= strtbuff[i] )
					endbuff[k++]=strtbuff[i++];
				else
					endbuff[k++]=strtbuff[j++];
			}
			
			while(i<=h1)
				endbuff[k++]=strtbuff[i++];
			while(j<=h2)
				endbuff[k++]=strtbuff[j++];
			/**Merging completed**/
			/*Take the next two pairs for merging */
			l1=h2+1; 
		}/*End of while*/

		/*any pair left */
		for(i=l1; k<MAX; i++) 
			endbuff[k++]=strtbuff[i];

		for(i=0;i<MAX;i++)
			strtbuff[i]=endbuff[i];

		
	}/*End of for loop */
	
	printf("\nSorted list is :\n");
	for( i = 0 ; i<MAX ; i++)
		printf("%d ", strtbuff[i]);
	
	printf("\n");
	
	return 0;
}

