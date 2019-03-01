#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};
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

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int len;
   key_t key;
   system("touch msgq.txt");
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
   
   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to send messages.\n");
   printf("Enter lines of text, ^D to quit:\n");
   buf.mtype = 1; /* we don't really care in this case */
   
   while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
      len = strlen(buf.mtext);
      time(&timestamp);
      print_time(fp);
      fprintf(fp,"message sent: %s\n",buf.mtext);
      /* remove newline at end, if it exists */
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
      perror("msgsnd");
   }
   strcpy(buf.mtext, "end");
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
   perror("msgsnd");
   
   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   fprintf(fp,"message queue: done sending messages.\n");
   return 0;
}