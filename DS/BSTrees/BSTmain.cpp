#include <iostream>
#include "BST.cpp"

using namespace std;

int main() {
    BST t;
    char command;
    cout << "BST implementation" << endl;
    cout << "Commands" << endl;
    cout << "- i then the key to insert node" << endl;
    cout << "- r then the key to remove a node with a certain key" << endl;
    cout << "- p to print the tree" << endl;
    cout << "- e to exit the program" << endl;
    cout << "----------------------------------" << endl;
    bool toInfinityAndBeyond = true; ll key;
    while(toInfinityAndBeyond) {
        cin >> command;
        switch(command) {
            case('i'): 
                cin >> key; t.insert(key);
                break;
            case('r'):
                cin >> key; t.remove(key);
            case('p'): 
                t.print();
                break;
            case('e'):
                toInfinityAndBeyond = false;
                break;
        }
    }
}