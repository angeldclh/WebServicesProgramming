#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../constants.h"


int main(){

    int sock, num1, num2;
    socklen_t sizesock;
    char buf[MSGLEN], op;
    double result; //Double and not int for division
    char result_msg[RESLEN];
    
    //Create UDP socket
    struct sockaddr_in sock_address;
    bzero(&sock_address, sizeof(struct sockaddr_in));
    sock_address.sin_family=AF_INET;
    sock_address.sin_addr.s_addr=INADDR_ANY;
    sock_address.sin_port=htons(PORT_SERVER);
    sizesock = sizeof(sock_address);

   
    if((sock=socket(PF_INET,SOCK_DGRAM,0))==-1){
        fprintf(stdout,"SERVER: Error at socket creation.\n");
        exit(1);
    }

    printf("SERVER: socket created.\n");

    //Bind socket to port
    if(bind(sock, (struct sockaddr*) &sock_address, sizeof(sock_address))<0){
      fprintf(stdout,"SERVER: Error at socket binding.\n");
      close(sock);
      exit(1);
    }
    printf("SERVER: success at binding.\n");


    //Server loop
    while(1){
        //Receive data
        printf("SERVER: waiting for message.\n");
        if(recvfrom(sock, (char *)&buf, MSGLEN, 0, (struct sockaddr *) &sock_address, &sizesock) == -1){
            fprintf(stderr, "SERVER: Error when receiving message.\n");
            close(sock);
            exit(1);
        }
        printf("SERVER; message received.\n");

        //Get numbers and operation. No need to check the message's format: the client does that
        num1 = buf[0] - '0';
        num2 = buf[1] - '0';
        op = buf[2];
        switch(op){
        case '+' :
            result = num1+num2;
            break;
        case '-' :
            result = num1-num2;
            break;
        case '*' :
            result = num1*num2;
            break;
        case '/': 
            result = (double)num1/(double)num2;
        }

        printf("SERVER: sending message.\n");
        
        //Create a message with the result
        snprintf((char*)&result_msg, RESLEN, "%.3f",result);

        //Send it
        if(sendto(sock, &result_msg, RESLEN, 0, (struct sockaddr *) &sock_address, sizesock) == -1) {
            fprintf(stderr, "SERVER: Error when trying to send the result.\n");
            close(sock);
            exit(1);
        }
    }

    /* The socket is not closed explicitly. However, I've checked with netstat --listen and
       SIGINT (ctrl-c) does close it */
    
}
            
