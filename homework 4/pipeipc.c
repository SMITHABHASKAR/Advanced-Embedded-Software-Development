#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
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
 

int main() {

   time_t timestamp;
   timestamp=time(NULL);
   struct timeval currentTime;
   gettimeofday(&currentTime,NULL);
   unsigned long micro=currentTime.tv_sec*(uint64_t)1000000+currentTime.tv_usec;

   char buffer[100],current[100]="";
   FILE * fp=fopen("pipe.txt","a");
sprintf(current,"%s.%lu",buffer,micro);

   int pipefds1[2], pipefds2[2],pipefds3[2],pipefds4[2];

   int returnstatus1, returnstatus2;

   int pid;

   char pipe1writemessage[20] = "Hi";

   char pipe2writemessage[20] = "Hello";

   char pipe3writemessage[20] = "LED ON";

   char pipe4writemessage[20] = "LED OFF";
 
   char readmessage[20];

   returnstatus1 = pipe(pipefds1);

   

   if (returnstatus1 == -1) {

      printf("Unable to create pipe 1 \n");

      return 1;

   }

   returnstatus2 = pipe(pipefds2);

   

   if (returnstatus2 == -1) {

      printf("Unable to create pipe 2 \n");

      return 1;

   }

   pid = fork();
   pid = fork();

   

   if (pid != 0) // Parent process
 {

      close(pipefds1[0]); // Close the unwanted pipe1 read side

      close(pipefds2[1]); // Close the unwanted pipe2 write side

      close(pipefds3[0]);

      close(pipefds4[1]);
      
      print_time(fp);

      fprintf(fp,"In Parent: Writing to pipe  – Message is %s\n", pipe1writemessage);

      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));

      read(pipefds2[0], readmessage, sizeof(readmessage));
      time(&timestamp);
      print_time(fp);
      fprintf(fp,"In Parent: Reading from pipe – Message is %s\n", readmessage);

     print_time(fp);

      fprintf(fp,"In Parent: Writing to pipe  – Message is %s\n", pipe3writemessage);

      write(pipefds3[1], pipe3writemessage, sizeof(pipe3writemessage));  

      print_time(fp);

      fprintf(fp,"In Parent: Writing to pipe  – Message is %s\n", pipe4writemessage);

      write(pipefds4[0], pipe4writemessage, sizeof(pipe4writemessage)); 
     

   }
 else { //child process

      close(pipefds1[1]); // Close the unwanted pipe1 write side

      close(pipefds2[0]); // Close the unwanted pipe2 read side

      close(pipefds3[1]);

      close(pipefds4[0]);

      read(pipefds1[0], readmessage, sizeof(readmessage));
       timestamp=time(NULL);
      print_time(fp);
      fprintf(fp,"In Child: Reading from pipe  – Message is %s\n", readmessage);
       timestamp=time(NULL);
      print_time(fp);
      fprintf(fp,"In Child: Writing to pipe  – Message is %s\n", pipe2writemessage);

      write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));

      print_time(fp);
      fprintf(fp,"In Child: Writing to pipe  – Message is %s\n", pipe3writemessage);

      write(pipefds3[1], pipe3writemessage, sizeof(pipe3writemessage));

      read(pipefds4[0], readmessage, sizeof(readmessage));
       timestamp=time(NULL);
      print_time(fp);
      fprintf(fp,"In Child: Reading from pipe  – Message is %s\n", readmessage);

   }

   return 0;

}
