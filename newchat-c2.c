#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<string.h>

int main(){
	struct sockaddr_in server_addr;
	socklen_t addr_size;
	int client_socket;
	char buffer[1024];

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket < 0){
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}
	printf("Created socket\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2222);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	addr_size = sizeof(server_addr);
	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("Error connecting to server\n");
		exit(EXIT_FAILURE);
	}
	printf("Connected to server\n");

	while(1){
		recv(client_socket, buffer, sizeof(buffer), 0);
		printf("Message recieved: %s\n", buffer);

		printf("Enter a message to server: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

		send(client_socket, buffer, sizeof(buffer), 0);
		bzero(buffer, 1024);
		sleep(1);
		
	}

	close(client_socket);
}