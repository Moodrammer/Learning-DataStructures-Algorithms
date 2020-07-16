#include <iostream>
#include <cassert>
#include <vector>
using namespace std;
// This is an educational array based implementation for disjoint sets (Union find)
// Reference : https://www.youtube.com/watch?v=KbFlZYCpONw

class UnionFind {
    int NumberOfElements;
    // current Number of groups inside the disjoint set
    int NumberOfGroups;
    // To map every component in the disjoint set to its id
    vector<int> parent;
    // Give the size of each root element
    vector<int> sz;
    private:
        // Check if two elements belong to the same set
        bool IsConnected(int FirstElementIndex, int SecondElementIndex);
        // returns a vector containing the representatives of sets
        vector<int> FindRoots();
    public:
    // Constructor
        UnionFind(int NumberOfComponents);
    // Finds the root of an element where: 
    // -> The root of an element is the element that is the representative of the current group the current 
    // element is inside
    // -> During finding the element Path Compression is applied which makes the structure much more stable
    // as it reduces the number of hops required to reach the parent element
    // With path compression the find function has an amortized complexity of alpha(n)
        int Find(int ElementIndex);
    // Unifies the sets of two elements given their indices into one set/group (Union by size)
        void Unify(int FirstElementIndex, int SecondElementIndex);
    // Prints the different sets currently inside the disjoint set
        void Print();
};
// ----------------------------------------------------------------------------------------------
int main() {
    cout << "Enter the number of elements of the disjoint set : " << endl;
    int size; cin >> size;
    UnionFind d1(size);
    cout << "Initializing the sets ..\n";
    d1.Print(); cout << endl;
    char input; int FirstIndex, SecondIndex;
    while(true) {
            cout << "Enter two indices to unify their sets .. or one or both as -1 to exit" << endl;
            cin >> FirstIndex >> SecondIndex;
            if(FirstIndex == -1 || SecondIndex == -1) break;
            d1.Unify(FirstIndex, SecondIndex);
            d1.Print(); cout <<endl;
    }
}
// ----------------------------------------------------------------------------------------------
// Public Members
UnionFind :: UnionFind(int NumberOfComponents) {
    assert(NumberOfComponents > 0);
    NumberOfElements = NumberOfGroups = NumberOfComponents;

    parent.resize(NumberOfComponents);
    sz.resize(NumberOfComponents);

    for(int i = 0; i < NumberOfComponents; i++) {
        // At the begining each element is a parent of itself forming its own group
        parent[i] = i;
        // At the begining each group has a size of one element
        sz[i] = 1;
    }
}

int UnionFind :: Find(int ElementIndex) {
    //make sure the element index is inside the bounds of the elements in the disjoint set
    assert(ElementIndex >= 0 && ElementIndex < NumberOfElements);
    // representative element of the set
    int root;
    if(parent[ElementIndex] == ElementIndex) root = ElementIndex;
    else {
        int CurrentIndex = ElementIndex;
        while(parent[CurrentIndex] != CurrentIndex) CurrentIndex = parent[CurrentIndex];
        root = CurrentIndex;
        // Path Compression
        CurrentIndex = ElementIndex;
        int TempIndex;
        while(parent[CurrentIndex] != CurrentIndex){
            TempIndex = CurrentIndex;
            parent[CurrentIndex] = root;
            CurrentIndex = parent[TempIndex];
        }
    }

    return root;
}

// Utility Function
bool UnionFind :: IsConnected(int FirstElementIndex, int SecondElementIndex) {
    return ((Find(FirstElementIndex) == Find(SecondElementIndex))? true : false);
}

void UnionFind :: Unify(int FirstElementIndex, int SecondElementIndex) {
    //make sure that both indices are inside the bounds of the disjoint set elements
    assert(FirstElementIndex >= 0 && FirstElementIndex < NumberOfElements);
    assert(SecondElementIndex >= 0 && SecondElementIndex < NumberOfElements);
    
    if(IsConnected(FirstElementIndex, SecondElementIndex)) return;

    // According to the video for convinience we add the shorter set to the longer one
    int FirstRoot = Find(FirstElementIndex), SecondRoot = Find(SecondElementIndex);
    if(sz[FirstRoot] < sz[SecondRoot]) {
        sz[SecondRoot] += sz[FirstRoot];
        parent[FirstRoot] = SecondRoot;
    }
    else {
        sz[FirstRoot] += sz[SecondRoot];
        parent[SecondRoot] = FirstRoot;
    }

    NumberOfGroups--;
    return;
}

vector<int> UnionFind :: FindRoots() {
    vector<int> roots;
    for(int i = 0; i < NumberOfElements; i++) {
        if(parent[i] == i) roots.push_back(i);
    }
    return roots;
}

void UnionFind :: Print() {
    vector<int> roots = FindRoots();
    for(int r = 0; r < roots.size(); r++) {
        cout << "{" << roots[r];
        for(int i = 0; i < NumberOfElements; i++) {
            if(i != roots[r] && Find(i) == roots[r]) cout << ", " << i;
        }
        cout << "}," << endl;
    }
}