#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 1000
#define INF 1000000

// Structure to represent the graph
typedef struct {
    int **adjMatrix;   // Adjacency matrix
    int numNodes;      // Number of nodes
} Graph;

void bfs_parallel(Graph *graph, int startNode) {
    int *visited = (int *)malloc(graph->numNodes * sizeof(int));
    int *queue = (int *)malloc(graph->numNodes * sizeof(int));
    int front = 0, rear = 0;

    // Initialize the visited array and the queue
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = 0;
    }

    visited[startNode] = 1;  // Mark the start node as visited
    queue[rear++] = startNode;  // Enqueue the start node

    while (front < rear) {
        int currentNode = queue[front++];
        printf("Visited Node: %d\n", currentNode);

        // Parallelize the exploration of neighbors
        #pragma omp parallel for
        for (int i = 0; i < graph->numNodes; i++) {
            if (graph->adjMatrix[currentNode][i] == 1 && !visited[i]) {
                visited[i] = 1;
                #pragma omp critical
                {
                    queue[rear++] = i; // Enqueue the unvisited neighbors
                }
            }
        }
    }

    free(visited);
    free(queue);
}

// Utility function to create a graph
Graph *createGraph(int numNodes) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numNodes = numNodes;
    graph->adjMatrix = (int **)malloc(numNodes * sizeof(int *));
    for (int i = 0; i < numNodes; i++) {
        graph->adjMatrix[i] = (int *)malloc(numNodes * sizeof(int));
        for (int j = 0; j < numNodes; j++) {
            graph->adjMatrix[i][j] = 0;  // Initialize adjacency matrix
        }
    }
    return graph;
}

// Utility function to add an edge
void addEdge(Graph *graph, int u, int v) {
    graph->adjMatrix[u][v] = 1;
    graph->adjMatrix[v][u] = 1;
}

int main() {
    int numNodes = 6;
    Graph *graph = createGraph(numNodes);

    // Adding edges to the graph (undirected graph)
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);

    printf("BFS starting from node 0:\n");
    bfs_parallel(graph, 0);

    // Cleanup
    for (int i = 0; i < numNodes; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);

    return 0;
}

