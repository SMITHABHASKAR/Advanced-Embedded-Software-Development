#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#define N 2 //2 child threads

//Structure for the threads

typedef struct threadstruc{
	int child_number;
	pthread_t threadid;
	char * fileio;
	char * output;
}threadd;

void child_thread1(char * fname);
void child_thread2(char * fname);
int to_lower_file(FILE *f); // Function to convert contents of file into lower case
void * create_thread(void *);

int to_lower_file(FILE *f)

    {

        int c;

        char ch;

        while ((ch = fgetc(f))!= EOF)

        {    

            c = (int)ch;

            if (c >= 65 && c <= 90)

            {

                ch = ch + 32;

                fseek(f, -1L, 1);

                fputc(ch, f);

            }

        }

        return 0;

    }

void child_thread1(char * fname){
	FILE * fpi;
	FILE * fpo;
	int status=0;
	char ch;
	int count[26],x,c;
	fpi=fopen(fname,"a");
	pthread_t self;
	self=pthread_self();
	pid_t tid;
	tid=syscall(SYS_gettid);
	printf("POSIX Thread ID:%d Linux Thread ID:%d\n\r",self,tid);
	fpo=fopen("gdb.txt","r+");
	//Converting contents of file into lower.
	status=to_lower_file(fpo);
	if(fpo==NULL) {
		printf("File doesnot exist\a");
		
		exit(0);
	}
    
    while((ch = fgetc(fpo)) != EOF) {
   /** Considering characters from 'a' to 'z' only and ignoring others. */
 
      if (ch >= 'a' && ch <= 'z') {
         x = ch - 'a';
         count[x]++;
      }
 
      c++;
   }
 
   for (c = 0; c < 26; c++)
         fprintf(fpi,"%c occurs %d times in the string.\n", c + 'a', count[c]);
 
   return 0;

   fclose(fpo);
   fclose(fpi);
}

void child_thread2(char * fname)
{
	FILE * fpi;
	FILE * fpo;

	fpi=fopen(fname,"a");
	pthread_t self;
	self=pthread_self();
	pid_t tid;
	tid=syscall(SYS_gettid);
	fprintf(fpi,"POSIX Thread ID:%d Linux Thread ID:%d\n\r",self,tid);
}


/*int main()
{
	char * fname = malloc(sizeof(char));
	FILE * fp;
	printf("Enter the name of the output file\n\r");
	scanf("%s",fname);
	fp=fopen(fname,"a");
	fprintf(fp,"hi");
	fclose(fp);
}
/*	int main()
{
    threadd thread[3];
    for (int i = 0; i < 2; i++) {
        pthread_create(&(thread->threadid[i]), NULL, create_thread,NULL);
    }
    for (int i = 0; i < 2; i++)
       pthread_join(&(thread->threadid[i]), NULL);
    return 0;
}*/

int main()
{
	pthread_t t1,t2;
	threadd * child_thread1 = malloc(sizeof(threadd));
	threadd * child_thread2 = malloc(sizeof(threadd));
	FILE * fileptr;
	fileptr = fopen("gdb.txt", "r");

	child_thread1 -> fileio= "gdb.txt";
	char * fname = malloc(sizeof(char));
    FILE * fp;
   
   printf("Enter the name of the output file\n\r");
   scanf("%s",fname);
   fp=fopen(fname,"a");
   	child_thread1 -> child_number = 1;
   	child_thread1 -> output = fname;
	child_thread2 -> fileio = "gdb.txt";
	child_thread2 -> child_number = 2;
    child_thread2 -> output = fname;
   int i;
   
   threadd thread[N];
   /*
    for(i=0; i<N; i++)
   {
      thread[i].threadid=i;
     pthread_create(&(thread[i].threadid), NULL, create_thread, (void *)(thread+i));

   }*/

pthread_create(t1, NULL, child_thread1,(char *) fname);
pthread_create(t2, NULL, child_thread2,(char *) fname);
    
  
   //wait for all threads
  /* for (i = 0; i < N; i++)
       pthread_join(thread[i].threadid, NULL); */
   //print results of each thread
  pthread_join(t1,NULL);
  pthread_join(t2,NULL); 
}