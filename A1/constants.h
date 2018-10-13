#define PORT_SERVER 50000 //Private port to avoid potential conflicts
#define PORT_CLIENT 50001 //Different to server's in case they both run on the same machine
#define MSGLEN 4 //2 numbers and 1 operator + \0
#define RESLEN 6 //In case of division, 3 decimals: x.xxx\0
#define SERVER_ADDRESS "127.0.0.1" //Change if server runs on a different machine
#define LOCALHOST "127.0.0.1"
