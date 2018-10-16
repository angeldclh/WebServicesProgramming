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
    if (argc != 2 || (strcmp(argv[1],"1") != 0 && strcmp(argv[1],"2") != 0 && strcmp(argv[1],"2") != 0 )){
        fprintf(stderr, "CLIENT: Introduce your client identifer (1, 2 or 3).\n");
        return 1;
    }
    int sock, aux;
    int num1, num2, op, clientid;
    struct sockaddr_in sock_address, sock_address_server;
    char msg[MSGLEN], result_msg[RESLEN];
    socklen_t sizesock;

   
    
    
    //Create UDP socket
    bzero(&sock_address, sizeof(struct sockaddr_in));
    sock_address.sin_family=AF_INET;
    sock_address.sin_addr.s_addr=inet_addr(LOCALHOST);
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
    

    if(clientid == 1) { //Send two digits
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
        
    }
    else if(clientid == 2){ //Send the operation
        do {
            printf("Please, enter the operation you want to perform (+, -, * or /) and press enter.\n");
            op = getchar();
            while((aux = getchar()) != EOF && aux != '\n');
            }
        while(op != 43 && //ASCII +
              op != 45 && //ASCII -
              op != 42 && //ASCII *
              op != 47); //ASCII /
        msg[2] = (char) op;
    }
    else{ //Receive the result
        //Receive the answer
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

        


        
