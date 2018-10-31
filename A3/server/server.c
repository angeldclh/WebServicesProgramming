#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../constants.h"

/* Function which creates the TCP socket pointed by  sock, binds it to the address
   pointed by sock_address using the port port and starts listening) */
int create_bind_listen_socket(int *sock, struct sockaddr_in *sock_address, int port){
    //Create UDP socket
    bzero(sock_address, sizeof(struct sockaddr_in));
    sock_address->sin_family=AF_INET;
    sock_address->sin_addr.s_addr=INADDR_ANY;
    sock_address->sin_port=htons(port);
       
    if((*sock=socket(PF_INET,SOCK_STREAM,0))==-1){
        fprintf(stdout,"SERVER: Error at socket creation.\n");
        return -1;
    }
    
    printf("SERVER: socket created.\n");
    
    //Bind socket to port
    if(bind(*sock, (struct sockaddr*) sock_address, sizeof(*sock_address))<0){
        fprintf(stdout,"SERVER: Error at socket binding.\n");
        close(*sock);
        return -2;
    }
    printf("SERVER: success at binding.\n");

    //Listen
    if(listen(*sock, 3) < 0) {
        fprintf(stderr, "SERVER: error at listen.\n");
        return -3;
    }
    
    return 0;
}
    

int main(){

    int sock1, sock2, sock3, sock1_client, sock2_client, sock3_client, num1, num2, aux;
    char buf[MSGLEN], op;
    double result; //Double and not int for division
    char result_msg[RESLEN];
    socklen_t addrlen;
    struct sockaddr_in sock_address1, sock_address2, sock_address3;

    addrlen = sizeof(struct sockaddr_in);
    
    //Socket server <-> client 1
    if(create_bind_listen_socket(&sock1, &sock_address1, PORT_SERVER1) != 0){
        fprintf(stderr, "SERVER: couldn't create socket for client 1.\n");
        return 1;
    }

    //Socket server <-> client 2
    if(create_bind_listen_socket(&sock2, &sock_address2, PORT_SERVER2) != 0){
        fprintf(stderr, "SERVER: couldn't create socket for client 2.\n");
        return 1;
    }

    //Socket server <-> client 3
    if(create_bind_listen_socket(&sock3, &sock_address3, PORT_SERVER3) != 0){
        fprintf(stderr, "SERVER: couldn't create socket for client 3.\n");
        return 1;
    }

    
    //Server loop
    while(1){
        //Receive numbers from client 1
        printf("SERVER: waiting for 2 digits.\n");

        if((sock1_client = accept(sock1, (struct sockaddr *) &sock_address1, &addrlen)) < 0){
            fprintf(stderr, "SERVER: Error when accepting connetion from client 1.\n");
            close(sock1);
            close(sock2);
            close(sock3);            
            exit(1);
        }
            

        if((aux = recv(sock1_client, (char *)&buf, MSGLEN, 0)) == -1){
            fprintf(stderr, "SERVER: Error when receiving message with 2 digits.\n");
            close(sock1);
            close(sock2);
            close(sock3);            
            exit(1);
        }
        else if (aux == 0) { //Client closed connection
            printf("SERVER: Client 1 closed the connection.\n");
            //            close(sock1);
        }
        printf("SERVER: digits received, waiting for operation\n");

        //Accept connection from client 2 and get the operation
        if((sock2_client = accept(sock2, (struct sockaddr *) &sock_address2, &addrlen)) < 0){
            fprintf(stderr, "SERVER: Error when accepting connetion from client 2.\n");
            close(sock1);
            close(sock2);
            close(sock3);            
            exit(1);
        }

        if((aux = recv(sock2_client, (char *)&op, 1, 0)) == -1){
            fprintf(stderr, "SERVER: Error when receiving message with operation.\n");
            close(sock1);
            close(sock2);
            close(sock3);            
            exit(1);
        }
        else if (aux == 0) { //Client closed connection
            printf("SERVER: Client 1 closed the connection.\n");
            //            close(sock2);
        }

 
        printf("SERVER: operation received.\n");
        

        //Get numbers and operation. No need to check the message's format: clients do that
        num1 = buf[0] - '0';
        num2 = buf[1] - '0';
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
       
        //Create a message with the result
        snprintf((char*)&result_msg, RESLEN, "%.3f",result);
        
        
        //Accept connection from client 3 and send him the result
        if((sock3_client = accept(sock3, (struct sockaddr *) &sock_address3, &addrlen)) < 0){
            fprintf(stderr, "SERVER: Error when accepting connetion from client 3.\n");
            close(sock1);
            close(sock2);
            close(sock3);            
            exit(1);
        }

       printf("SERVER: sending result.\n");

       if(write(sock3_client, &result_msg, RESLEN) == -1){
           fprintf(stderr, "SERVER: Error when trying to send the result.\n");
           close(sock1);
           close(sock2);
           close(sock3);
           exit(1);
        }
       printf("SERVER: result sent.\n");
    }

    /* The sockets are not closed explicitly. However, I've checked with netstat --listen and
       SIGINT (ctrl-c) does close it */
    
}
            
