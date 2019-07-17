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

int main()
{
int awssocket, childsocket, udpsocket;
int MYTCPPORT=25945;
int MYUDPPORT=24945;
float data, result;
//structure to store the data sent by the TCP client
struct {char fun[10]; char num[10];} buffer;
char log[10];
char div[10];
char function[10];
//character array containing LOG
strcpy(log,"LOG");
//character array containing DIV
strcpy(div,"DIV");
int b1, b2;
float awsAresult, awsBresult, awsCresult, awsA2result, awsB2result;
struct sockaddr_in serverA_addr;
struct sockaddr_in serverB_addr;
struct sockaddr_in serverC_addr;
struct sockaddr_in awstcpaddress;
struct sockaddr_in awsudpaddress;
struct sockaddr_storage receivedfrom;
socklen_t addrlen;
socklen_t addrlenA, addrlenB, addrlenC;

//socket creation and binding referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//TCP socket creation
if((awssocket = socket(PF_INET, SOCK_STREAM, 0))==-1)
{
perror("Socket creation\n");
exit(1);
}

//Address structures referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//AWS TCP server address information (IP address: 127.0.0.1, PORT: 25945)
awstcpaddress.sin_family = AF_INET;
awstcpaddress.sin_port = htons(MYTCPPORT);
awstcpaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(awstcpaddress.sin_zero, '\0', sizeof awstcpaddress.sin_zero);

//binding TCP socket to AWS TCP server address structure
if(bind(awssocket, (struct sockaddr *) &awstcpaddress, sizeof(awstcpaddress))==-1)
{
close(awssocket);
perror("Error while Binding TCP socket");
exit(1);
}

printf("The AWS is up and running\n");

//socket creation and binding referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//UDP socket creation
if((udpsocket = socket(PF_INET, SOCK_DGRAM, 0))==-1)
{
perror("Error creating UDP socket");
exit(1);
}

//Address structures referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//AWS UDP client address information (IP address: 127.0.0.1, PORT: 24945)
awsudpaddress.sin_family = AF_INET;
awsudpaddress.sin_port = htons(MYUDPPORT);
awsudpaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(awsudpaddress.sin_zero, '\0', sizeof awsudpaddress.sin_zero);

//binding UDP socket to AWS UDP client address structure
if(bind(udpsocket, (struct sockaddr *)&awsudpaddress, sizeof awsudpaddress)==-1)
{
close(udpsocket);
perror("Error while Binding UDP socket");
exit(1);
}

while(1)
{
//referenced from Beej's guide to network programming (Section 6.1)
//listening to incoming TCP connections
if(listen(awssocket,5)==-1)
{
perror("Error: Listening\n");
exit(1);
}

addrlen=sizeof receivedfrom;
//referenced from Beej's guide to network programming (Section 6.1)
//accepting connection and creating child socket
childsocket = accept(awssocket, (struct sockaddr *) &receivedfrom, &addrlen);

if (childsocket<0)
{
perror("Error accepting\n");
return 1;
}
printf("connection accepted\n");

//referenced from Beej's guide to network programming (Section 6.2)
//receiving data from TCP client through child socket
if(recv(childsocket,&buffer,sizeof(buffer),0)==-1)
{
perror("Error Receiving data\n");
exit(1);
}
//copying input function from structure to variable function
strcpy(function,buffer.fun);
//copying input x value from structure to variable data
//conversion of x value from string to float data type
data=atof(buffer.num);

printf("The AWS received <%g> and function=%s from the client using TCP over port %d\n",data,function,MYTCPPORT);

//Address structures referenced from https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//backend serverA address structure
serverA_addr.sin_family = AF_INET;
serverA_addr.sin_port = htons(21945);
serverA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(serverA_addr.sin_zero, '\0', sizeof serverA_addr.sin_zero);
addrlenA=sizeof serverA_addr;

//backend serverB address structure
serverB_addr.sin_family = AF_INET;
serverB_addr.sin_port = htons(22945);
serverB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(serverB_addr.sin_zero, '\0', sizeof serverB_addr.sin_zero); 
addrlenB=sizeof serverB_addr;

//backend serverC address structure
serverC_addr.sin_family = AF_INET;
serverC_addr.sin_port = htons(23945);
serverC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(serverC_addr.sin_zero, '\0', sizeof serverC_addr.sin_zero);
addrlenC=sizeof serverC_addr;

//all sendto() and recvfrom() functions referenced from https://www.programminglogic.com/sockets-programming-in-c-using-udp-datagrams/
//sending x value to serverA through UDP socket
if(sendto(udpsocket,&data,sizeof(float),0,(struct sockaddr *) &serverA_addr, addrlenA)==-1)
{
perror("Error Sending data to back end server A\n");
continue;
}
else
printf("The AWS sent <%g> to Backend server A\n",data);

//receiving computed square from serverA through UDP socket
if(recvfrom(udpsocket,&awsAresult,sizeof(float),0,NULL, NULL)==-1)
{
perror("Receiving from backend server A\n");
continue;
}

//sending x value to serverB through UDP socket
if(sendto(udpsocket,&data,sizeof(float),0,(struct sockaddr *) &serverB_addr, addrlenB)==-1)
{
perror("Sending data to back end serverB\n");
continue;
}
else
printf("The AWS sent <%g> to Backend server B\n",data);

//receiving computed cube from serverB through UDP socket
if(recvfrom(udpsocket,&awsBresult,sizeof(float),0,NULL, NULL)==-1)
{
perror("Receiving from backend server B\n");
continue;
}

//sending x value to serverC through UDP socket
if(sendto(udpsocket,&data,sizeof(float),0,(struct sockaddr *) &serverC_addr, addrlenC)==-1)
{
perror("Sending data to back end server C\n");
continue;
}
else
printf("The AWS sent <%g> to Backend server C\n",data);

//receiving computed 5th power from serverC through UDP socket
if(recvfrom(udpsocket,&awsCresult,sizeof(float),0,NULL, NULL)==-1)
{
perror("Receiving from backend server C\n");
continue;
}

printf("The AWS server received <%g> Backend Server <A> using UDP over port <%d>\n",awsAresult,MYUDPPORT);

printf("The AWS server received <%g> Backend Server <B> using UDP over port <%d>\n",awsBresult,MYUDPPORT);

printf("The AWS server received <%g> Backend Server <C> using UDP over port <%d>\n",awsCresult,MYUDPPORT);

//sending x^2 value to serverA through UDP socket
if(sendto(udpsocket,&awsAresult,sizeof(float),0,(struct sockaddr *) &serverA_addr, addrlenA)==-1)
{
perror("Sending data to back end server A\n");
continue;
}
else
printf("The AWS sent <%g> to Backend server A\n",awsAresult);

//receiving computed 4th power from serverA through UDP socket
if(recvfrom(udpsocket,&awsA2result,sizeof(float),0,NULL, NULL)==-1)
{
perror("Receiving from backend server A\n");
continue;
}

//sending x^2 value to serverB through UDP socket
if(sendto(udpsocket,&awsAresult,sizeof(float),0,(struct sockaddr *) &serverB_addr, addrlenB)==-1)
{
perror("Sending data to back end server B\n");
continue;
}
else
printf("The AWS sent <%g> to Backend server B\n",awsAresult);

//receiving computed 6th power from serverB through UDP socket
if(recvfrom(udpsocket,&awsB2result,sizeof(float),0,NULL, NULL)==-1)
{
perror("Receiving from backend server B\n");
continue;
}

printf("The AWS server received <%g> Backend Server <A> using UDP over port <%d>\n",awsA2result,MYUDPPORT);

printf("The AWS server received <%g> Backend Server <B> using UDP over port <%d>\n",awsB2result,MYUDPPORT);

printf("Values of powers received by AWS: <%g, %g, %g, %g, %g, %g>\n",data,awsAresult,awsBresult,awsA2result,awsCresult,awsB2result);

//comparing function to div array
int function1=strcmp(div,function);
//div computation
if (function1==0)
{
result=1+data+awsAresult+awsBresult+awsA2result+awsCresult+awsB2result;
}
//comparing function to log array
int function2=strcmp(log,function);
//log computation
if(function2==0)
{
result=-data-(awsAresult/2)-(awsBresult/3)-(awsA2result/4)-(awsCresult/5)-(awsB2result/6);
}

printf("AWS calculated %s on <%g>: <%g>\n",function,data,result);

//referenced from Beej's guide to network programming (Section 6.1)
//sending computed result to TCP client through child socket
if(send(childsocket,&result,sizeof(float),0)==-1)
{
perror("Sending to client\n");
}
else
printf("The AWS sent <%g> to client\n",result);
//close child socket
close(childsocket);
}
return 0;
}






