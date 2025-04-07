#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char concatenated_msgs[BUFFER_SIZE] = "";
int word_count = 0;

void *handle_client(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int client_id;

    recv(sock, buffer, sizeof(buffer), 0);
    client_id = atoi(buffer); // First message indicates client ID

    for (int i = 0; i < 5; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(sock, buffer, sizeof(buffer), 0);

        if (client_id == 1) {
            strcat(concatenated_msgs, buffer);
            strcat(concatenated_msgs, " ");
        } else if (client_id == 2) {
            int count = 0;
            char *token = strtok(buffer, " ");
            while (token) {
                count++;
                token = strtok(NULL, " ");
            }
            word_count += count;
        }
        printf("Client %d: %s\n", client_id, buffer);
    }

    if (client_id == 2) {
        printf("\nConcatenated messages from Client 1: %s\n", concatenated_msgs);
        printf("Total words from Client 2: %d\n", word_count);
    }

    close(sock);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in server, client;
    socklen_t addr_size;
    pthread_t thread_id;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (connect(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 2);
    printf("Server listening on port %d\n", PORT);

    while ((new_socket = accept(server_fd, (struct sockaddr *)&client, &addr_size))) {
        new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        //new_sock is a dynamically allocated integer pointer (malloc(sizeof(int))).

        // *new_sock = new_socket; stores the client’s socket descriptor in it.
        // we dont carry out on single thread coz till the time one msg receives the client other has to wait
        // This ensures that each thread gets its own copy of the socket descriptor.
        pthread_create(&thread_id, NULL, handle_client, (void *)new_sock);
        pthread_detach(thread_id);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
