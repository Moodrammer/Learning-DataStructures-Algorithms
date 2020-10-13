/* 
    Some Notes on FloydWarshall algorithm
    - Floyd Warshall algorithm is used to solve the problem of APSP (All pair shortest path)
    - It is best used for small graphs due to its time complexity of O(V^3)
    - It is based on a dynamic programming idea of exploring all possible paths between each pair of nodes to
    find the shortest
    - It can also good at detecting the precense of negative cycles in the graph.
    - Floyd Warshall algorithm is not best used in general for finding the shortest path from a single source.
    It is better to use bfs (unweighted graphs with no negative cycles) or dijkstra (weighted graphs with no negative cycles)

    Example input :                                     10
    4 5                                           (0)-------->(3)
    0 3 10                                         |        / /|\
    0 1 5                                        5 |   -5 /    |
    1 2 3                                          |    /      | 1
    2 3 1                                         \|/|/_       |
    3 1 -5                                        (1)-------->(2)
                                                         3
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define For(i, init, n) for(int i = init; i < n; i++)

const int N = 1000;
#define oo 20000000

int nVertices, nEdges; // number of vertices
ll costs[N][N];
ll shortestDist[N][N];
int nextInPath[N][N];

void setBaseCosts(ll (&arr)[N][N]) {
    For(i, 0, nVertices) {
        For(j, 0, nVertices) {
            arr[i][j] = costs[i][j];
            if(arr[i][j] != oo) nextInPath[i][j] = j;
            else nextInPath[i][j] = oo; //No path between i and j
        }
    }
}

void propagateNegativeCycles() {
    For(intermediateVertex, 0, nVertices) {
        For(from, 0, nVertices) {
            For(to, 0, nVertices) {
                if(shortestDist[from][intermediateVertex] != oo && shortestDist[intermediateVertex][to] != oo) {
                    if(shortestDist[from][intermediateVertex] + shortestDist[intermediateVertex][to] < shortestDist[from][to]) {
                        /* 
                            - This indicates the presence of a negative cycle as the path keeps getting better even thougth
                            we are sure after applying floydWarshall once that we have reached the optimal path 
                            - We set the path to -oo to indicate that we can loop indefinitely in the negative cycle and reduce
                            the cost before eventually reaching the goal node if the node is either inside or affected by the
                            negative cycle
                        */
                    shortestDist[from][to] = -oo;
                    nextInPath[from][to] = -1;
                    }
                }
            }
        }
    }
}


/*
    . O(V^3) in time complexity
    Since we pick each vertex one by one and consider this vertex as an intermediate vertex in the shortest path
    from every two vertices i and j
    . O(V^2) in space complexity
    Since we need to store an adjacency matrix for the cost between any two vertices
*/
void floydWarshall() {
    setBaseCosts(shortestDist);
    For(intermediateVertex, 0, nVertices) {
        For(from, 0, nVertices) {
            For(to, 0, nVertices) {
                if(shortestDist[from][intermediateVertex] != oo && shortestDist[intermediateVertex][to] != oo) {
                    if(shortestDist[from][intermediateVertex] + shortestDist[intermediateVertex][to] < shortestDist[from][to]) {
                        shortestDist[from][to] = shortestDist[from][intermediateVertex] + shortestDist[intermediateVertex][to];
                        nextInPath[from][to] = nextInPath[from][intermediateVertex];
                    }
                }
            }
        }
    }
    propagateNegativeCycles();
}

/*
    Return a path if existing between the start & end nodes
*/
vector<int> reconstructPath(int from, int to) {
    vector<int> path;
    
    if(nextInPath[from][to] == oo) return path;
    // if(from == to) {
    //     if(nextInPath[from][to] == -1) {}
    // }
    
    int curr = from;
    for(;curr != to; curr = nextInPath[curr][to]) {
        if(curr == -1) {
            path.clear();
            path.push_back(-1);
            return path;
        }
        path.push_back(curr);
    }
    if(nextInPath[curr][to] == -1) {
        path.clear();
        path.push_back(-1);
        return path;
    }
    path.push_back(to);
    return path;
}

void printDistancesArray() {
    cout << "--------------------\n";
    For(from, 0, nVertices) {
        For(to, 0, nVertices) {
            if(shortestDist[from][to] == oo) cout << "oo ";
            else if(shortestDist[from][to] == -oo) cout << "-oo ";
            else cout << shortestDist[from][to] << " ";
        }
        cout << endl;
    }
}


int main() {

    cin >> nVertices >> nEdges;
    int from, to; ll cost;
    For(i, 0, nVertices) {
        For(j, 0, nVertices) {
            if(i == j) costs[i][j] = 0;
            else costs[i][j] = oo;
        }
    }
    while(nEdges--) {
        cin >> from >> to >> cost;
        costs[from][to] = cost;
    }

    floydWarshall();
    printDistancesArray();

    char option;
    while(1) {
        cout << "Press r to reconstruct a path\nPress q to exit\n";
        cin >> option;
        if(option == 'q') break;
        else if(option == 'r') { 
            cout << "Enter the path you require to construct in the form : from to\n";
            int from , to;
            cin >> from >> to;
            vector<int> path = reconstructPath(from, to);
            if(path.empty()) {
                printf("There is no Path between node %d & node %d \n", from, to);
            }
            else if(path.size() == 1 && path[0] == -1) {
                printf("This path is affected by a negative cycle \n");
            }
            else{
                for(auto v : path) {
                    cout << v << " -> ";
                }
                cout << endl;
            }
        }
    }
}