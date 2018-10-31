#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "../constants.h"


/* Structure to pass as argument to the connection handler */
typedef struct pthread_arg_t {
    int fd; //File descriptor for the socket
    struct sockaddr_in address; //Client address
} pthread_arg_t;


/* Function which creates the TCP socket pointed by  sock, binds it to the address
   pointed by sock_address using the port port and starts listening) */
int create_bind_listen_socket(int *sock, struct sockaddr_in *sock_address, int port){
    //Create TCP socket
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


/* TODO: Thread function */
void *connection_handler(void *arg){
    char op, read, aux;
    char buf[MSGLEN], auxbuf[MSGLEN], result_msg[RESLEN];
    double num1, num2, result;
    int i, fd;
    pthread_arg_t *thread_arg;

    thread_arg = (pthread_arg_t *) arg;
    fd = thread_arg->fd;

    //Receive message with numbers and operation
    if((aux = recv(fd, (char *)&buf, MSGLEN, 0)) == -1){
        fprintf(stderr, "SERVER: Error when receiving message from client.\n");
        close(fd);
        return NULL;
    }
    else if (aux == 0) { //Client closed connection
        printf("SERVER: Client closed the connection.\n");
        return NULL;
    }
 
    //Get numbers and operation. No need to check the message's format: clients do that
    //First number
    i = 0;
    while(buf[i] != '+' && buf[i] != '-' && buf[i] != '*' && buf[i] != '/'){
        read = buf[i];
        auxbuf[i] = read;
        i++;
    }
    auxbuf[i] = '\0'; //End of string
    //String to double
    num1 = strtod(auxbuf, NULL);

    //Operation:
    op = buf[i];
    i++;

    //Second number
    num2 = strtod(&buf[i], NULL);

    //Result
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
        result = num1/num2;
    }
       
    //Create a message with the result
    memset(result_msg, 0, sizeof(result_msg));
    snprintf((char*)&result_msg, RESLEN, "%.3f", result);
        
    //Send the result
    if(send(fd, &result_msg, RESLEN, 0) == -1){
        fprintf(stderr, "SERVER: Error when trying to send the result.\n");
        close(fd);
        exit(1);
    }

    close(fd);

    return NULL;
}
    

int main(){

    int listening_sock;
    socklen_t addrlen;
    struct sockaddr_in lsock_address;
    pthread_t thread;
    pthread_arg_t *thread_arg;
    pthread_attr_t thread_attr;

    addrlen = sizeof(struct sockaddr_in);
    
    //Listening socket
    if(create_bind_listen_socket(&listening_sock, &lsock_address, PORT_SERVER) != 0){
        fprintf(stderr, "SERVER: couldn't create listening socket.\n");
        return 1;
    }

    //Initialise thread attributes
    if(pthread_attr_init(&thread_attr) != 0){
        fprintf(stderr, "Error initialising thread attributes.\n");
        exit(1);
    }
    if(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0){
        fprintf(stderr, "Error pthread_attr_setdetachstate.\n");
        exit(1);
    }


    //Server loop
    while(1){
        //Structure to pass to the connection handler
        if((thread_arg = (pthread_arg_t *)malloc(sizeof(pthread_arg_t))) == NULL){
            fprintf(stderr, "Error at malloc.\n");
            continue;
        }
        
        //When accepting a connection, create a new thread for that client
        if((thread_arg->fd = accept(listening_sock, (struct sockaddr *)&thread_arg->address, &addrlen)) != 0){
            fprintf(stderr, "Error when accepting connection.\n");
            free(thread_arg);
            continue;
        }

        if((pthread_create(&thread, &thread_attr, connection_handler, (void *)thread_arg)) != 0){
            fprintf(stderr, "Error when creating new thread.\n");
            free(thread_arg);
            continue;
        }
    }

    return 0;
}
