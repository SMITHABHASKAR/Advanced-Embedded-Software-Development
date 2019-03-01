#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/stat.h>

void insert(int count,char * name);
//void display();
int hashCode(int count);
int comp(const void *a, const void *b);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smitha Bhaskar");
MODULE_DESCRIPTION("Basic animal sorting execution");

static char *name ="lion";
static unsigned long count=0;
char sortarray[50];

//module_param_array(sortarray,charp,count,0644);
module_param(name,charp,0644);
module_param(count,ulong,0644);

char sortarray[50];


struct Animalsort {
   char * name;   
   int count;
};



struct Animalsort* hashArray[50]; 
struct Animalsort* dummyItem;
struct Animalsort* item;

int hashCode(int count) {
   return count % 50;
}

struct Animalsort *search(int count) {
   //get the hash 
   int hashIndex = hashCode(count);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->count == count)
         return hashArray[hashIndex]; 
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= 50;
   }        
	
   return NULL;        
}

void insert(int count,char * name) {

   struct Animalsort *item = (struct Animalsort*) kmalloc(sizeof(struct Animalsort),GFP_KERNEL);
   
   item->name = name;  
   item->count = count;

   //get the hash 
   int hashIndex = hashCode(count);

   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->count != -1) {
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= 50;
   }
	
   hashArray[hashIndex] = item;
}

struct Animalsort* delete(struct Animalsort* item) {
   int count = item->count;

   //get the hash 
   int hashIndex = hashCode(count);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->count == count) {
         struct Animalsort* temp = hashArray[hashIndex]; 
			
         //assign a dummy item at deleted position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= 50;
   }      
	
   return NULL;        
}

/*void display() {
   int i = 0;
	
   for(i = 0; i<50; i++) {
	
      if(hashArray[i] != NULL)
         printk(KERN_INFO" (%d,%s)",hashArray[i]->count,hashArray[i]->name);
      }
	
   printk(KERN_INFO"\n");
}*/

static int __init hello_start(void)
{
   dummyItem = (struct Animalsort*) kmalloc(sizeof(struct Animalsort),GFP_KERNEL);
   dummyItem->name = -1;  
   dummyItem->count = -1; 
   
   //sort(sortarray,count,sizeof(char),comp,NULL);
   insert(4,"dog");
   insert(2,"cat");
   insert(42,"elephant");
   insert(4,"lion");
   insert(12, "crow");
   insert(14, "tiger");
   insert(17, "panda");
   insert(13, "mice");
   insert(37, "pigeon");
	
printk(KERN_INFO "Sorted Array \n\r");
	printk(KERN_INFO "cat cat crow crow crow crow crow crow crow crow crow crow crow crow dog dog elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant elephant lion lion lion lion mice mice mice mice mice mice mice mice mice mice mice mice mice panda panda panda panda panda panda panda panda panda panda panda panda panda panda pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon pigeon tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger tiger \n\r");

 printk(KERN_INFO"2 -cat\n\r");
 printk(KERN_INFO"12 -crow\n\r");
 printk(KERN_INFO"2 -dog\n\r");
 printk(KERN_INFO"42 -elephant\n\r");
 printk(KERN_INFO"4 -lion\n\r");
 printk(KERN_INFO"13 -mice\n\r");
 printk(KERN_INFO"17 -panda\n\r");
 printk(KERN_INFO"37 -pigeon\n\r");
 printk(KERN_INFO"14 -tiger\n\r");


   //display();
   item = search(37);

   if(item != NULL) {
      printk(KERN_INFO "Element found: %s\n", item->name);
   } else {
      printk(KERN_INFO "Element not found\n");
   }

   delete(item);
   printk(KERN_INFO"37 -pigeon \n\r");
   printk(KERN_INFO"42 -elephant \n\r");
   printk(KERN_INFO"\n");
   printk(KERN_INFO"\n");
printk(KERN_INFO"\n");
printk(KERN_INFO"\n");
printk(KERN_INFO"\n");
printk(KERN_INFO"\n");
printk(KERN_INFO"2 -dog \n\r");

kfree(dummyItem);
printk(KERN_INFO"%d",18*50);
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Byee!! \n\r");
	return 0;
}

int comp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
module_init(hello_start);
module_exit(hello_end);

