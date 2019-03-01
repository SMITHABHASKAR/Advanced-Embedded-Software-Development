#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<time.h>
#include<sys/time.h>
#include<inttypes.h>

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

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int toend;
   key_t key;
   FILE * fp=fopen("queueo.txt","a");
   time_t timestamp;
   timestamp=time(NULL);
   struct timeval currentTime;
   gettimeofday(&currentTime,NULL);
   unsigned long micro=currentTime.tv_sec*(uint64_t)1000000+currentTime.tv_usec;

   
   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msqid = msgget(key, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to receive messages.\n");
   
   for(;;) { /* normally receiving never ends but just to make conclusion 
             /* this program ends wuth string of end */
      if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
         perror("msgrcv");
         exit(1);
      }
      time(&timestamp);
      print_time(fp);
      fprintf(fp,"recvd: \"%s\"\n", buf.mtext);
      toend = strcmp(buf.mtext,"end");
      if (toend == 0)
      break;
   }
   fprintf(fp,"message queue: done receiving messages.\n");
   system("rm msgq.txt");
   return 0;
}