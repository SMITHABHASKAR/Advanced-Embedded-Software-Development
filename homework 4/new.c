#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#define N 2 //2 child threads

//Structure for the threads

typedef struct threadstruc{
	int child_number;
	pthread_t threadid;
  char * ft;
	
}threadd;

struct node{ //Hash map nodes
    int key;
    int val;
    struct node *next;
};
struct table{
    int size;
    struct node **list;
};
struct table *createTable(int size){
    struct table *t = (struct table*)malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++)
        t->list[i] = NULL;
    return t;
}


pthread_mutex_t pmutex;
void cpu_usage(int value, FILE * fpi);
void child_thread1(char * fname);
void child_thread2(char * fname);
int to_lower_file(FILE *f); // Function to convert contents of file into lower case
void * create_thread(void *);
void count_alpha(FILE * fpi);
void utilization(int argC,char* argV[], FILE * fpi);

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
	int frequency[26],x,c;
	fpi=fopen(fname,"a");
	pthread_t self;
	self=pthread_self();
	pid_t tid;
	tid=syscall(SYS_gettid);
	fprintf(fpi,"POSIX Thread ID:%d Linux Thread ID:%d\n\r",self,tid);
	fpo=fopen("gdb.txt","r+");
	//Converting contents of file into lower.
	status=to_lower_file(fpo);
	if(fpo==NULL) {
		printf("File doesnot exist\a");
		
		exit(0);
	}
    
    count_alpha(fpi);
  }

  void count_alpha(FILE * fpi){
    int frequency[26];
int ch;
FILE* txt_file = fopen ("gdb.txt", "r");
FILE * output = fpi;
 
/* init the freq table: */
for (ch = 0; ch < 26; ch++)
    frequency[ch] = 0;
 
while (1) {
    ch = fgetc(txt_file);
    if (ch == EOF) break; /* end of file or read error.  EOF is typically -1 */
 
    /* assuming ASCII; "letters" means "a to z" */
    if ('a' <= ch && ch <= 'z')      /* lower case */
        frequency[ch-'a']++;
    
  }
  for (ch = 0; ch < 26; ch++){
	if(frequency[ch]<100)
    	fprintf(output,"%c %d\n\r",ch+'a',frequency[ch]);
}
  fclose(txt_file);
  fclose(output);
}
  

void utilization(int argC,char* argV[], FILE * fpi)
{
  char str[100];
  const char d[2] = " ";
  char* token;
  int i = 0,times,lag;
  long int sum = 0, idle, lastSum = 0,lastIdle = 0;
  long double idleFraction;
  FILE * fname=fpi;
  if(argC!=3)
    fprintf(fname,"Usage : %s <number of times /proc/stat should be polled followed by delay in seconds.>",argV[0]);
 
  else{
    times = atoi(argV[1]);
    lag = atoi(argV[2]);
 
    while(times>0){
      FILE* fp = fopen("/proc/stat","r");
          i = 0;
      fgets(str,100,fp);
      fclose(fp);
      token = strtok(str,d);
 
      while(token!=NULL){
        token = strtok(NULL,d);
        if(token!=NULL){
          sum += atoi(token);
 
        if(i==3)
          idle = atoi(token);
 
        i++;
      }
    }
 
    fprintf(fname,"\nIdle for : %lf ms of the time.",(1.0 - (idle-lastIdle)*1.0/(sum-lastSum))*100);
 
    lastIdle = idle;
    lastSum = sum;
 
 
    times--;
    sleep(lag);
    } 
  }
 
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
  fprintf(fpi,"Child thread 2\n\r");
	fprintf(fpi,"POSIX Thread ID:%d Linux Thread ID:%d\n\r",self,tid);
  struct sigaction cputh2;
  struct itimerval tmr;

    memset (&cputh2, 0, sizeof (cputh2));
    cputh2.sa_handler = &cpu_usage;
    
    tmr.it_interval.tv_sec = 0;
    tmr.it_interval.tv_usec = 100000;

    tmr.it_value.tv_sec = 0;
    tmr.it_value.tv_usec = 100000;

    sigaction (SIGVTALRM, &cputh2, NULL);
    sigaction (SIGUSR1, &cputh2, NULL);
    sigaction (SIGUSR2, &cputh2, NULL);

    setitimer (ITIMER_VIRTUAL, &tmr, NULL);

    char str[100];
  const char d[2] = " ";
  char* token;
  int i = 0,times,lag;
  long int sum = 0, idle, lastSum = 0,lastIdle = 0;
  long double idleFraction;
  
 
    times = 5;
    lag = 2;
 
    while(times>0){
      FILE* fp = fopen("/proc/stat","r");
          i = 0;
      fgets(str,100,fp);
      fclose(fp);
      token = strtok(str,d);
 
      while(token!=NULL){
        token = strtok(NULL,d);
        if(token!=NULL){
          sum += atoi(token);
 
        if(i==3)
          idle = atoi(token);
 
        i++;
      }
    }
 
    fprintf(fpi,"\nIdle for : %lf ms of the time.",(1.0 - (idle-lastIdle)*1.0/(sum-lastSum))*100);
 
    lastIdle = idle;
    lastSum = sum;
 
 
    times--;
    sleep(lag);
    } 
  
  
    
    pthread_mutex_lock(&pmutex);
        fprintf(fpi,"Executing child thread 2 with POSIX Thread ID:%d Linux Thread ID:%d\n\r",self,tid);
    pthread_mutex_unlock(&pmutex);
    pthread_exit(NULL);
      


}

//Hash Map to sort data from text file.


int hashCode(struct table *t,int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}
void insert(struct table *t,int key,int val){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = list;
    t->list[pos] = newNode;
}
int lookup(struct table *t,int key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}

void cpu_usage(int value, FILE * fpi)
{
  if(value == SIGVTALRM)
  {
    char *comm = "cat /proc/stat | head -n 2";
    
    }
  
  if(value == SIGUSR1 || value == SIGUSR2)
  { 
    
    pthread_mutex_lock(&pmutex);
        fprintf(fpi, "Signal handler recieved, exiting threads\n\r");
    pthread_mutex_unlock(&pmutex);
    pthread_exit(NULL);
  }
}


void * create_thread(void *thread)
{
   threadd *my_data  = (threadd*)thread;
   int order=my_data->child_number;
   char * fname=my_data->ft;
   if(order==0)
    child_thread1(fname);
  else
    child_thread2(fname);
}

int main()
{
	
	char * fname = malloc(sizeof(char));
    FILE * fp;
   
   printf("Enter the name of the output file\n\r");
   scanf("%s",fname);

   int i;
   
   threadd thread[N];
   
    for(i=0; i<N; i++)
   {
      thread[i].ft=fname;
      thread[i].child_number=i;
     pthread_create(&(thread[i].threadid), NULL, create_thread, (void *)(thread+i));

   }

   
   //wait for all threads
   for (i = 0; i < N; i++)
       pthread_join(thread[i].threadid, NULL); 
   //print results of each thread
 pthread_exit(NULL);
}
