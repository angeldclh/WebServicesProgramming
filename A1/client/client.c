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


int main(){
    int sock, end, aux, anothercalc;
    int num1, num2, op;
    struct sockaddr_in sock_address, sock_address_server;
    char msg[MSGLEN], result_msg[MSGLEN];
    socklen_t sizesock;

    //Create UDP socket
    bzero(&sock_address, sizeof(struct sockaddr_in));
    sock_address.sin_family=AF_INET;
    sock_address.sin_addr.s_addr=inet_addr(LOCALHOST);
    sock_address.sin_port=htons(PORT_CLIENT);
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
    sock_address_server.sin_port=htons(PORT_SERVER);
    

    //Get and send digits and operation, receive result and ask for another operation or closing
    end = 0; 
    while(!end){
        //Empty the message
        bzero(msg, MSGLEN*sizeof(char));
        
        //Ask for the first number
        do{
            printf("Please, enter the first digit and press enter.\n");
            num1 = getchar();
            msg[0] = (char) num1;
            printf("1st number: %d\n", num1-'0');
            while((aux = getchar()) != EOF && aux != '\n');
        }
        while(num1-'0'>9);
        
        //Same for second number
        do{
            printf("Please, enter the second digit and press enter.\n");
            num2 = getchar();
            msg[1] = (char) num2;
            printf("2nd number: %d\n", num2-'0');
            while((aux = getchar()) != EOF && aux != '\n');
        }
        while(num2-'0'>9);   

        //Ask for the operation

        do {
            printf("Please, enter the operation you want to perform (+, -, * or /) and press enter.\n");
            op = getchar();
            while((aux = getchar()) != EOF && aux != '\n');
            printf("Operation: %c\n", op-'0');
        }
        while(op != 43 && //ASCII +
              op != 45 && //ASCII -
              op != 42 && //ASCII *
              op != 47); //ASCII /
        msg[2] = (char) op;
            
        printf("mensaje: %s length = %lu\n", msg, strlen(msg));

        //Message is formed, send it to the server
        if(sendto(sock, &msg, MSGLEN, 0, (struct sockaddr *) &sock_address_server, sizesock) == -1) {
            fprintf(stderr, "CLIENT: Error when trying to send the message.\n");
            close(sock);
            exit(1);
        }

        printf("CLIENT: message sent.\n");

        //Receive the answer
        if(recvfrom(sock, (char *) result_msg, RESLEN, 0, (struct sockaddr *) &sock_address_server, &sizesock) == -1){
            fprintf(stderr, "CLIENT: Error when receiving result.\n");
            close(sock);
            exit(1);
        }
        printf("The result is %s.\n", result_msg);

        //Do another calculation or finish
        do{
        printf("Do you want to do another calculation? y/n\n");
        anothercalc = getchar();
        while((aux = getchar()) != EOF && aux != '\n');
        }        
        while(anothercalc != (int)'y' &&
              anothercalc != (int)'n');

        if(anothercalc == 'n')
            end = 1;
        }

    //Close the socket
    close(sock);
    return 0;
}

        


        
