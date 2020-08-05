#include "vector"
#include "iostream"

using namespace std;


/*
Tested input:
13
2 1 9
2 0 8
1 3
3 2 4 5
1 3
2 3 6
2 7 5
5 8 3 6 10 11
3 1 9 7
2 0 8
2 7 11
2 7 10
0
*/

class graph {
    // represent the graph as an adjacency list
    int numberOfNodes;
    // for simplicity consider the nodes to contain only integers and the graph to be unweighted
    vector<vector<int>> adjacencyList;
    // keep track of visited and unvisited nodes
    vector<bool> visited;
    // used to count the keep track of the connected components in the graph
    vector<int> connectedComponents;
    int countComponents = 0;


    public:
    graph(int numberOfNodes) {
        this->numberOfNodes = numberOfNodes;
        visited.resize(numberOfNodes);
        for(int i = 0; i < visited.size(); i++) visited[i] = 0;
        adjacencyList.resize(numberOfNodes);
        connectedComponents.resize(numberOfNodes);
    }
    // considering it to be an undirected graph
    void connect(int from, int to) {
        adjacencyList[from].push_back(to);
    }

    /*
    Note that dfs is not useful alone , but may be useful in many other applications when augmented with 
    other tasks during the dfs process like counting the number of connected components in the graph
    */
    void dfs(int currentNode) {
        // if the node is already visited backtrack till the returning to the first node in the path
        // then continuing to its next path arbitrarily
        // else if it is the first time to visit the node, mark that node as visited
        visited[currentNode] = 1;
        connectedComponents[currentNode] = countComponents;

        vector<int> connections = adjacencyList[currentNode];
        for(auto node : connections) {
            if(!visited[node]) {
                cout << "From: " << currentNode << " to: " << node << endl;
                dfs(node);
            }
        }
    }

    // utility functions
    void resetVisited() {
        for(int i = 0; i < visited.size(); i++) visited[i] = 0;
    }
    /*
        The idea is to label each group of connected Nodes (component) by a number:
        This is done by applying a dfs to each node if it is not visited yet giving it a certain label 
        and marking all the visited nodes during the traversal as visited giving it the same label as the
        starting node, then looping over all nodes to reach the unvisited nodes yet because they are not 
        connected to the graph
    */
    int countConnectedComponents() {
        for(int i = 0; i < numberOfNodes; i++) {
            // if the current node is not visited before, apply a dfs to the current node which will 
            // reach all the connected nodes to that current node
            if(!visited[i]) {
                dfs(i);
                countComponents++;
            }
        }
        return countComponents;
    }

    void printConnections() {
        for(int i = 0; i < numberOfNodes; i++) {
            cout << "Node " << i << " : is connected to label : "<< connectedComponents[i] << endl;
        }
    }
};

int main() {
    /* The input is describe as follows 
    - n is the number of Nodes in the graph 
    - Following n lines each with the connections from n to the other Nodes
    as follows the number of connections followed by the nodes to connect
    */
   int numberOfNodes, numberOfConnections, node; 
   cin >> numberOfNodes;
   graph g(numberOfNodes);
   for(int i = 0; i < numberOfNodes; i++) {
       cin >> numberOfConnections;
       while(numberOfConnections--) {
           cin >> node;
           g.connect(i, node);
       }
   }

   g.dfs(0);

   g.resetVisited();
   cout << "Num Connected : " << g.countConnectedComponents() << endl;
   g.printConnections();
}