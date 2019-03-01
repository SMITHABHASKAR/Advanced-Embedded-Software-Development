#include <unistd.h> 
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
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    FILE * fp=fopen("socket_server.txt","a");
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n");
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n");
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n"); 
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );
 time(&timestamp);
   print_time(fp); 
    fprintf(fp,"Hello message sent\n");
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    valread = read( new_socket , buffer, 1024); 
    fprintf(fp,"%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
 time(&timestamp);
   print_time(fp);
    fprintf(fp,"Hello message sent\n"); 
    return 0; 
} 
