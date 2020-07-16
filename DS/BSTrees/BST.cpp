#include <iostream>
#include <utility>
#include "Node.cpp"
#include <queue>

using namespace std;

class BST {
    Node * root_;
    public: 
    BST(Node * r);
    void setRoot(Node * r);
    
    Node * find(Node * r, ll key);
    bool insert(ll key);
    Node * next(Node * n);
    Node * rightAncestor(Node * n);
    Node * leftDescendant(Node * n);
    bool remove(ll key);

    void print() const;
};

BST :: BST(Node * r = nullptr) : root_(r) {};

void BST :: setRoot(Node * r) {
    root_ = r;
}

// find either returns a node pointer to the key if it is found or returns a pointer to the 
// node which should have been the parent of the Node which would contain that key so that we
// can use find later to find the parent of the key during insertion
Node * BST :: find(Node * r, ll key) {
    if(r == nullptr) return r;
    if(key == r -> getKey()) return r;
    
    else if(key < r -> getKey()) {
        Node * leftChild = r -> getLeftChild();
        if(leftChild) return find(leftChild, key);
    }
    
    else if(key > r -> getKey()) {
        Node * rightChild = r -> getRightChild();
        if(rightChild) return find(rightChild, key);
    }
    return r;
}

bool BST :: insert(ll key) {
    // finding either a pointer to the node it self if it was inserted before or 
    // the position we're going to insert it in if it is the first time inserting this node
    Node * m = find(root_, key);
    ll mkey = m -> getKey();
    if (mkey != key)
    {
        Node * n = new Node(key, 1, m);
        if(key < mkey) m -> setLeftChild(n);
        else m -> setRightChild(n);
        n -> setParent(m);
        return true;
    }
    return false;
}

Node * BST :: rightAncestor(Node * n) {
    Node * parent = n -> getParent();
    while(n -> getKey() > parent -> getKey()) parent = parent -> getParent();
    return parent;
}

Node * BST :: leftDescendant(Node * n) {
    if(n -> getLeftChild() == nullptr) return n;
    return leftDescendant(n -> getLeftChild());
}

Node * BST :: next(Node * n) {
    if(!n) return n;
    Node * nxt;
    if(n -> getRightChild()) {
        nxt = leftDescendant(n -> getRightChild());
    }
    else {
        nxt = rightAncestor(n);
    }
    return nxt;
}

bool BST :: remove(ll key) {
    Node * n = find(root_, key);
    if(n == nullptr) return false;
    ll nkey = n -> getKey();
    if(key == nkey) {
        Node * leftChild = n -> getLeftChild();
        Node * rightChild = n -> getRightChild();
        Node * parent = n -> getParent();
        // if it is a leaf
        if(!(leftChild) && !(rightChild)) {
            if(parent) {
                if(parent -> getRightChild() == n) parent -> setRightChild(nullptr);
                else parent -> setLeftChild(nullptr);
            }
            else root_ = nullptr;
            delete n;
        }
        // if it has both a left child and a right child
        else if(leftChild && rightChild) {
            Node * nextNode = next(n);
            ll temp = nextNode -> getKey();
            remove(nextNode -> getKey());
            n -> setKey(temp);
        }
        // if it has a left child ,but no right child
        else if(leftChild) {
            if(parent) {
                if(parent -> getRightChild() == n) parent -> setRightChild(leftChild);
                else parent -> setLeftChild(leftChild);
            }
            else root_ = leftChild;
            leftChild -> setParent(parent);
            delete n;
        }
        // if it has a right child , but no left child
        else if(rightChild) {
            if(parent) {
                if(parent -> getRightChild() == n) parent -> setRightChild(rightChild);
                else parent -> setLeftChild(rightChild);
            }
            else root_ = rightChild;
            rightChild -> setParent(parent);
            delete n;
        }
        // if it has both a right child and a left child
        // we get the smallest element in its right sub tree to be its successor
        return true;
    }
    return false;
}

// longest print function ever
void BST :: print() const{
    vector<pair<ll, char>> keys;
    queue<pair<Node *, char>> bfs; pair<Node * , char> f;
    if(root_ != nullptr) {bfs.push(make_pair(root_, 'R')); keys.push_back(make_pair(root_->getKey(), 'R'));}
    while(!(bfs.empty())) {
         f = bfs.front();
         if(f.first -> getLeftChild()) {
             bfs.push(make_pair(f.first -> getLeftChild(), 'l'));
             keys.push_back(make_pair(f.first->getLeftChild() -> getKey(), f.second));
         }
         else {
             keys.push_back(make_pair(-1, 'x'));
         }
         if(f.first -> getRightChild()) {
             bfs.push(make_pair(f.first -> getRightChild(), 'r'));
             keys.push_back(make_pair(f.first->getRightChild() -> getKey(), f.second));
         }
         else {
             keys.push_back(make_pair(-1, 'x'));
         }
        bfs.pop();
        //  cout << f -> getKey() << endl;
    }
    int j = 1, cnt = 0; char c; pair<ll, char> p;
    for(int i = 0; i < keys.size(); i++) {
        p = keys[i];
        c = p.second;
        if(p.second == 'x') cout << "  ";
        else cout << p.first << "  ";
        cnt++;
        if(cnt == j) {j *= 2; cout << endl; cnt = 0;}
    }
    cout << endl << "-----------------------------" << endl;
}