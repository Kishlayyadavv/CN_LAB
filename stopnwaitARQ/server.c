#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int expected_seq = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    socklen_t len = sizeof(client_addr);
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr*)&client_addr, &len);

        int seq = buffer[0] - '0'; // First char is seq
        char* msg = buffer + 2;    // After space

        printf("Received frame with seq: %d, message: %s\n", seq, msg);

        if (seq == expected_seq) {
            printf("Frame accepted. Sending ACK%d\n", seq);
            char ack[BUFFER_SIZE];
            sprintf(ack, "ACK%d", seq);
            sendto(sockfd, ack, strlen(ack), 0, (struct sockaddr*)&client_addr, len);
            expected_seq = 1 - expected_seq; // Flip expected seq
        } else {
            // Resend last ACK (duplicate)
            printf("Unexpected seq. Resending ACK%d\n", 1 - expected_seq);
            char ack[BUFFER_SIZE];
            sprintf(ack, "ACK%d", 1 - expected_seq);
            sendto(sockfd, ack, strlen(ack), 0, (struct sockaddr*)&client_addr, len);
        }
    }

    close(sockfd);
    return 0;
}
