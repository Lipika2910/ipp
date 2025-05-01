#include <iostream>
#include <climits>
#include <omp.h>
using namespace std;

#define V 9  // You can change V based on your graph size

// Parallel function to find the vertex with the minimum distance value
int minDistance(int dist[], bool sptSet[]) {
   int min = INT_MAX;
    int min_index = -1;

    #pragma omp parallel
    {
    	#pragma omp single
        int local_min = INT_MAX;
        int local_index = -1;

        #pragma omp for nowait
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && dist[v] <= local_min) {
                local_min = dist[v];
                local_index = v;
            }
        }

        #pragma omp critical
        {
            if (local_min < min) {
                min = local_min;
                min_index = local_index;
            }
        }
    }

    return min_index;
}

// Parallel Dijkstra's algorithm
void dijkstra(int graph[V][V], int src) {
    int dist[V];    // dist[i] will hold the shortest distance from src to i
    bool sptSet[V]; // sptSet[i] will be true if vertex i is included in the shortest path tree

    // Initialize all distances as INFINITE and sptSet[] as false
    #pragma omp parallel for
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0; // Distance of source vertex from itself is 0

    // Find shortest path for all vertices
    #pragma omp parallel for
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex
        int u = minDistance(dist, sptSet);
        if (u == -1) break;  // No vertex reachable

        sptSet[u] = true;  // Mark vertex as processed

        // Update distance value of adjacent vertices of the picked vertex
        #pragma omp parallel for schedule(static)
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Print the constructed distance array
    cout << "Vertex \t Distance from Source\n";
    #pragma omp parallel for
    for (int i = 0; i < V; i++) {
        cout << i << " \t " << dist[i] << "\n";
    }
}

int main() {
    int graph[V][V] = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0}, 
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
        
    };

    int source = 0;
    dijkstra(graph, source);

    return 0;
}