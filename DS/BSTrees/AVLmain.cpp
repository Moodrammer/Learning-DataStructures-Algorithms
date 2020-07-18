#include "AVLTree.cpp"
using namespace std;

int main() {
    AVLTree t;
    char command;
    cout << "AVL Tree implementation" << endl;
    cout << "Commands" << endl;
    cout << "- i then the key to insert node" << endl;
    cout << "- r then the key to remove a key with a certain value " << endl;
    cout << "- p to print the tree" << endl;
    cout << "- f to print the info of each Node(leftChild, rightChild, parent, key) " << endl;
    cout << "- e to exit the program" << endl;
    cout << "----------------------------------" << endl;
    bool toInfinityAndBeyond = true; ll key;
    while(toInfinityAndBeyond) {
        cin >> command;
        switch(command) {
            case('i'): 
                cin >> key; t.insertNode(key);
                break;
            case('r'):
                cin >> key; t.removeNode(key);
                break;
            case('p'): 
                t.print();
                break;
            case('f'):
                t.printInfo();
                break;
            case('e'):
                toInfinityAndBeyond = false;
                break;
        }
    }
}