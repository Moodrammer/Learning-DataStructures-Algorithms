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
#include <iostream>
#include <algorithm>
#include <cassert>

#include "BST.cpp"

using namespace std;

class AVLTree : public BST {
// Utility functions to keep the AVL property in the tree
    ll calculateHeight(Node * n);
    Node * rotateRight(Node * n);
    Node * rotateLeft(Node * n);
    void rebalanceRight(Node * n);
    void rebalanceLeft(Node * n);

    public:
    AVLTree(Node * r);
};


AVLTree :: AVLTree(Node * r) :BST(r) {}

ll AVLTree :: calculateHeight(Node * n) {
    if(n == NULL) return 0;
    return 1 + max(calculateHeight(n -> getRightChild()), calculateHeight(n -> getLeftChild()));
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
    Node * p = n -> getParent();
    Node * l = n -> getLeftChild();
    Node * lr = l -> getRightChild();

    n -> setLeftChild(lr);
    if(lr != NULL) lr -> setParent(n);

    l -> setRightChild(n);
    n -> setParent(l);

    if(p) {
        if(p -> getRightChild() == n) {
            p -> setRightChild(l);
        }
        else {
            p -> setLeftChild(l);
        }
        l -> setParent(p);
    }
    return l;
}

Node * AVLTree :: rotateLeft(Node * n) {
    Node * p = n -> getParent();
    Node * r = n -> getRightChild();
    Node * rl = r -> getLeftChild();

    n -> setRightChild(rl);
    if(rl) rl -> setParent(n);

    r -> setLeftChild(n);
    n -> setParent(r);

    if(p) {
        if(p -> getRightChild() == n) p -> setRightChild(r);
        else p -> setLeftChild(r);
        r -> setParent(p);
    }

    return r;
}
