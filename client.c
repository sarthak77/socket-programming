// Client side C/C++ program to demonstrate Socket programming

//including libraries
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

//defining port
#define PORT 8080

//driver code
int main(int argc, char const *argv[])
{

	//declaring data structures
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";

	char *cmd1 = "listall";
	char *cmd2 = "send";
	char commands[1000]={0};

	char buffer[1024] = {0};

	while(1)
	{

		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Socket creation error \n");
			return -1;
		}

		memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
		// which is meant to be, and rest is defined below

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Converts an IP address in numbers-and-dots notation into either a
		// struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
		{
			printf("\nInvalid address/ Address not supported \n");
			return -1;
		}

		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
		{
			printf("\nConnection Failed \n");
			return -1;
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//clearing buffers
		memset(commands,0,sizeof(commands));
		memset(buffer,0,sizeof(buffer));
	
		// printf("\n'%s'\n",&commands);//testing

		//cmd prompt
		printf("Enter the command-> ");
		
		//space sepearated inputs appowed
		scanf("%[^\n]%*c",&commands);

		//checking for listall
		if(!(strcmp(commands,cmd1)))
		{
			send(sock,cmd1,strlen(cmd1),0);  // send the message.
			valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer
			printf("\n%s\n",buffer );
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//checking for send
        else if(!(strncmp(commands,cmd2,4)))
		{
			//error handling if no file given
			if(!(strcmp(commands,cmd2)))
			{
				printf("Enter file name\n");
			}
			else
			{
				int count=0;
				char delim[]=" ";
				char *ptr = strtok(commands, delim);
				while(ptr != NULL)
				{
					count++;

					//error handling
					if(count>2)
					{
						printf("Enter 1 file\n");
						break;
					}

					//copying filename in commands buffer
					strcpy(commands,"");
					strcat(commands,ptr);

					ptr = strtok(NULL, delim);
				}

				//count=2 implies only one file given
				if(count==2)
				{
					send(sock,commands,strlen(commands),0);  // send the message.
					valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer

					//if file cannot be downloaded
					if(!strcmp(buffer,"Cannot download the file\n"))
					{
						printf("%s",buffer);
					}
					else
					{
						FILE* file;
						file=fopen(commands,"w");
						if(file==NULL)
						{   
							printf("Cannot download the file\n");
							// return 0;
						}
						else
						{
							fputs(buffer,file);
							fclose(file);
						}
					}
				}
			}
		}
	}
    return 0;
}
