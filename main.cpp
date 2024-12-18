#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
using namespace std;

const int SIZE = 9;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

string stationName[] = {
    "Central Power Station",
    "East Power Station",
    "West Power Station",
    "South Power Station",
    "North Power Station",
    "Backup Generator",
    "Transformer Station A",
    "Transformer Station B",
    "Solar Power Plant"
};

class Graph
{
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;
    vector<Edge> edges;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));

            this->edges.push_back(edge);
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Power Grid Network Topology:" << endl;
        cout << "================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Power Station " << i << " (" << stationName[i] << ") connects to: " << endl;
            for (Pair v : adjList[i])
                cout << "    Station " << v.first << " (Capacity: " << v.second << " MW) " << endl;
        }
        cout << endl;
    }

    void DFS(int start) {
        vector<bool> visit(SIZE, false);
        stack<int> s;
        s.push(start);

        cout << "Network Trace (DFS) from station " << start << " (" << stationName[start] << "):" << endl;
        cout << "Purpose: Tracking possible power flow paths through the network" << endl;
        cout << "================================" << endl;

        while (!s.empty()) {
            int vertex = s.top();
            s.pop();

            if (!visit[vertex]) {
                cout << "Inspecting Station " << vertex << " (" << stationName[vertex] << ")" << endl;
                visit[vertex] = true;
            }

            for (auto it = adjList[vertex].begin(); it != adjList[vertex].end(); it++) {
                if (!visit[it->first]) {
                    cout << "    Potential spread to Station " << it->first << " (" << stationName[it->first] 
                        << ") - " << "Capacity: " << it->second << " MW" << endl;
                    s.push(it->first);
                }
            }
        }
        cout << endl;
    }

    void BFS(int start) {
        vector<bool> visit(SIZE, false);
        queue<int> q;
        q.push(start);
        visit[start] = true;

        cout << "Layer-by-Layer Network Inspection (BFS) from Station " << start << " (" << stationName[start] << "):" << endl;
        cout << "Purpose: Analyzing service areas by distance from source" << endl;
        cout << "================================" << endl;

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();
            cout << "Checking Station " << vertex << " (" << stationName[vertex] << ")" << endl;

            for (auto &neighbor : adjList[vertex]) {
                if (!visit[neighbor.first]) {
                    visit[neighbor.first] = true;
                    cout << "    Next service area: Station " << neighbor.first << " (" << stationName[neighbor.first] 
                        << ") - Capacity: " << neighbor.second << " MW" << endl;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    void dijkstra(int start) {
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        vector<int> dist(SIZE, numeric_limits<int>::max());

        pq.push(make_pair(0, start));
        dist[start] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        cout << "Shortest path from station " << start << " (" << stationName[start] << "):" << endl;
        for (int i = 0; i < SIZE; i++) {
            cout << start << " -> " << i << " : " << dist[i] << " MW" << endl;
        }
        cout << endl;
    }

    int find(int i, vector<int> &parent) {
        if (parent[i] == -1)
            return i;
        return parent[i] = find(parent[i], parent);
    }

    void kruskalsMST() {
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
            return a.weight < b.weight;
        });

        vector<int> parent(SIZE, -1);

        cout << "Minimum Spanning Tree edges:" << endl;
        for (auto &edge : edges) {
            int w = edge.weight;
            int x = edge.src;
            int y = edge.dest;

            int s1 = find(x, parent);
            int s2 = find(y, parent);

            if (s1 != s2) {
                parent[s1] = s2;
                cout << "Edge from " << x << " to " << y << " with capacity: " << w << " units" << endl;
            }
        }
        cout << endl;
    }

    void menu() {
        int choice;
        do {
            cout << "Water Distribution Network Menu:" << endl;
            cout << "[1] Display water distribution network" << endl;
            cout << "[2] Check contaminant spread (BFS)" << endl;
            cout << "[3] Plan inspection route (DFS)" << endl;
            cout << "[4] Calculate shortest paths" << endl;
            cout << "[5] find Minimum Spanning Tree" << endl;
            cout << "[0] Exit" << endl;
            cout << "enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << endl;
                    printGraph();
                    break;
                case 2:
                    cout << endl;
                    BFS(0);
                    break;
                case 3:
                    cout << endl;
                    DFS(0);
                    break;
                case 4:
                    cout << endl;
                    dijkstra(0);
                    break;
                case 5:
                    cout << endl;
                    kruskalsMST();
                    break;
                case 0:
                    cout << endl;
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << endl;
                    cout << "Invalid choice, try again" << endl;
                    cout << endl;
            }
        } while (choice != 0);
    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        //{2,4,4},{2,5,5} deleted, {3,1,2},{3,8,10},{4,1,9},{4,6,2},{1,1,2},{1,0,7} added
        {0,1,12},{0,2,8},{0,3,21},{2,3,6},{2,6,2},{5,6,6},{4,5,9},{3,1,2},{3,8,10},{4,1,9},{4,6,2},{1,1,2},{7,0,7}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.menu();

    return 0;
}
