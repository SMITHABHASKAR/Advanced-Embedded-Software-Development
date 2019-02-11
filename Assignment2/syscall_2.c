#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/errno.h>

SYSCALL_DEFINE3(mergefinal, int *, strtbuff, int , MAX , int * , endbuff)
{
    printk("Entering system call\n");
    int i,j,temp = 0;
    int *kBuff = NULL;
    int length;
    length = (sizeof(int)*MAX);

    kBuff = (int*)kmalloc( length, GFP_KERNEL);
    tempbuff = (int*)kmalloc(length, GFP_KERNEL);
    
    if(kBuff == NULL)
    {
          printk(KERN_ERR "Kernel buffer not created properly\n");
          kfree(kBuff);
          return -ENOMEM;
    }

    if (strtbuff == NULL || endbuff == NULL)
    {
          printk(KERN_ERR "Null Pointer passed \n");
          return -EFAULT;
    }

    if( MAX < 256 )
    {
       printk(KERN_ERR "Buffer must have at least 256 elements\n");
       return -EINVAL;
    }

    if( length < 0 )
    {
       printk(KERN_ERR "Invalid length\n");
       return -EINVAL;
    }

    if(copy_from_user(kBuff, strtbuff, length))
    {
          printk(KERN_ERR "Not all data was copied from user\n");
          return -EFAULT;
    }

    printk(KERN_INFO "No errors. Proceeding with sort");
    
   for(int size=1; size < MAX; size=size*2 )
	{
		int l1=0;
		int k=0;  /*Index for endbuff strtbuff*/
		while( l1+size < MAX)
		{
			int h1=l1+size-1;
			int l2=h1+1;
			int h2=l2+size-1;
			/*h2 exceeds the limlt of strtbuff */
			if( h2>=MAX ) 
				h2=MAX-1;
			
			/*Merge the two pairs with lower limits l1 and l2*/
			int i=l1;
			int j=l2;
			
			while(i<=h1 && j<=h2 )
			{
				if( kBuff[j] <= kBuff[i] )
					tempbuff[k++]=kBuff[i++];
				else
					tempbuff[k++]=kBuff[j++];
			}
			
			while(i<=h1)
				tempbuff[k++]=kBuff[i++];
			while(j<=h2)
				tempbuff[k++]=kBuff[j++];
			/**Merging completed**/
			/*Take the next two pairs for merging */
			l1=h2+1; 
		}/*End of while*/

		/*any pair left */
		for(i=l1; k<MAX; i++) 
			tempbuff[k++]=kBuff[i];

		for(i=0;i<MAX;i++)
			kBuff[i]=tempbuff[i];

		
	}/*End of for loop */

    if(copy_to_user(endbuff, tempbuff, length))
    {
       printk(KERN_ERR "Not all data was copied to user\n");
       return -EFAULT;
    }
    
    printk(KERN_INFO "Done with system call \n");

    return 0;
}
