#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <random>

using namespace std;

// Structure to represent a city
struct City {
    string name;
    vector<pair<int, int>> connections; // pair represents (city_index, flight_duration)
};

// Function to generate a random airport name
string generateRandomAirportName() {
    static const string airportNames[] = {
        "John F. Kennedy International Airport",
        "Los Angeles International Airport",
        "Chicago O'Hare International Airport",
        "Hartsfield-Jackson Atlanta International Airport",
        "Dallas/Fort Worth International Airport",
        "Denver International Airport",
        "Heathrow Airport",
        "Beijing Capital International Airport",
        "Tokyo Haneda Airport",
        "Dubai International Airport"
    };

    int numAirportNames = sizeof(airportNames) / sizeof(airportNames[0]);
    int randomIndex = rand() % numAirportNames;

    return airportNames[randomIndex];
}

// Function to generate a random flight name
string generateRandomFlightName() {
    static const string airlines[] = {
        "Delta Air Lines",
        "American Airlines",
        "United Airlines",
        "Southwest Airlines",
        "Emirates",
        "Air China",
        "British Airways",
        "Japan Airlines",
        "Qantas",
        "Lufthansa"
    };

    int numAirlines = sizeof(airlines) / sizeof(airlines[0]);
    int randomAirlineIndex = rand() % numAirlines;

    string airportName = generateRandomAirportName();

    ostringstream flightNameStream;
    flightNameStream << airlines[randomAirlineIndex] << " Flight to " << airportName;

    return flightNameStream.str();
}

// Function to find the shortest path between two cities using Dijkstra's algorithm
vector<int> dijkstra(vector<City>& cities, int start, int end, vector<int>& prev) {
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
                prev[neighbor] = minIndex;
            }
        }
    }

    return distance;
}

// Function to find the shortest paths between all cities using Floyd-Warshall algorithm
vector<vector<int>> floydWarshall(vector<City>& cities, vector<vector<int>>& prev) {
    int numCities = cities.size();
    vector<vector<int>> dist(numCities, vector<int>(numCities, INT_MAX));

    // Initialize distance matrix with direct connections
    for (int i = 0; i < numCities; i++) {
        dist[i][i] = 0;
        for (const auto& connection : cities[i].connections) {
            int neighbor = connection.first;
            int duration = connection.second;
            dist[i][neighbor] = duration;
            prev[i][neighbor] = i;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < numCities; k++) {
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    prev[i][j] = prev[k][j];
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

// Function to print the path from start to end city using Dijkstra
void printDijkstraPath(vector<City>& cities, int start, int end, const vector<int>& prev) {
    if (start != end && prev[end] != -1) {
        printDijkstraPath(cities, start, prev[end], prev);
        cout << " -> ";
    }
    cout << cities[end].name;
}

// Function to print the path from start to end city using Floyd-Warshall
void printFloydWarshallPath(vector<City>& cities, int start, int end, const vector<vector<int>>& prev) {
    if (start != end && prev[start][end] != -1) {
        printFloydWarshallPath(cities, start, prev[start][end], prev);
        cout << " -> ";
    }
    cout << cities[end].name;
}

int main() {
    srand(time(0)); // Seed the random number generator

    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<City> cities(numCities);

    // Initialize city names
    for (int i = 0; i < numCities; i++) {
        cities[i].name = generateRandomAirportName();
    }

    // Initialize city connections and flight durations with random values
    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            int duration = rand() % 400 + 100; // Random flight duration between 100 and 499 minutes
            cities[i].connections.push_back(make_pair(j, duration));
            cities[j].connections.push_back(make_pair(i, duration));
        }
    }

    // Print city connections and flight durations
    cout << "City Connections:" << endl;
    for (int i = 0; i < numCities; i++) {
        cout << "Connections from " << cities[i].name << ":" << endl;
        for (const auto& connection : cities[i].connections) {
            int cityIndex = connection.first;
            int duration = connection.second;
            cout << cities[cityIndex].name << " (Flight Duration: " << duration << " minutes)" << endl;
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
    vector<int> prev(numCities, -1);
    vector<int> shortestPathDijkstra = dijkstra(cities, start - 1, end - 1, prev);

    // Print the shortest path using Dijkstra
    cout << "Shortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << " (Dijkstra): ";
    if (shortestPathDijkstra[end - 1] == INT_MAX) {
        cout << "No path found";
    } else {
        printDijkstraPath(cities, start - 1, end - 1, prev);
        cout << " (" << shortestPathDijkstra[end - 1] << " minutes)";
    }

    // Find the shortest paths using Floyd-Warshall
    vector<vector<int>> prevFloyd(numCities, vector<int>(numCities, -1));
    vector<vector<int>> shortestPathsFloydWarshall = floydWarshall(cities, prevFloyd);

    // Print the shortest path using Floyd-Warshall
    cout << "\nShortest path from " << cities[start - 1].name << " to " << cities[end - 1].name << " (Floyd-Warshall): ";
    if (shortestPathsFloydWarshall[start - 1][end - 1] == INT_MAX) {
        cout << "No path found";
    } else {
        printFloydWarshallPath(cities, start - 1, end - 1, prevFloyd);
        cout << " (" << shortestPathsFloydWarshall[start - 1][end - 1] << " minutes)" << endl;
    }

    string weatherStatus = checkWeather(temperature);
    cout << "Weather Status: " << weatherStatus << endl;

    string fuelStatus = checkFuel(fuel);
    cout << "Fuel Status: " << fuelStatus << endl;

    return 0;
}
