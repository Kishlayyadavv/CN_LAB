#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char mac_address[20], ip_address[INET_ADDRSTRLEN];

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

    // 5. Receive client's MAC & IP address
    recv(new_socket, mac_address, sizeof(mac_address), 0);
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_address, INET_ADDRSTRLEN);

    // 6. Display client details
    printf("\nClient Connected!\n");
    printf("IP Address: %s\n", ip_address);
    printf("MAC Address: %s\n", mac_address);

    // 7. Close connection
    close(new_socket);
    close(server_fd);
    return 0;
}
