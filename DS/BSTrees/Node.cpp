typedef long long ll;

class Node {
    ll key_;
    ll height_;
    Node * parent_;
    Node * left_child_;
    Node * right_child_;

    public:
        Node(ll k, ll h, Node * p);
        void setKey(ll k);
        void setHeight(ll h);
        // should i pass the node pointer to the parent & Children Nodes by reference
        // -> we pass the pointers by reference if we allocate memory inside the function
        void setParent(Node * p);
        void setLeftChild(Node * l);
        void setRightChild(Node * r);

        ll getKey() const;
        ll getHeight() const;
        Node * getParent() const;
        Node * getLeftChild() const;
        Node * getRightChild() const;
};
//--------------------------------------------
Node :: Node(ll k, ll h, Node * p) : key_(k), height_(h), parent_(p) {
    left_child_ = right_child_ = nullptr;
}

void Node :: setKey(ll k) {key_ = k;}

void Node :: setHeight(ll h) {
    height_ = h;
}

void Node :: setParent(Node * p) {parent_ = p;}

void Node :: setLeftChild(Node * l) {left_child_ = l;}

void Node :: setRightChild(Node * r) {right_child_ = r;}

ll Node :: getKey() const {return key_;}

ll Node :: getHeight() const {return height_;}

Node * Node :: getParent() const {return parent_;}

Node * Node :: getLeftChild() const {return left_child_;}

Node * Node :: getRightChild() const {return right_child_;}
