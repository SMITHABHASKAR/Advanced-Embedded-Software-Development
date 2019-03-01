#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <string.h>
#include<time.h>
#include<sys/time.h>
#include<inttypes.h>

char *message[]={ "One",
"Two","three","four","LED ON","LED OFF","five","six","LED ON","seven"

};
void client(FILE * fp); 
 void server(FILE * fp);

void print_time (FILE * fp);

void print_time (FILE * fp)

{

struct timeval tv;

struct tm* ptm;

char time_string[40];

long milliseconds;


/* Obtain the time of day, and convert it to a tm struct. */

gettimeofday (&tv, NULL);

ptm = localtime (&tv.tv_usec);

/* Format the date and time, down to a single second. */

strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);

/* Compute milliseconds from microseconds. */

milliseconds = tv.tv_usec / 1000000;

/* Print the formatted time, in seconds, followed by a decimal point

   and the milliseconds. */

 fprintf (fp,"%s.%06ld\n", time_string, milliseconds);

} 
  
void server(FILE * fp){ 
    
  // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    fprintf(fp,"Data read from memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
   

     
     
}  


void client(FILE * fp) 
{ 
     // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    printf("Write Data : "); 
    gets(str); 
  
    fprintf(fp,"Data written in memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
  
    
    
} 

int main(){

time_t timestamp;
   timestamp=time(NULL);
   struct timeval currentTime;
   gettimeofday(&currentTime,NULL);
   unsigned long micro=currentTime.tv_sec*(uint64_t)1000000+currentTime.tv_usec;

    FILE * fp=fopen("Sharedo.txt","a");
    int i;
    
   // server(fp);
    for(i=0;i<10;i++){

   time(&timestamp);
   print_time(fp);
    client(fp);
   time(&timestamp);
   print_time(fp);
    server(fp);
   }
    
    return 0;
}
