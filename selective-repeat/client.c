#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define TOTAL_FRAMES 10
#define WINDOW_SIZE 4
#define TIMEOUT 2 // Timeout duration in seconds

int acknowledged_frames[TOTAL_FRAMES] = {0}; // Track acknowledged frames

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int base = 0, next_frame = 0, ack;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server!\n");

    while (base < TOTAL_FRAMES) {
        // Send frames within the window
        while (next_frame < base + WINDOW_SIZE && next_frame < TOTAL_FRAMES) {
            if (!acknowledged_frames[next_frame]) { // Send only unacknowledged frames
                printf("Sending Frame %d...\n", next_frame);
                write(sock, &next_frame, sizeof(next_frame));
            }
            next_frame++;
            sleep(1); // Simulate transmission delay
        }

        // Wait for ACK
        fd_set read_fds;
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        int result = select(sock + 1, &read_fds, NULL, NULL, &timeout);

        if (result == 0) { // Timeout occurred
            printf("Timeout! Retransmitting unacknowledged frames...\n");
            next_frame = base; // Retransmit only unacknowledged frames
        } else {
            // Receive ACK
            read(sock, &ack, sizeof(ack));
            printf("ACK %d received\n", ack);
            acknowledged_frames[ack] = 1; // Mark frame as acknowledged

            // Move base forward if all lower frames are acknowledged
            while (acknowledged_frames[base] && base < TOTAL_FRAMES) {
                base++;
            }
        }
    }

    printf("All frames sent successfully!\n");
    close(sock);
    return 0;
}
