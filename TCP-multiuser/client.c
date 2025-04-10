#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

void *receive_messages(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024];
    int len;

    while ((len = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[len] = '\0';
        printf("Message: %s", buffer);
    }

    return NULL;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    pthread_t recv_thread;
    char msg[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // or use inet_addr("127.0.0.1")

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    pthread_create(&recv_thread, NULL, receive_messages, &sock);

    printf("Connected to the server. Type messages below:\n");
    while (fgets(msg, sizeof(msg), stdin)) {
        send(sock, msg, strlen(msg), 0);
    }

    close(sock);
    return 0;
}
