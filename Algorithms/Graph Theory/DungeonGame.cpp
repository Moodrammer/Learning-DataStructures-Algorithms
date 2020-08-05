/*
Sample dungeon input :
4 4
S#..
....
..#.
...E
output: 
S # . .
* . . .
* . # .
* * * E
where:
S : The point where the player starts
E : The Exit the player wants to reach
. : The cell is empty , the player can move to it
# : The cell contains rocks, the player can't move to it

The idea is representing the dungeon as an unweighted graph then using bfs
to find the shortest path in the unweighted graph



    +---+---+---+---+                                +---+---+---+---+
    | S | X |   |   |                                | S | X | 4 |   |
    +---+---+---+---+                                +---+---+---+---+
    |   |   |   |   |                                | 1 | 2 | 3 | 4 |
    +---+---+---+---+                                +---+---+---+---+
    |   |   | X |   |                                | 2 | 3 | X | 5 |
    +---+---+---+---+                                +---+---+---+---+
    |   |   |   | E | --> Freedom                    | 3 | 4 | 5 | 6 | --> Freedom
    +---+---+---+---+                                +---+---+---+---+

*/

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define vvc vector<vector<char>>
#define vvb vector<vector<bool>>
#define vvi vector<vector<int>>

//directional vectors {north, south, east ,west}
int dr[] = {-1, +1, 0, 0};
int dc[] = {0, 0, +1, -1};
// queues for keeping the dimensions during the traversal
queue<int> x, y;
// variables for the dungeon
int dungeonRows, dungeonCols;
bool endReached = false;
int numberLeftInLayer = 1; // initialized to 1 as the first layer always consists of 1 node
int numberInNextLayer = 0;

int moveCount = 0;

void exploreSurroundings(int r, int c, vvc &dungeon, vvb &visited, vvi &prevR, vvi &prevC) {
    int rr, cc;
    for(int i = 0; i < 4; i++) {
        rr = r + dr[i];
        cc = c + dc[i];
        // check not going out of bounds of the grid
        if(rr < 0 || cc < 0) continue;
        if(rr >= dungeonRows || cc >= dungeonCols) continue;
        // don't push if the cell is filled with rocks
        if(dungeon[rr][cc] == '#') continue;
        // don't push if the cell is already visited before
        if(visited[rr][cc]) continue;
        // The node is being visited for the first time
        // 1- mark the node as visited
        visited[rr][cc] = 1;
        // 2- set the previous coordinates to be able to reconstruct the path
        prevR[rr][cc] = r;
        prevC[rr][cc] = c;
        // 3- push to the queue
        x.push(rr);
        y.push(cc);
        // 4- Increment the number of nodes in the current layer
        numberInNextLayer++;
    }
}

void reconstructPath(vvc &d, vvi &pr, vvi &pc, int er, int ec) {
    int r = pr[er][ec], c = pc[er][ec];
    while(r != -1 && c != -1) {
        d[r][c] = '*';
        r = pr[r][c];
        c = pc[r][c];
    }
}

int main() {
    cout << "Enter the number of rows & Columns in the dungeon: " << endl;
    cin >> dungeonRows >> dungeonCols;
    
    vvc dungeon(dungeonRows, vector<char>(dungeonCols));
    vvb visited(dungeonRows, vector<bool>(dungeonCols, 0));
    vvi prevX(dungeonRows, vector<int>(dungeonCols, -1));
    vvi prevY(dungeonRows, vector<int>(dungeonCols, -1));

    int startRow, startCol, endRow, endCol;
    cout << "Enter the dungeon grid : " << endl;
    for(int r = 0; r < dungeonRows; r++) {
        for(int c = 0; c < dungeonCols; c++) {
            cin >> dungeon[r][c];
            if(dungeon[r][c] == 'S') {
                startRow = r;
                startCol = c;
            }
        } 
    }

    // Push the starting indices to the queue
    x.push(startRow); y.push(startCol);
    visited[startRow][startCol] = 1;
    int r, c;
    while(!(x.empty())) {
        r = x.front();
        c = y.front();
        x.pop(); y.pop();
        // if we reached our goal (The end), then we should break out of the loop
        if(dungeon[r][c] == 'E') {
            endReached = 1;
            endRow = r;
            endCol = c;
            break;
        }
        exploreSurroundings(r, c, dungeon, visited, prevX, prevY);
        numberLeftInLayer--;
        if(numberLeftInLayer == 0) {
            numberLeftInLayer = numberInNextLayer;
            numberInNextLayer = 0;
            moveCount++;
        }
    }
    if(endReached) {
        reconstructPath(dungeon, prevX, prevY, endRow, endCol);
        cout << "The number of movements needed = " << moveCount << endl;
        cout << "The Path taken represented by * : " << endl;
        for(int r = 0; r < dungeonRows; r++) {
            for(int c = 0; c < dungeonCols; c++) {
                cout << dungeon[r][c] << " ";
            }
            cout << endl;
        }
    }
    else {
        cout << "You're trapped inside the dungeon .. :< " << endl;
    }

}
