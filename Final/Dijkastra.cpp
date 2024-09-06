#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// City
struct City {
    string name;
    vector<int> connections;
};

// Duration between two cities
int generateDuration() {
    return rand() % 100 + 1; // Random duration between 1 and 100 minutes
}

// Matrix between all cities
vector<vector<int>> generateMatrix(int numCities) {
    vector<vector<int>> matrix(numCities, vector<int>(numCities, 0)); // Initialize matrix with 0 s

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

    // Find the shortest path
    vector<int> shortestPath = dijkstra(matrix, start - 1, end - 1);

    // Print the shortest path
    cout << "Shortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << ": ";
    if (shortestPath[end - 1] == INT_MAX) {
        cout << "No path found";
    } else {
        cout << shortestPath[end - 1] << " minutes";
    }

    return 0;
}
