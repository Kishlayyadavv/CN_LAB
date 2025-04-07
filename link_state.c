// this uses djikstra's algorithm to find the shortest path between two nodes
#include <stdio.h>
#include <limits.h>

#define MAX_NODES 10
#define INF 9999

int num_nodes; // Total number of nodes
int cost_matrix[MAX_NODES][MAX_NODES]; // Graph represented as an adjacency matrix

// Function to find the node with the minimum distance value
int min_distance(int distance[], int visited[]) {
    int min = INF, min_index = -1;
    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i] && distance[i] < min) {
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

// Dijkstra's Algorithm: Computes shortest paths from source
void dijkstra(int source) {
    int distance[MAX_NODES]; // Shortest distance from source
    int visited[MAX_NODES] = {0}; // 1 if node is visited
    int parent[MAX_NODES]; // Stores the previous node in the shortest path

    // Initialize distances and parent nodes
    for (int i = 0; i < num_nodes; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[source] = 0;

    for (int count = 0; count < num_nodes - 1; count++) {
        int u = min_distance(distance, visited);
        if (u == -1) break; // No more reachable nodes
        visited[u] = 1;

        // Update distances of adjacent nodes
        for (int v = 0; v < num_nodes; v++) {
            if (!visited[v] && cost_matrix[u][v] != INF && distance[u] + cost_matrix[u][v] < distance[v]) {
                distance[v] = distance[u] + cost_matrix[u][v];
                parent[v] = u;
            }
        }
    }

    // Print shortest paths
    printf("\nShortest paths from node %d:\n", source);
    printf("Destination | Distance | Path\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("     %d      |    %d    | ", i, distance[i]);
        int temp = i;
        while (temp != -1) {
            printf("%d ", temp);
            temp = parent[temp];
            if (temp != -1) printf("<- ");
        }
        printf("\n");
    }
}

// Function to take input and initialize the cost matrix
void initialize_cost_matrix() {
    printf("Enter the number of nodes: ");
    scanf("%d", &num_nodes);

    printf("Enter the cost adjacency matrix (9999 for no direct link):\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            scanf("%d", &cost_matrix[i][j]);
            if (i == j) cost_matrix[i][j] = 0; // Distance to itself is 0
        }
    }
}

// Main function
int main() {
    initialize_cost_matrix();
    int source;
    printf("Enter the source node: ");
    scanf("%d", &source);
    dijkstra(source);
    return 0;
}
