# Client-Server-Computaion-Offloading

This project computes the Taylor Series Expansion of the log(x) or 1/x function using socket programming with computation offloading to backend servers.

The backend servers A, B and C are compiled and run first so as to keep them set up before initiating data transmission. The AWS is then compiled and run, followed finally by the compilation and running of the client. The input function and value of x is given as input arguments to the client during run time and is of the format ./client <function> <input>. The obtained input arguments are stored in a structure and sent over a TCP connection to AWS. The AWS sends the value of x to the three backend servers A, B and C over UDP connections so as to compute x^2, x^3 and x^5. To obtain x^4 and x^6, the AWS forwards the obtained x^2 from backend server A again to servers A and B respectively over UDP connections. The AWS checks the input function for LOG or DIV, based on which it computes the Taylor series expansion to obtain the approximate value of the provided function and input value. Finally, the computed result is sent back to the client over the TCP connection and is displayed on the terminal.

||References||
Input arguments taken during run time, send() and recv() functions have been referenced from Beej's guide to network programming (Sections 6.1, 6.2, 6.3 and 6.4)
Address structures, sendto() and recvfrom() functions, socket creation and binding have been referenced from the following websites:
https://www.programminglogic.com/sockets-programming-in-c-using-udp-datagrams/ 
https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/ 
