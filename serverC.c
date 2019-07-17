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

//code referenced from Beej's guide to network programming (Section 6.3) and  https://www.programminglogic.com/sockets-programming-in-c-using-udp-datagrams/
int main()
{
int serversockC;
int MYPORT = 23945;
int clientport = 24945;
float data;
float result;
struct sockaddr_in clientaddress, serveraddrC;
struct sockaddr_storage receivedfrom;
socklen_t addrlen;
int rec_data,sent_data;

//server C address information structure
serveraddrC.sin_family=AF_INET;
serveraddrC.sin_addr.s_addr=inet_addr("127.0.0.1");
serveraddrC.sin_port=htons(MYPORT);

//client address information structure
clientaddress.sin_family=AF_INET;
clientaddress.sin_addr.s_addr=inet_addr("127.0.0.1");
clientaddress.sin_port=htons(clientport);

//socket creation
if((serversockC=socket(PF_INET, SOCK_DGRAM, 0))==-1)
{
perror("Error while creating socket");
exit(1);
}

//binding the server C socket to server C address information structure
if(bind(serversockC, (struct sockaddr *)&serveraddrC, sizeof serveraddrC)==-1)
{
close(serversockC);
perror("Error while Binding socket");
exit(1);
}


printf("Server C is up and running UDP using port %d\n",MYPORT);

addrlen=sizeof receivedfrom;

while(1)
{
//receiving data from the client
if((rec_data=recvfrom(serversockC,&data,sizeof(float),0,(struct sockaddr *) &receivedfrom, &addrlen))==-1)
{
perror("Error while receiving from AWS");
exit(1);
}
printf("The server C received input <%g>\n",data);

//computing 5th power
result = data*data*data*data*data;
printf("The server C calculated 5th power <%g>\n",result);

//sending data to client whose address mentioned in clientaddress structure
if((sent_data=sendto(serversockC,&result,sizeof(float),0, (struct sockaddr *) &clientaddress, sizeof clientaddress))==-1)
{
perror("Error while sending to AWS");
exit(1);
}
printf("The server C has finished sending the output to AWS\n");
}
return 0;
}

