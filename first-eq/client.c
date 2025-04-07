#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void chat(int sock, int client_id) {
    char buffer[BUFFER_SIZE];
    char client_id_str[10];
    sprintf(client_id_str, "%d", client_id);
    send(sock, client_id_str, strlen(client_id_str), 0);

    for (int i = 0; i < 5; i++) {
        printf("Enter message %d: ", i + 1);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        send(sock, buffer, strlen(buffer), 0);
    }
}

int main() {
    int sock;
    struct sockaddr_in server;
    int client_id;

    printf("Enter client ID (1 or 2): ");
    scanf("%d", &client_id);
    getchar(); // Consume newline

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    chat(sock, client_id);

    close(sock);
    return 0;
}
