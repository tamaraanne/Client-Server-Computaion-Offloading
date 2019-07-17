//Library files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

//input arguments int argc and char *argv[] to obtain input during runtime
//referenced from Beej's guide to network programming (Section 6.2)
int main(int argc, char *argv[])
{
//check if arguments entered are three else terminate execution
if(argc!=3)
{
printf("Incorrect arguments\n");
exit(1);
}
int clientsocket;
int awsport=25945;
struct sockaddr_in clientaddress, awsaddress, clientdetails;
float finalresult;

//structure to store the input function and x value
struct {char fun[10]; char num[10];} buffer;
int s;
//copy input function to the structure variable fun
strcpy(buffer.fun,argv[1]);
//copy input x value to the structure variable num
strcpy(buffer.num,argv[2]);

//Address structures referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//TCP client address structure to store IP address 127.0.0.1 and port
clientaddress.sin_family=AF_INET;
clientaddress.sin_addr.s_addr=inet_addr("127.0.0.1");
//dynamic port assignment
clientaddress.sin_port=htons(0);

//AWS TCP server adrress structure to store IP address 127.0.0.1 and port 25945
awsaddress.sin_family=AF_INET;
awsaddress.sin_addr.s_addr=inet_addr("127.0.0.1");
awsaddress.sin_port=htons(awsport);

//referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//socket creation
if((clientsocket=socket(PF_INET, SOCK_STREAM, 0))==-1)
{
perror("Client Socket Creation\n");
exit(1);
}

//Binding referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//binding the created socket to the client address structure
if(bind(clientsocket, (struct sockaddr *)&clientaddress, sizeof clientaddress)==-1)
{
close(clientsocket);
perror("Error while Binding socket");
exit(1);
}

s=sizeof(struct sockaddr);

//to obtain the address and dynamic port selected and store in clientdetails structure
getsockname(clientsocket, (struct sockaddr *) &clientdetails, &s);
//printing obtained details
//printf("The client IP address is %s\n",inet_ntoa(clientdetails.sin_addr));
//printf("Operating on Port %d\n",(int) ntohs(clientdetails.sin_port));

//connect referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//connecting TCP client to the AWS TCP server
if(connect(clientsocket, (struct sockaddr *) &awsaddress, sizeof awsaddress)==-1)
{
close(clientsocket);
perror("Error Connecting\n");
exit(1);
}

printf("The client is up and running\n");

while(1)
{
//referenced from Beej's guide to network programming (Section 6.1)
//sending data stored in structure to the connected AWS TCP server
if(send(clientsocket,&buffer,sizeof(buffer),0)==-1)
{
perror("Error sending data\n");
exit(1);
}

//referenced from Beej's guide to network programming (Section 6.2)
//receiving the final result from the AWS TCP server
if(recv(clientsocket,&finalresult,sizeof(float),0)==-1)
{
perror("Error receiving the result\n");
exit(1);
}
printf("According to AWS, %s on <%s>: <%g>\n",buffer.fun,buffer.num,finalresult);
break;
}
//close created socket
close(clientsocket);
return 0;
}



