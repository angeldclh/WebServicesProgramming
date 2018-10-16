#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../constants.h"


int main(int argc, char **argv){
    if (argc != 2 || (strcmp(argv[1],"1") != 0 && strcmp(argv[1],"2") != 0 && strcmp(argv[1],"3")) != 0 ){
        fprintf(stderr, "CLIENT: Introduce your client identifer (1, 2 or 3) as an argument.\n");
        return 1;
    }
    int sock, aux;
    int num1, num2, op, clientid;
    struct sockaddr_in sock_address, sock_address_server;
    char msg[MSGLEN];
    char result_msg[RESLEN];//, *line;
    socklen_t sizesock;
    
    
    //Create UDP socket
    bzero(&sock_address, sizeof(struct sockaddr_in));
    sock_address.sin_family=AF_INET;
    sock_address.sin_addr.s_addr=inet_addr(LOCALHOST); //Same for all clients
    //Each type client has his own port. TODO: try with htons(0)
    if(strcmp(argv[1], "1") == 0){
        clientid=1;
        sock_address.sin_port=htons(PORT_CLIENT1);
    }   
    else if (strcmp(argv[1], "2") == 0){
        clientid=2;
        sock_address.sin_port=htons(PORT_CLIENT2);
    }
    else{
        clientid=3;
        sock_address.sin_port=htons(PORT_CLIENT3);
    }

    printf("Puerto: %d\n", sock_address.sin_port);

    sizesock = sizeof(sock_address);
   
    if((sock=socket(AF_INET,SOCK_DGRAM,0))==-1){
        fprintf(stdout,"CLIENT: Error at socket creation.\n");
        exit(1);
    }

    printf("CLIENT: socket created.\n");

    //Bind socket to port
    if(bind(sock, (struct sockaddr*) &sock_address, sizeof(sock_address))<0){
      fprintf(stdout,"CLIENT: Error at socket binding.\n");
      close(sock);
      exit(1);
    }
    printf("CLIENT: success at binding.\n");

    //Define the server's address
    bzero(&sock_address_server, sizeof(struct sockaddr_in));
    sock_address_server.sin_family=AF_INET;
    sock_address_server.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);

    

    if(clientid == 1) { //Get two digits
        sock_address_server.sin_port=htons(PORT_SERVER1);
        //Empty the message
        bzero(msg, MSGLEN*sizeof(char));
        
        //Ask for the first number
        do{
            printf("Please, enter the first digit and press enter.\n");
            num1 = getchar();
            msg[0] = (char) num1;
            while((aux = getchar()) != EOF && aux != '\n');
        }
        while(num1-'0'>9);
        
        //Same for second number
        do{
            printf("Please, enter the second digit and press enter.\n");
            num2 = getchar();
            msg[1] = (char) num2;
            while((aux = getchar()) != EOF && aux != '\n');
        }
        while(num2-'0'>9);   

        //Message is formed, send it to the server
        if(sendto(sock, &msg, MSGLEN, 0, (struct sockaddr *) &sock_address_server, sizesock) == -1) {
            fprintf(stderr, "CLIENT: Error when trying to send the numbers.\n");
            close(sock);
            exit(1);
        }
    }
    else if(clientid == 2){ //Get the operation
        sock_address_server.sin_port=htons(PORT_SERVER2);
        do {
            printf("Please, enter the operation you want to perform (+, -, * or /) and press enter.\n");
            op = getchar();
            while((aux = getchar()) != EOF && aux != '\n');
            }
        while(op != 43 && //ASCII +
              op != 45 && //ASCII -
              op != 42 && //ASCII *
              op != 47); //ASCII /

        //Message is formed, send it to the server
        if(sendto(sock, (char*) &op, 1, 0, (struct sockaddr *) &sock_address_server, sizesock) == -1) {
            fprintf(stderr, "CLIENT: Error when trying to send the operation.\n");
            close(sock);
            exit(1);
        }
    }
    else{ //Send "hello message" and receive the result
        sock_address_server.sin_port=htons(PORT_SERVER3);
        if(sendto(sock, "h", 1, 0, (struct sockaddr *) &sock_address_server, sizesock) == -1) {
            fprintf(stderr, "CLIENT: Error when trying to send the hello message.\n");
            close(sock);
            exit(1);
        }
        printf("CLIENT: hello message sent.\n");

        
        if(recvfrom(sock, (char *) result_msg, RESLEN, 0, (struct sockaddr *) &sock_address_server, &sizesock) == -1){
            fprintf(stderr, "CLIENT: Error when receiving result.\n");
            close(sock);
            exit(1);
        }
        printf("The result is %s.\n", result_msg);
    }

    
    //Close the socket
    close(sock);
 
    return 0;
}

        


        
