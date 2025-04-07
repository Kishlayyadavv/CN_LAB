#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define PORT 8080
#define INTERFACE "eth0" // Change to "wlan0" if using Wi-Fi

// Function to get MAC Address
void get_mac_address(char *mac_str) {
    struct ifreq ifr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket error");
        exit(1);
    }

    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) {
        perror("MAC address fetch failed");
        close(sock);
        exit(1);
    }
    close(sock);

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(mac_str, 20, "%02X:%02X:%02X:%02X:%02X:%02X", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char mac_address[20];

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

    // 4. Get and send MAC address
    get_mac_address(mac_address);
    send(sock, mac_address, sizeof(mac_address), 0);

    printf("MAC Address sent to server: %s\n", mac_address);

    // 5. Close connection
    close(sock);
    return 0;
}
