#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define MAX_PACKETS 10 // Number of cycles for simulation

// Function to simulate the leaky bucket algorithm
void leaky_bucket(int bucket_size, int output_rate, int input_packets[], int n) {
    int bucket_content = 0;

    printf("\nCycle | Incoming | Stored | Sent | Dropped\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        bucket_content += input_packets[i]; // Add incoming packets

        // If bucket overflows, drop excess packets
        if (bucket_content > bucket_size) {
            printf("  %d   |    %d     |   %d   |  %d  |   %d  \n", 
                i + 1, input_packets[i], bucket_size, output_rate, bucket_content - bucket_size);
            bucket_content = bucket_size; // Store max allowed packets
        } else {
            printf("  %d   |    %d     |   %d   |  %d  |   0  \n", 
                i + 1, input_packets[i], bucket_content, output_rate);
        }

        // Transmit packets at the output rate
        bucket_content -= output_rate;
        if (bucket_content < 0) bucket_content = 0; // Avoid negative bucket content
        
        sleep(1); // Simulate time delay
    }

    // Drain remaining packets
    while (bucket_content > 0) {
        printf("  -   |    0     |   %d   |  %d  |   0  \n", bucket_content, output_rate);
        bucket_content -= output_rate;
        if (bucket_content < 0) bucket_content = 0;
        sleep(1);
    }
}

int main() {
    int bucket_size, output_rate, n;
    
    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);
    
    printf("Enter output rate (fixed transmission rate): ");
    scanf("%d", &output_rate);
    
    printf("Enter number of packet entries: ");
    scanf("%d", &n);
    
    int input_packets[n];
    printf("Enter incoming packets for %d cycles: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &input_packets[i]);
    }

    leaky_bucket(bucket_size, output_rate, input_packets, n);
    
    return 0;
}
