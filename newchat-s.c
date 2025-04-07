#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<string.h>

int main(){
	struct sockaddr_in server_addr, client_addr, client_addr2;
	int server_socket, client_socket, client_socket2;
	char buffer[1024];
	char buffer2[1024];
	socklen_t addr_size;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		perror("couldnt create socket\n");
		exit(EXIT_FAILURE);
	}

	printf("Socket created\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2222);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("couldnt bind socket\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket binded\n");

	listen(server_socket, 10);
	printf("Server listening...\n");

	addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
	printf("Client 1 joined\n");
	client_socket2 = accept(server_socket, (struct sockaddr*)&client_addr2, &addr_size);
	printf("Client 2 joined\n");

	while(1){
		//recv from 1 send to 2
		recv(client_socket, buffer, sizeof(buffer), 0);
		printf("Message recieverd: %s", buffer);

		send(client_socket2, buffer, sizeof(buffer), 0);
		bzero(buffer, 1024);

		//recv from 2 send to 1
		recv(client_socket2, buffer2, sizeof(buffer2), 0);
		printf("Message recieverd: %s", buffer);

		send(client_socket, buffer2, sizeof(buffer2), 0);
		bzero(buffer2, 1024);
		sleep(1);
	}

	close(client_socket);
	close(client_socket2);
	close(server_socket);
}