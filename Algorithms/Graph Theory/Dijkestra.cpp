/*Implementation for the Dijkestra algorithm :
==============================================
    Input :
    =======
    weighted Directed graph with non-negative cycles + start node
    Output :
    ======= 
    - distance from start node to all other nodes
    - Printing the Path from the End nodes to the start node
    
    Complexity : when using the min heap O(v + elog(v))

    Simple explantion :
    ===================
     - We extract the node with the min distance from the min heap
     - We ignore this node if it is visited before (No path is more optimal for a node after visiting it)
     - if not visited :
        -> Mark the node as visited
        -> expand the node as follows - for every edge reachable from the current node
            - Check if (distance till current node + weight on that edge < distance to the next node)
                set the new distance to the next node with the smaller distance
                Push the new node in the min heap
============================================================================================================
    Sample Input :
        v e for number of vertices & number of edges
        5 6
        from to weight
        1 2 10
        1 3 4
        1 5 50
        3 5 60
        3 4 30
        2 4 20
    Sample Output :
        ============================================
        Distances array
        0 10 4 30 50
        ============================================
        Paths
        1 <-
        1 <- 2 <- 
        1 <- 3 <-
        1 <- 2 <- 4 <-
        1 <- 5 <-
===========================================================================================================
*/

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<pair<int, ll>> vpil;
typedef pair<ll, int> pli;
#define IO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

const int N = 100001;

struct node {
    int index; 
    ll distance; 
    int parent;

    node() : index(-1), distance(-1), parent(-1) {}
    node(int i, ll d, int p) : index(i), distance(d), parent(p) {}
};

class compareDist {
    public:
        bool operator() (node &a, node &b) {
            return a.distance < b.distance;
        }
};

int v, e;

vpil edges[N];
bool vis[N] = {0};

ll dis[N];
int par[N];

priority_queue<node, vector<node>, compareDist> open;

void dijkestra(int s) {
    for(int i = 1; i <= v; dis[i++] = 2000000000);
    dis[s] = 0;
    open.push(node(s, 0, -1));
    node curr; 
    ll childDist, parentDist, weight;
    int parentIdx, childIdx;
    while(!open.empty()) {
        curr = open.top();
        parentIdx = curr.index;
        parentDist = -curr.distance;
        open.pop();

        if(vis[parentIdx]) continue;

        vis[parentIdx] = 1;
        par[parentIdx] = curr.parent;

        for(auto child : edges[parentIdx]) {
            childIdx = child.first;
            weight = child.second;
            childDist = dis[parentIdx] + weight;
            if(childDist < dis[childIdx]) {
                dis[childIdx] = childDist;
                open.push(node(childIdx, -1 * dis[childIdx], parentIdx));
            }
        }
    }
}

void printPath(int fr) {
    if(par[fr] != -1) printPath(par[fr]);
    cout << fr << " <- ";
}

void read() {
    cin >> v >> e;
    int fr, to, we;
    while(e--) {
        cin >> fr >> to >> we;
        edges[fr].push_back(make_pair(to, we));
        // edges[to].push_back(make_pair(fr, we));
    }
}

int main() {
    IO;
    read();

    dijkestra(1);

    cout << endl;
    cout << "============================================" << endl;
    cout << "Distances array" << endl;

    for(int i = 1; i <= v; i++) cout << dis[i] << " ";
    cout << endl;

    cout << "============================================" << endl;
    cout << "Paths" << endl;
    for(int i = 1; i <= v; i++) {
        printPath(i);
        cout << endl;
    }
}