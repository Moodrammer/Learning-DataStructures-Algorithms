// consider an implementation for avl trees with integer keys
// - AVL trees are balanced binary search trees
// - Their main goal is to maintain the AVL property | rightSubtree height - leftSubtree height | <= 1 
// - It is proved that maintaining the AVL property for all the Nodes of the BST asserts that the height of
// the tree remains close to log N , and so all the operations of the tree will have a complexity of
// O(log n) where n is the total number of nodes in the tree
// - Quick Proof:
// --------------
// -> if we make an inductive hypothesis that if the height of any Node in the AVL tree is h, the number of
// Nodes in the subtree rooted by this Node will be atleast the Fibonnaci number number h in the Fib sequence
// Fh
// -> if the Node is a leaf, it follows the hypothesis as it has One Node (h = 1) and Fib(1) = 1;
// -> For any tree following the AVL property if the height of any Node is h, the height of one of its children
// will be h - 1 and the height of the other one should be at least h - 2 (AVL property) so the number of Nodes
// in both subtrees according to the hypothesis should be atleast Fib(h - 1), Fib(h - 2) respectively and thus
// the number of Nodes of the subtree rooted by the main node will be atleast Fib(h - 2) + Fib(h - 1) which 
// from the definition of the Fibonacci sequence = to Fib(h)
// -> In the Fibonacci sequence if n <= 6 the and we calculate Fib(n) , the last element will be at least
// 2 ^ (n / 2)
// -> So following the inductive hypothesis for an AVL tree with N nodes and height h the number of nodes
// would be atleast 2 ^ (h / 2) and atmost N
// 2 ^ (h / 2) <= N , h <= 2 * log(N)
// ----------------------------------------------------------------------------------------------------
// REFERENCE : https://www.youtube.com/watch?v=1QSYxIKXXP4

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <queue>
typedef long long ll;

using namespace std;

class Node {
    public:
    ll key;
    ll height;
    ll bf;
    Node * leftChild;
    Node * rightChild;
    Node * parent;

    Node(ll k) {
        key = k;
        // considering the definition of the height to be the number of edges between the current Node and
        // the leaf having the most number of edges from the current Node
        height = 0; 
        bf = 0;
        parent = nullptr;
        leftChild = nullptr; rightChild = nullptr;
    }
};

class AVLTree {
    Node * root;
// Utility functions to keep the AVL property in the tree
    void update(Node * n);
    Node * rotateRight(Node * n);
    Node * rotateLeft(Node * n);
    
    Node * balance(Node * n);
    Node * rebalanceRight(Node * n);
    Node * rebalanceLeft(Node * n);
    Node * insert(Node * root, ll key, Node * parent);
    Node * remove(Node * root, ll key);
    Node * leftDescendant(Node * n);

    public:
    AVLTree(Node * r = nullptr);
    bool find(Node * root, ll key);
    bool insertNode(ll key);
    bool removeNode(ll key);

    void print() const;
    void printInfo() const;
};


AVLTree :: AVLTree(Node * r) : root(r) {};

void AVLTree :: update(Node * n) {
    // initialize both to negative one to handle the case of a leaf node so that the heigt of the leaf
    // is calculated as one
    ll lh = -1, rh = -1;
    if(n -> rightChild) rh = n -> rightChild -> height;
    if(n -> leftChild) lh = n -> leftChild -> height;
    n -> height = 1 + max(rh, lh);
    // consider the balance factor to always be the height of the right subtree - the height of the left
    // subtree and thus its allowed values are {-1, 0, +1} where :
    // - bf < -1 means the tree is left heavy
    // - bf = 0 means the tree is roughly balanced
    // - bf > +1 means the tree is right heavy
    n -> bf = rh - lh;
    return;
}

/*
Rotation back & forth on a problematic node
  p ->  P                  P  <- p
        |                  |
 n ->   A                  x   <- n
      /   \              /   \
l->  x     y    <->     c     A  <-r
   /   \                    /   \
  c     d  <-lr      rl->  d     y

*/

Node * AVLTree :: rotateRight(Node * n) {
    Node * p = n -> parent;
    Node * l = n -> leftChild;
    Node * lr = l -> rightChild;

    n -> leftChild = lr;
    if(lr != NULL) lr -> parent = n;

    l -> rightChild = n;
    n -> parent = l;

    l -> parent = p;
    // if(p) {
    //     if(p -> rightChild == n) {
    //         p -> rightChild = l;
    //     }
    //     else {
    //         p -> leftChild = l;
    //     }
    //     l -> parent = p;
    // }
    // Update the heights of the two nodes that were affected during rotation
    update(n);
    update(l);

    // update(p);
    return l;
}

Node * AVLTree :: rotateLeft(Node * n) {
    Node * p = n -> parent;
    Node * r = n -> rightChild;
    Node * rl = r -> leftChild;

    n -> rightChild = rl;
    if(rl) rl -> parent = n;

    r -> leftChild = n;
    n -> parent = r;

    // We can set the parent in the call back of the recursive call
    r -> parent = p;
    // if(p) {
    //     // if(p -> rightChild == n) p -> rightChild = r;
    //     // else p -> leftChild = r;
    //     r -> parent = p;
    // }
    // Update the heights of the two nodes that were affected during rotation
    update(n);
    update(r);
    // I'm not sure if I should update the height of the parent node also
    // The parent node will be updated on the call backs of insert and remove
    // update(p);
    return r;
}

/*
    There are four cases in rebalancing an AVL Tree:
    let : 
    - p -> problematic node
    - rp -> the right child of the problematic node
    - lp -> the left child of the problematic node
    - rlp -> the left child of the right child of the problematic node
    - rrp -> the right child of the right child of the problematic node
    - lrp -> the right child of the left child of the problematic node
    - llp -> the left child of the left child of the problematic node
    cases:
    [1] The left left case:
    where lp.height > rp.height by more than 1 && llp.height > lrp.height
    Fix: rotate right once on p
    [2] the left right case:
    where lp.height > rp.height by more than 1 && llp.height < lrp.height
    Fix : rotate left first on lp then rotate right on p
    [3] the right left case:
    where lp.height < rp.height by more than 1 && rlp.height > rrp.height
    Fix: rotate right first on rp then rotate left on p
    [4] the right right case:
    where lp.height > rp.height by more than 1 && rlp.height < rrp.height
    Fix: rotate left once on p
*/
Node * AVLTree :: rebalanceRight(Node * n) {
    ll bf = n -> leftChild -> bf;
    // left left case
    if(bf <= 0) return rotateRight(n);
    // left right case
    else {
        n->leftChild = rotateLeft(n -> leftChild);
        return rotateRight(n);
    }
}

Node * AVLTree :: rebalanceLeft(Node * n) {
    ll bf = n -> rightChild -> bf;
    // right right case
    if(bf >= 0) return rotateLeft(n);
    // right left case
    else {
        n->rightChild =  rotateRight(n -> rightChild);
        return rotateLeft(n);
    }
}

Node * AVLTree :: balance(Node * n) {
    ll bf = n -> bf;
    if(bf == -2) return rebalanceRight(n);
    else if(bf == 2) return rebalanceLeft(n);
    else return n;
}

// Notice that on inserting a Node , the only heights that are affected are the heights on the path
// from the inserted node up to the root
Node * AVLTree :: insert(Node * root, ll key, Node * parent) {
    if(root == nullptr) {
        Node * n = new Node(key);
        n->parent = parent;
        return n;
    }

    if(key < root->key) {
        root->leftChild = insert(root->leftChild, key, root);
    }
    else {
        root->rightChild = insert(root->rightChild, key, root);
    }

    // Update the nodes heights and balance factor on the call back
    update(root);
    // rebalance the current node on the call back then return the new root of the subtree
    // after rotation to be set as the child in the previous call
    return balance(root);
}

bool AVLTree :: find(Node * root, ll key) {
    if(root == nullptr) return false;
    
    if(key == root->key) return true;
    else if (key < root->key) return find(root->leftChild, key);
    else return find(root->rightChild, key);
}

// return false if it is a duplicate value
bool AVLTree :: insertNode(ll key) {
    // Assuming that duplicate values aren't allowed in the tree
    if(!(find(root, key))) {
        // we must reset the root since it may be rotated during rebalancing the tree
        // Notice that insert is a recursive function that eventually returns a pointer to the 
        // node that became the root after finishing all the rotations and rebalacing
        root = insert(root, key, nullptr);
        return true;
    }
    return false;
}

Node * AVLTree :: leftDescendant(Node * n) {
    if(n->leftChild == nullptr) return n;
    return leftDescendant(n->leftChild);
}

Node * AVLTree :: remove(Node * root, ll key) {
    if(key == root->key) {
        if(root->rightChild){
            Node * next = leftDescendant(root->rightChild);
            root->key = next->key;
            root->rightChild = remove(root->rightChild, next->key);
            return root;
        }
        else {
            // root->parent->leftChild = root->leftChild;
            if(root->leftChild != nullptr) root->leftChild->parent = root->parent;
            Node * l = root->leftChild;
            delete root;
            return l;
        }
    }

    else if(key < root->key) root->leftChild = remove(root->leftChild, key);
    else root->rightChild = remove(root->rightChild, key);

    update(root);
    return balance(root);
}

bool AVLTree :: removeNode(ll key) {
    if(find(root, key)) {
        root = remove(root, key);
        return true;
    }
    return false;
}

// longest print function ever
void AVLTree :: print() const{
    vector<pair<ll, char>> keys;
    queue<pair<Node *, char>> bfs; pair<Node * , char> f;
    if(root != nullptr) {bfs.push(make_pair(root, 'R')); keys.push_back(make_pair(root->key, 'R'));}
    while(!(bfs.empty())) {
         f = bfs.front();
         if(f.first->leftChild) {
             bfs.push(make_pair(f.first->leftChild, 'l'));
             keys.push_back(make_pair(f.first->leftChild->key, f.second));
         }
         else {
             keys.push_back(make_pair(-1, 'x'));
         }
         if(f.first->rightChild) {
             bfs.push(make_pair(f.first->rightChild, 'r'));
             keys.push_back(make_pair(f.first->rightChild->key, f.second));
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
        if(p.second == 'x') cout << "| ";
        else cout << "|"<< p.first << " ";
        cnt++;
        if(cnt == j) {j *= 2; cout << endl; cnt = 0;}
    }
    cout << endl << "-----------------------------" << endl;
}

void AVLTree :: printInfo() const {
    queue<Node *> bfs; Node * curr;
    if(root != nullptr) bfs.push(root);
    while(!(bfs.empty())) {
        curr = bfs.front();
        bfs.pop();
        if(curr->leftChild) bfs.push(curr->leftChild);
        if(curr->rightChild) bfs.push(curr->rightChild);
        
        //printing 
        cout << "k: " << curr->key << " ";
        cout << "l: " << ((curr->leftChild)? to_string(curr->leftChild->key) : "x") << " ";
        cout << "r: " << ((curr->rightChild)? to_string(curr->rightChild->key) : "x") << " ";
        cout << "p: " << ((curr->parent)? to_string(curr->parent->key) : "x") << " ";
        cout << endl;
    }
}
