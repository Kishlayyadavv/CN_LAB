#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 5

int client_sockets[MAX_CLIENTS]; // Store connected clients
int client_count = 0;
pthread_mutex_t lock; // Mutex for thread safety

// Function to send the current time to all clients
void *broadcast_time(void *arg) {
    while (1) {
        sleep(5); // Wait for 5 seconds

        time_t now = time(NULL);
        char time_str[100];
        snprintf(time_str, sizeof(time_str), "Current Time: %s", ctime(&now));

        pthread_mutex_lock(&lock);
        for (int i = 0; i < client_count; i++) {
            send(client_sockets[i], time_str, strlen(time_str), 0);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    pthread_t time_thread;
    pthread_mutex_init(&lock, NULL);

    // 1. Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Bind to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // 3. Listen for connections
    listen(server_fd, MAX_CLIENTS);
    printf("Server listening on port %d...\n", PORT);

    // 4. Start time broadcasting thread
    pthread_create(&time_thread, NULL, broadcast_time, NULL);

    while (1) {
        // 5. Accept new client connection
        new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (new_socket == -1) {
            perror("Connection failed");
            continue;
        }

        pthread_mutex_lock(&lock);
        client_sockets[client_count++] = new_socket; // Store new client socket
        pthread_mutex_unlock(&lock);

        printf("New client connected! Total clients: %d\n", client_count);
    }

    // Clean up
    pthread_mutex_destroy(&lock);
    close(server_fd);
    return 0;
}
