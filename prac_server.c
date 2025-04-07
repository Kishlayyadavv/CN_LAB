void main(){
    int sock_fd;
    struct sockaddr_in server,client;
    addr_len adr=sizeOf(server);
    
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0){
        printf("The connection failed");
        exit(1);
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr="127.0.0.1";
    server.port=PORT;

    if(bind(sock_fd,(struct sockaddr_in *)&server,sizeOf(server),0)<0){
        printf("Failed to bind to the port");
        exit(1);
    }
    
    listen(sock_fd,2);
    while(1)
}