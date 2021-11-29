#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include "nokia5110.h"

#define PORT 9000
#define BACKLOG_CONNECTIONS 6
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define FILE_PATH "/var/tmp/aesdsocketdata"
#define SA struct sockaddr
int socket_fd, client_fd, write_file_fd;
struct addrinfo hints, *res;
char ipstr[INET6_ADDRSTRLEN];
uint8_t daemon_arg = 0;


int lcd_display(char * buffer);



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


/* handler for SIGINT and SIGTERM */
static void signal_handler (int signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
	syslog(LOG_NOTICE, "Caught signal, exiting\n");

	close(write_file_fd);
	remove(FILE_PATH);
	shutdown(socket_fd, SHUT_RDWR);
    }
    else {
	/* this should never happen */
	closelog();
	fprintf (stderr, "Unexpected signal!\n");
	exit (EXIT_FAILURE);
    }

    closelog();
    exit (EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

    struct sockaddr_in servaddr;
    openlog("aesdsocket.c", LOG_PID, LOG_USER);
    // Register signals with handler
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    int opt;
    while ((opt = getopt(argc, argv,"d")) != -1) {
        switch (opt) {
            case 'd' :
                daemon_arg = 1;
                break;
            default:
                break;
        }
    }

    int reuse_addr =1;

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
	syslog(LOG_ERR, "Error in capturing socket file descriptor\n");
	closelog();
	return -1;
    }

       // Reuse address
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int)) == -1) {
        syslog(LOG_ERR, "setsockopt");
        close(socket_fd);
	closelog();
        return -1;
    }


    if(daemon_arg)
    {
    	int pid = fork();

    	if (pid < 0 )
	    syslog(LOG_ERR, "Forking error\n");

    	if(pid > 0)
    	{
	    syslog(LOG_DEBUG, "Daemon created\n");
	    exit(EXIT_SUCCESS);
    	}
    	if(pid == 0)
    	{
	    int sid = setsid();

            if(sid == -1)
	    {
	    	syslog(LOG_ERR, "Error in setting sid\n");
		close(socket_fd);
		closelog();
	    	exit(EXIT_FAILURE);
	    }

            if (chdir("/") == -1)
	    {
            	syslog(LOG_ERR, "chdir");
            	close(socket_fd);
		closelog();
            	exit(EXIT_FAILURE);
            }

    	    int dev_null_fd = open("/dev/null", O_RDWR);
            dup2(dev_null_fd, STDIN_FILENO);
            dup2(dev_null_fd, STDOUT_FILENO);
            dup2(dev_null_fd, STDERR_FILENO);

            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
    	}
    }
  
  
	//connect
	// assign IP, PORT
	char ip_addr[24]={0};
	printf("Enter IP address of the server: ");
	scanf("%s",ip_addr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip_addr);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(socket_fd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	char buff[80] = {'\0'};
    	//while(1)
	//{
		printf("\n\rFrom Server :");
		read(socket_fd, buff, sizeof(buff));
		printf("%s",buff); 
	//}
	
	char read_buffer[256];
	
	
	
	
	
	read(socket_fd, read_buffer, sizeof(read_buffer));
	
	printf("%s %d %d ",read_buffer, read_buffer[1], read_buffer[20]);
	
	
	
	lcd_display(read_buffer);
	
	
	
	
}


int lcd_display(char * buffer)
{

int rc;
//int i;
int x, y;

	// SPI Channel, D/C, RESET, LED
	rc = nokiaInit(0, 37, 35, 13);
	if (rc != 0)
	{
		printf("Problem initializing nokia5110 library\n");
		return 0;
	}
	
	nokiaWriteString(2, 1, "AESD Final", FONT_NORMAL);
	nokiaWriteString(2, 2, "Sharath Jonnala", FONT_NORMAL);
	nokiaWriteString(1, 3, "Chaithra Suresh", FONT_NORMAL);
	nokiaWriteString(2, 4, "2d-Object Scanner", FONT_SMALL);
	
	usleep(4000000);
	
	// draw a box around the whole display
	for (x=0; x<84; x++)
	{
		nokiaSetPixel(x, 0, 1);
		nokiaSetPixel(x, 47, 1);
	}
	for (y=0; y<48; y++)
	{
		nokiaSetPixel(0, y, 1);
		nokiaSetPixel(83, y, 1);
	}
	usleep(3000000);
	
	
	/*
	for (i=0; i<10000; i++)
	{
		x = rand() & 0x7f;
		y = rand() & 0x3f;
		// cover entire display with black
		while (nokiaGetPixel(x,y))
		{
			x++;
			if (x >= 84)
			{
				y++;
				x = 0;
				if (y >= 48)
					break;
			}
		}
		nokiaSetPixel(x, y, 1);	
	}
	*/
	
	
	//usleep(4000000);
	int m=0;
	
	printf("\n");
	
	for(int k=0; k<48; k=k+3)
	{	
		for(int j=0; j<48; j=j+3)
		{
			if(buffer[m]=='O'){
			
			for(int g=k;g<k+3;g++)
			for(int f=j;f<j+3;f++)
			
			nokiaSetPixel(g, f, 0);
			printf("0 ");
			}
			else {
			
			for(int g=k;g<k+3;g++)
			for(int f=j;f<j+3;f++)
			
			nokiaSetPixel(g,f, 1);
			printf("1 ");
			}
			
			m++;
			if(m==256) break;
			
			
		}
		printf("\n");
	}
	
	
	usleep(3000000);
	nokiaShutdown();

	return 0;
	
	}
