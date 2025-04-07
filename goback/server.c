#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define TOTAL_FRAMES 10
#define LOSS_PROBABILITY 3 // Simulate 1 in 3 frames lost

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int expected_frame = 0, received_frame;
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Bind socket to port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for connections
    listen(server_fd, 5);
    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_socket == -1) {
        perror("Connection failed");
        exit(1);
    }

    printf("Client connected!\n");

    while (expected_frame < TOTAL_FRAMES) {
        // Receive frame
        read(new_socket, &received_frame, sizeof(received_frame));

        // Simulate random loss
        if (rand() % LOSS_PROBABILITY == 0) {
            printf("Frame %d lost!\n", received_frame);
            continue; // Ignore this frame
        }

        printf("Frame %d received successfully.\n", received_frame);

        // Send ACK only for expected frames
        if (received_frame == expected_frame) {
            write(new_socket, &expected_frame, sizeof(expected_frame));
            printf("ACK %d sent\n", expected_frame);
            expected_frame++; // Move to next expected frame
        } else {
            printf("Out-of-order frame received, discarding...\n");
        }
    }

    printf("All frames received successfully!\n");
    close(new_socket);
    close(server_fd);
    return 0;
}
