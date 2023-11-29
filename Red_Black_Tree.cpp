#include <iostream>
#include <string>
#include <queue>
using namespace std;

class RBTree {
    struct Node {
        // Each Node will be a Key/Value Pair
        // The Tree will Search/Store based on the Key
        // The Key is the Jaro-Value of the Password
        float jval;
        // The Value is the given Password
        string pass;
        // Red = true, Black = false
        bool color;
        // Keep track of Left, Right, and Parent nodes
        Node *parent, *left, *right;
        // Null node initializer
        Node() {
            jval = -1;
            pass = "";
            // Null nodes are always black
            color = false;
            parent = left = right = nullptr;
        }
        Node(float jval, string pass) {
            this->jval = jval;
            this->pass = pass;
            // Always Insert as a Red Node
            color = true;
            parent = nullptr;
        }
    };
    Node* root;
    // Helper node to ensure invalid addresses aren't accessed in opertations
    Node* null;
    public:
    // Default Constructor
    RBTree();
    void insert(const float& jval, const string& pass);
    void treeUpkeep(Node* n);
    void leftRotate(Node* n);
    void rightRotate(Node* n);
    // Silly Function
    void levelPrint();
};

RBTree::RBTree() {
    null = new Node;
    root = null;
}

void RBTree::insert(const float& jval, const string& pass) {
    Node* n = new Node(jval, pass);
    n->left = n->right = null;
    // If root doesn't exist, assign n as new root
    if(root == null) {
        // Root must be Black
        n->color = false;
        root = n;
        return;
    }
    Node* parent = nullptr;
    Node* temp = root;
    while(temp != null) {
        parent = temp;
        if(n->jval < temp->jval) temp = temp->left;
        else temp = temp->right;
    }
    n->parent = parent;
    if(n->jval < parent->jval) parent->left = n;
    else parent->right = n;
    // Current Node needs a grandparent for RBTree Balancing
    if(n->parent->parent) treeUpkeep(n);
}

void RBTree::treeUpkeep(Node* n) {
    Node* unc;
    // Balance only if two consecutive Red nodes
    while (n->parent->color) {
        // Check whether uncle node is to left or right of grandparent
        if(n->parent == n->parent->parent->right) {
            // Uncle is left child of grandparent
            unc = n->parent->parent->left;
            // If uncle is red => flip colors
            if(unc->color) {
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                // Uncle and Parent have been fixed, so move to grandparent
                n = n->parent->parent;
            }
            // Else uncle is black => rotate
            else {
                if(n == n->parent->left) {
                    n = n->parent;
                    rightRotate(n);
                }
                n->parent->color = false;
                n->parent->parent->color = true;
                leftRotate(n->parent->parent);
            }
        }
        else {
            // Uncle is right child of grandparent
            unc = n->parent->parent->right;
            if(unc->color) {
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                n = n->parent->parent;
            }
            else {
                if(n == n->parent->right) {
                    n = n->parent;
                    leftRotate(n);
                }
                n->parent->color = false;
                n->parent->parent->color = true;
                rightRotate(n->parent->parent);
            }
        }
        // If we've reached the top of the tree (root) stop
        if(n == root) break;
    }
    root->color = false;
}

void RBTree::leftRotate(Node* n) {
    Node* child = n->right;
    n->right = child->left;
    if(child->left != null) child->left->parent = n;
    child->parent = n->parent;
    if(n->parent == nullptr) root = child;
    else if(n == n->parent->left) n->parent->left = child;
    else n->parent->right = child;
    child->left = n;
    n->parent = child;
}

void RBTree::rightRotate(Node* n) {
    Node* child = n->left;
    n->left = child->right;
    if(child->right != null) child->right->parent = n;
    child->parent = n->parent;
    if(n->parent == nullptr) root = child;
    else if(n == n->parent->right) n->parent->right = child;
    else n->parent->left = child;
    child->right = n;
    n->parent = child;
}

void RBTree::levelPrint() {
    queue<Node*> q;
    Node* temp;
    q.push(root);
    while(!q.empty()) {
        temp = q.front();
        q.pop();
        cout << temp->pass << " ";
        if(temp->left != null) q.push(temp->left);
        if(temp->right != null) q.push(temp->right);
    }
}

int main() {
    // set precision to 5 decimal points
    RBTree myTree;
    myTree.insert(5.5, "a");
    myTree.insert(4.0, "b");
    myTree.insert(3.5, "c");
    myTree.insert(3.0, "d");
    myTree.insert(2.5, "e");
    myTree.insert(2.0, "f");
    myTree.levelPrint();
    return 0;
}