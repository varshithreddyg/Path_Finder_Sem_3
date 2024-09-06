#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;

// Structure to represent a city
struct City {
    string name;
    vector<int> connections;
};

// Function to generate a random duration between two cities
int generateDuration() {
    return rand() % 100 + 1; // Random duration between 1 and 100 minutes
}

// Function to generate the matrix between all cities
vector<vector<int>> generateMatrix(int numCities) {
    vector<vector<int>> matrix(numCities, vector<int>(numCities, 0)); // Initialize matrix with 0s

    // Generate random durations between cities
    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            int duration = generateDuration();
            matrix[i][j] = duration;
            matrix[j][i] = duration;
        }
    }

    return matrix;
}
// Shortest path between two cities using Dijkstra's algorithm
vector<int> dijkstra(vector<vector<int>>& matrix, int start, int end) {
    int numCities = matrix.size();
    vector<int> distance(numCities, INT_MAX); // Initialize distances to infinity
    vector<bool> visited(numCities, false); // Initialize visited array

    distance[start] = 0; // Distance from start city to itself is 0

    for (int i = 0; i < numCities - 1; i++) {
        int minDistance = INT_MAX;
        int minIndex;

        // Minimum distance
        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && distance[j] <= minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }

        visited[minIndex] = true; // Mark the city as visited

        // Update the distances of the neighboring cities
        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && matrix[minIndex][j] && distance[minIndex] != INT_MAX && distance[minIndex] + matrix[minIndex][j] < distance[j]) {
                distance[j] = distance[minIndex] + matrix[minIndex][j];
            }
        }
    }

    return distance;
}

// Heuristic function for A* (assuming straight-line distance as heuristic)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<int> astar(vector<vector<int>>& matrix, int start, int end) {
    int numCities = matrix.size();
    vector<int> distance(numCities, INT_MAX);
    vector<bool> visited(numCities, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == end) break;

        if (!visited[u]) {
            visited[u] = true;
            for (int v = 0; v < numCities; ++v) {
                if (matrix[u][v] != 0 && distance[u] != INT_MAX && distance[u] + matrix[u][v] < distance[v]) {
                    distance[v] = distance[u] + matrix[u][v];
                    pq.push({distance[v] + heuristic(u, v, end, end), v});
                }
            }
        }
    }

    return distance;
}

vector<vector<int>> floydWarshall(vector<vector<int>>& matrix) {
    int numCities = matrix.size();
    vector<vector<int>> dist(numCities, vector<int>(numCities, 0));

    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (matrix[i][j] != 0) {
                dist[i][j] = matrix[i][j];
            } else {
                dist[i][j] = INT_MAX;
            }
        }
    }

    for (int k = 0; k < numCities; k++) {
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return dist;
}

int main() {
    srand(time(0)); // Seed the random number generator

    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<City> cities(numCities);

    // Generate matrix and city names
    vector<vector<int>> matrix = generateMatrix(numCities);
    for (int i = 0; i < numCities; i++) {
        cout << "Enter the name of city " << i + 1 << ": ";
        cin >> cities[i].name;
    }

    // Print the matrix
    cout << "Matrix:" << endl;
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    int start, end;
    cout << "Enter the starting city (1-" << numCities << "): ";
    cin >> start;
    cout << "Enter the ending city (1-" << numCities << "): ";
    cin >> end;

    // Find the shortest path using Dijkstra's algorithm
    vector<int> shortestPathDijkstra = dijkstra(matrix, start - 1, end - 1);

    // Find the shortest path using A* algorithm
    vector<int> shortestPathAstar = astar(matrix, start - 1, end - 1);

    // Find the shortest paths using Floyd-Warshall algorithm
    vector<vector<int>> shortestPathsFloydWarshall = floydWarshall(matrix);

    // Print the shortest paths
    cout << "Shortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << ":\n";
    if (shortestPathDijkstra[end - 1] == INT_MAX) {
        cout << "No path found (Dijkstra)\n";
    } else {
        cout << "Dijkstra: " << shortestPathDijkstra[end - 1] << " minutes\n";
    }

    if (shortestPathAstar[end - 1] == INT_MAX) {
        cout << "No path found (A*)\n";
    } else {
        cout << "A*: " << shortestPathAstar[end - 1] << " minutes\n";
    }

    if (shortestPathsFloydWarshall[start - 1][end - 1] == INT_MAX) {
        cout << "No path found (Floyd-Warshall)\n";
    } else {
        cout << "Floyd-Warshall: " << shortestPathsFloydWarshall[start - 1][end - 1] << " minutes\n";
    }

    return 0;
}
