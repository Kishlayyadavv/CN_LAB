#include <stdio.h>
//it usees bellman ford algorithm
#define MAX_NODES 10
#define INF 9999

typedef struct {
    int distance[MAX_NODES];  // Distance to each node
    int next_hop[MAX_NODES];  // Next hop for each destination
} RoutingTable;

RoutingTable nodes[MAX_NODES];  // Routing tables for all nodes
int cost_matrix[MAX_NODES][MAX_NODES]; // Cost matrix
int num_nodes; // Total number of nodes

// Function to initialize cost matrix
void initialize_cost_matrix() {
    printf("Enter the number of nodes: ");
    scanf("%d", &num_nodes);

    printf("Enter the cost adjacency matrix (9999 for no direct link):\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            scanf("%d", &cost_matrix[i][j]);
            if (i == j) cost_matrix[i][j] = 0; // Distance to itself is 0

            nodes[i].distance[j] = cost_matrix[i][j];
            nodes[i].next_hop[j] = (cost_matrix[i][j] != INF && i != j) ? j : -1;
        }
    }
}

// Distance Vector Algorithm (Bellman-Ford)
void distance_vector_routing() {
    int updated;
    do {
        updated = 0;

        for (int i = 0; i < num_nodes; i++) { // For each node
            for (int j = 0; j < num_nodes; j++) { // For each destination
                for (int k = 0; k < num_nodes; k++) { // Check via each neighbor
                    if (nodes[i].distance[j] > nodes[i].distance[k] + nodes[k].distance[j]) {
                        nodes[i].distance[j] = nodes[i].distance[k] + nodes[k].distance[j];
                        nodes[i].next_hop[j] = k;
                        updated = 1; // Set flag if update occurs
                    }
                }
            }
        }
    } while (updated);
}

// Function to print routing tables
void print_routing_tables() {
    for (int i = 0; i < num_nodes; i++) {
        printf("\nRouting Table for Node %d:\n", i);
        printf("Destination | Distance | Next Hop\n");
        for (int j = 0; j < num_nodes; j++) {
            printf("     %d      |    %d    |    %d\n", j, nodes[i].distance[j], nodes[i].next_hop[j]);
        }
    }
}

// Main function
int main() {
    initialize_cost_matrix();
    distance_vector_routing();
    print_routing_tables();
    return 0;
}
