#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
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
   
int main(int argc, char const *argv[]) 
{ 
   time_t timestamp;
   timestamp=time(NULL);
   struct timeval currentTime;
   gettimeofday(&currentTime,NULL);
   unsigned long micro=currentTime.tv_sec*(uint64_t)1000000+currentTime.tv_usec;
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    FILE * fp=fopen("socket_server.txt","a");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
 time(&timestamp);
   print_time(fp);
    send(sock , hello , strlen(hello) , 0 ); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024);
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"%s\n",buffer ); 
    send(sock , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"%s\n",buffer );
    send(sock , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(sock , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer );
    send(sock , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(sock , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer );
    send(sock , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(sock , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    fprintf(fp,"%s\n",buffer );
    return 0; 
} 
