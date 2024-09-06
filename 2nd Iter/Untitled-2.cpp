#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Structure to represent a city
struct City {
    string name;
    vector<pair<int, int>> connections; // pair represents (city_index, duration)
};

// Function to find the shortest path between two cities using Dijkstra's algorithm
vector<int> dijkstra(vector<City>& cities, int start, int end) {
    int numCities = cities.size();
    vector<int> distance(numCities, INT_MAX); // Initialize distances to infinity
    vector<bool> visited(numCities, false); // Initialize visited array

    distance[start] = 0; // Distance from start city to itself is 0

    for (int i = 0; i < numCities - 1; i++) {
        int minDistance = INT_MAX;
        int minIndex;

        // Find the city with the minimum distance
        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && distance[j] <= minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }

        visited[minIndex] = true; // Mark the city as visited

        // Update the distances of the neighboring cities
        for (const auto& connection : cities[minIndex].connections) {
            int neighbor = connection.first;
            int duration = connection.second;
            if (!visited[neighbor] && distance[minIndex] != INT_MAX && distance[minIndex] + duration < distance[neighbor]) {
                distance[neighbor] = distance[minIndex] + duration;
            }
        }
    }

    return distance;
}

// Function to find the shortest paths between all cities using Floyd-Warshall algorithm
vector<vector<int>> floydWarshall(vector<City>& cities) {
    int numCities = cities.size();
    vector<vector<int>> dist(numCities, vector<int>(numCities, INT_MAX));

    // Initialize distance matrix with direct connections
    for (int i = 0; i < numCities; i++) {
        dist[i][i] = 0;
        for (const auto& connection : cities[i].connections) {
            int neighbor = connection.first;
            int duration = connection.second;
            dist[i][neighbor] = duration;
        }
    }

    // Floyd-Warshall algorithm
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

// Function to check weather and return safety status
string checkWeather(int temperature) {
    if (temperature > 25 || temperature < 5) {
        return "UNSAFE";
    } else {
        return "SAFE";
    }
}

// Function to check if the fuel is sufficient for the journey
string checkFuel(int fuel) {
    if (fuel >= 1000) {
        return "Good to Go!";
    } else {
        return "INSUFFICIENT FUEL";
    }
}

int main() {
    srand(time(0)); // Seed the random number generator

    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<City> cities(numCities);

    // Initialize city names
    for (int i = 0; i < numCities; i++) {
        cout << "Enter the name of city " << i + 1 << ": ";
        cin >> cities[i].name;
    }

    // Initialize city connections and durations with user input
    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            int duration;
            cout << "Enter the duration (in minutes) from " << cities[i].name << " to " << cities[j].name << ": ";
            cin >> duration;
            cities[i].connections.push_back(make_pair(j, duration));
            cities[j].connections.push_back(make_pair(i, duration));
        }
    }

    // Print city connections and durations
    cout << "City Connections:" << endl;
    for (int i = 0; i < numCities; i++) {
        cout << "Connections from " << cities[i].name << ":" << endl;
        for (const auto& connection : cities[i].connections) {
            int cityIndex = connection.first;
            int duration = connection.second;
            cout << cities[cityIndex].name << " (Duration: " << duration << " minutes)" << endl;
        }
        cout << endl;
    }

    // Generate and print the duration matrix
    cout << "Duration Matrix:" << endl;
    vector<vector<int>> durationMatrix(numCities, vector<int>(numCities, INT_MAX));
    for (int i = 0; i < numCities; i++) {
        for (const auto& connection : cities[i].connections) {
            int neighbor = connection.first;
            int duration = connection.second;
            durationMatrix[i][neighbor] = duration;
            durationMatrix[neighbor][i] = duration;
        }
    }

    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (durationMatrix[i][j] == INT_MAX) {
                cout << "INF\t";
            } else {
                cout << durationMatrix[i][j] << "\t";
            }
        }
        cout << endl;
    }

    int start, end;
    cout << "Enter the starting city (1-" << numCities << "): ";
    cin >> start;
    cout << "Enter the ending city (1-" << numCities << "): ";
    cin >> end;

    // Get temperature input from the user
    int temperature;
    cout << "Enter the current temperature in Celsius: ";
    cin >> temperature;

    // Get fuel input from the user
    int fuel;
    cout << "Enter the remaining fuel in liters: ";
    cin >> fuel;

    // Find the shortest path using Dijkstra
    vector<int> shortestPathDijkstra = dijkstra(cities, start - 1, end - 1);

    // Print the shortest path using Dijkstra
    cout << "Shortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << " (Dijkstra): ";
    if (shortestPathDijkstra[end - 1] == INT_MAX) {
        cout << "No path found";
    } else {
        cout << shortestPathDijkstra[end - 1] << " minutes";
    }

    // Find the shortest paths using Floyd-Warshall
    vector<vector<int>> shortestPathsFloydWarshall = floydWarshall(cities);

    // Print the shortest path using Floyd-Warshall
    cout << "\nShortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << " (Floyd-Warshall): ";
    if (shortestPathsFloydWarshall[start - 1][end - 1] == INT_MAX) {
        cout << "No path found";
    } else {
        cout << shortestPathsFloydWarshall[start - 1][end - 1] << " minutes" << endl;
    }

    string weatherStatus = checkWeather(temperature);
    cout << "Weather Status: " << weatherStatus << endl;

    string fuelStatus = checkFuel(fuel);
    cout << "Fuel Status: " << fuelStatus << endl;

    return 0;
}
