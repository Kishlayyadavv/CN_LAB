#include <stdio.h>
#include <stdlib.h>
#include <arpnet/inet.h>
#include <string.h>
#include <p_thread.h>

#define PORT 8000
void chat(int sock,int client_id){
    char client_id_str[10];
    sprintf(client_id_str,"%d",client_id);
    send(sock,client_id_str,sizeOf(client_id_str),0);
    char buffer[1024];
    for(int i=0;i<5;i++){
        printf("Enter the message:");
        scanf("%s",&buffer);
    }
    
    
}
void main(){
    int client_id,sock_fd;
    p_thread lock;
    struct sockaddr_in server,client_addr;
    socklen_t addr_len=sizeOf(client_addr);

    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0){
        printf("The connection failed");
        exit(1);
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr="127.0.0.1";
    server.port=PORT;

    if(bind(sock_fd,(struct sockaddr_in *)&server,sizeOf(server))<0){
        printf("Failed to bind to the port");
        exit(1);
    }
    
    chat(sock_fd,client_id)

}