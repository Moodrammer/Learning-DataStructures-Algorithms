#include <iostream>
#include "BST.cpp"

using namespace std;

int main() {
    Node * root = new Node(10, 1, nullptr);
    BST tree(root);
    tree.print(); 
    tree.insert(8);
    tree.insert(12);
    tree.insert(4);
    tree.insert(9);
    tree.insert(15);
    tree.print();
    // tree.remove(10);
    tree.remove(12); tree.print();
    tree.remove(10); tree.print();
    // tree.remove(4);
    tree.remove(8); tree.print();
    tree.remove(15); tree.print();
    tree.remove(9); tree.print();

}