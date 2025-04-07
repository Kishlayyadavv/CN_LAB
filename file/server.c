#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080

// Function to count lines, words, and characters in a file
void analyze_file(const char *filename, int *lines, int *words, int *characters, char *max_char) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open failed");
        return;
    }

    char ch, prev = '\0';
    int freq[256] = {0};
    *lines = *words = *characters = 0;

    while ((ch = fgetc(file)) != EOF) {
        (*characters)++;
        freq[(unsigned char)ch]++;

        if (ch == '\n') (*lines)++;
        if (isspace(ch) && !isspace(prev)) (*words)++;
        prev = ch;
    }

    fclose(file);

    // Find max occurring character
    int max_freq = 0;
    *max_char = '\0';
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            *max_char = (char)i;
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char filename[100];
    int lines, words, characters;
    char max_char;

    // 1. Create socket
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
    listen(server_fd, 5);
    printf("Server listening on port %d...\n", PORT);

    // 4. Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_socket == -1) {
        perror("Connection failed");
        exit(1);
    }

    // 5. Receive filename from client
    recv(new_socket, filename, sizeof(filename), 0);
    printf("Client requested file: %s\n", filename);

    // 6. Analyze the file
    analyze_file(filename, &lines, &words, &characters, &max_char);

    // 7. Send results to client
    send(new_socket, &lines, sizeof(lines), 0);
    send(new_socket, &words, sizeof(words), 0);
    send(new_socket, &characters, sizeof(characters), 0);
    send(new_socket, &max_char, sizeof(max_char), 0);

    printf("File analysis sent to client.\n");

    // 8. Close connection
    close(new_socket);
    close(server_fd);
    return 0;
}
