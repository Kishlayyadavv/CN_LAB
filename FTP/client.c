#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char filename[100];
    int lines, words, characters;
    char max_char;

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    // 4. Get filename from user
    printf("Enter the filename: ");
    scanf("%s", filename);

    // 5. Send filename to server
    send(sock, filename, sizeof(filename), 0);

    // 6. Receive results from server
    recv(sock, &lines, sizeof(lines), 0);
    recv(sock, &words, sizeof(words), 0);
    recv(sock, &characters, sizeof(characters), 0);
    recv(sock, &max_char, sizeof(max_char), 0);

    // 7. Display results
    printf("\nFile Analysis:\n");
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);
    printf("Most Frequent Character: %c\n", max_char);

    // 8. Close connection
    close(sock);
    return 0;
}
