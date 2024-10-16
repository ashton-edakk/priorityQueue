#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
private:
    struct NODE {
        int priority;
        T value;
        NODE* parent;
        NODE* left;
        NODE* right;
        NODE* link;
    };

    NODE* root;
    size_t sz;

    // Utility pointers for begin and next.
    NODE* curr;
    NODE* temp;

    NODE* cpy(NODE* node, NODE* parent = nullptr) {
        if (node == nullptr) { 
            return nullptr;
        }

        NODE* newNode = new NODE; 
        newNode->value = node->value;
        newNode->priority = node->priority;
        newNode->parent = parent;

        newNode->right = cpy(node->right, newNode);  
        newNode->left = cpy(node->left, newNode); 
        
        if (node->link != nullptr) { //Copy over the linked nodes for dupes
            NODE* newLinkNode = newNode;
            NODE* oldLinkNode = node->link;

            while (oldLinkNode != nullptr) { //Loop to make new nodes with same values and priorities
                NODE* newerLinkNode = new NODE;
                newerLinkNode->value = oldLinkNode->value;
                newerLinkNode->priority = oldLinkNode->priority;

                newLinkNode->link = newerLinkNode; //Add the new node to the linked list 

                newLinkNode = newLinkNode->link;
                oldLinkNode = oldLinkNode->link; //Iterate to the next node in the list
            }
            newLinkNode->link = nullptr; //set last node to nullptr 
        }
        return newNode;
    }

    void push(NODE*& node, NODE* parent, T value, int priority) {
        if (!node) {
            node = new NODE{ priority, value, parent, nullptr, nullptr, nullptr };
        }
        else if (priority > node->priority) {
            push(node->right, node, value, priority);
        }
        else if (priority < node->priority) {
            push(node->left, node, value, priority);
        }
        else {
            // Handle duplicate priority
            NODE* temp = node;
            while (temp->link) {
                temp = temp->link;
            }
            temp->link = new NODE{ priority, value, parent, nullptr, nullptr, nullptr };
        }
    }

    void build_as_string(NODE* node, ostringstream& oss) const {
        if (!node) return;

        build_as_string(node->left, oss);  // Traverse left subtree

        // Process current node
        for (NODE* curr = node; curr != nullptr; curr = curr->link) {
            oss << curr->priority << " value: " << curr->value << endl;
        }

        build_as_string(node->right, oss); // Traverse right subtree
    }

    void remove(NODE* node) {//helper function for clear()
        if (node == nullptr) {
            return;
        }
        remove(node->left);   
        remove(node->right);  

        
        NODE* link = node->link;
        while (link != nullptr) {
            NODE* temp = link;
            link = link->link;
            delete temp;
        }

        delete node;  
    }

    bool equal(NODE* og, NODE* copy) const { //==operator helper
        if (og == nullptr && copy == nullptr) {
            return true;
        }
        if (og == nullptr || copy == nullptr)
        {
            return false; 
        }

        if (og->priority != copy->priority || og->value != copy->value) {
            return false;
        }
        return equal(og->right, copy->right) && equal(og->left, copy->left);
    }

public:
    // Creates an empty `prqueue`
    // Runs in O(1)
    prqueue() {
        root = nullptr;
        sz = 0;
        curr = nullptr;
        temp = nullptr;
    }

    // Copy constructor
    // Runs in O(N), where N is the number of values in `other`
    prqueue(const prqueue& other) {

        root = cpy(other.root);
        sz = other.sz;
    }

    // Assignment operator; `operator=`
    // Runs in O(N + O)  N = number of values in `this` and O = number of values in `other`
    prqueue& operator=(const prqueue& other) {
        if (this != &other) { // Handle self-assignment
            clear(); // Clear existing content
            root = cpy(other.root); // Deep copy
            sz = other.sz;
        }
        return *this;
    }

    // Empties the `prqueue`, freeing all memory it controls.
    // Runs in O(N)
    void clear() {
        remove(root);
        root = nullptr;
        sz = 0;
    }

    // Destructor
    // Runs in O(N)
    ~prqueue() {
        clear();

    }

    // Adds `value` to the `prqueue` with the given `priority`
    // Runs in O(H + M)  H = height of the tree, and M = number of duplicate priorities
    void enqueue(T value, int priority) {

        //creates new node with value and priority
        NODE* newNode = new NODE; 
        newNode->priority = priority;
        newNode->value = value;
        newNode->left = nullptr; 
        newNode->right = nullptr;
        newNode->parent = nullptr;
        newNode->link = nullptr;

        sz++; //incs sz

        if (root == nullptr) { //tree is empty
            root = newNode;
            return;
        }

        NODE* curr = root; 
        NODE* parentNode = nullptr; 

        while (curr != nullptr) { //finds place that the new node goes
            parentNode = curr;
            if (priority > curr->priority) { 
                curr = curr->right;
            }
            else if (priority < curr->priority) {
                curr = curr->left; 
            }
            else { 
                while (curr->link != nullptr) { //if there is alreayd a node there, go down the list
                    curr = curr->link; 
                }
                curr->link = newNode;//sets it to the new node
                return;
            }
        }

        //finds the parent for the new node
        if (priority > parentNode->priority) {
            parentNode->right = newNode;
        }
        else {
            parentNode->left = newNode;
        }
        newNode->parent = parentNode; 
    }


    // Returns value with the smallest priority in the `prqueue` 
    // Does not modify the `prqueue`
    // If `prqueue` is empty, returns the default value for `T`
    // Runs in O(H + M)     H = height of the tree M = the number of duplicate priorities
    T peek() const {
        if (!root) {
            return T{}; // Return default value for T if the queue is empty.
        }
        NODE* curr = root;
        while (curr->left) {
            curr = curr->left; // Traverse to the leftmost node.
        }
        return curr->value;
    }

    // Returns value with the smallest priority in the `prqueue` 
    // Removes it from the `prqueue
    // If the `prqueue` is empty, returns the default value for `T`
    // Runs in O(H + M)     H = height of the tree M = number of duplicate priorities
    T dequeue() {
        if (root == nullptr) {
            return T{};  // queue is empty return the default value of T
        }

        NODE* rmNode = root;
        NODE* parent = nullptr;

        // finds the leftmost node which has the least priority.
        while (rmNode->left != nullptr) {
            parent = rmNode;
            rmNode = rmNode->left;
        }

        T returnValue = rmNode->value; //what we return

        if (rmNode->link == nullptr) {//no dupes
            NODE* replace = nullptr;
            if (rmNode->left != nullptr && rmNode->right != nullptr) { //two children
                replace = rmNode->right;
                NODE* newParent = rmNode;
                while (replace->left != nullptr) {
                    newParent = replace;
                    replace = replace->left;
                }
                if (newParent != rmNode) {
                    newParent->left = replace->right;
                    if (replace->right != nullptr)
                    {
                        replace->right->parent = newParent;
                    }
                    replace->right = rmNode->right;
                    rmNode->right->parent = replace;
                }
                replace->left = rmNode->left;
                rmNode->left->parent = replace;
            }
            else if (rmNode->left != nullptr || rmNode->right != nullptr) {
                // one child
                if (rmNode->left != nullptr) {
                    replace = rmNode->left;
                }
                else {
                    replace = rmNode->right;
                }
                replace->parent = parent;
            }

            if (parent != nullptr) {
                if (parent->left == rmNode) {
                    parent->left = replace;
                }
                else {
                    parent->right = replace;
                }
            }
            else {
                root = replace;
                if (root != nullptr)
                {
                    root->parent = nullptr; //root has no parents
                }
            }
        }
        else { // dupes

            NODE* linked = rmNode->link;
            linked->right = rmNode->right;
            linked->left = rmNode->left;

            if (linked->right != nullptr) {
                linked->right->parent = linked;
            }
            if (linked->left != nullptr) {
                linked->left->parent = linked;
            }

            if (parent != nullptr) {
                if (parent->left != rmNode) {
                    parent->right = linked;
                }
                else {
                    parent->left = linked;
                }
            }
            else {
                root = linked; //make root the new 
            }
            linked->parent = parent;
        }

        delete rmNode; //rm the node
        sz--; //dec size 
        return returnValue;

    }

    // Returns the number of elements in the `prqueue`
    // Runs in O(1)
    size_t size() const {
        return sz;
    }

    // Resets internal state for an iterative in-order traversal
    // O(H)     H = maximum height of the tree
    void begin() {
        curr = root; 

        while (curr != nullptr) {

            if (curr->left == nullptr) { //if no values to left
                temp = curr->right;
                break;
            }

            NODE* pred = curr->left;  //points to the parent of curr

            while (pred->right != nullptr && pred->right != curr) { //finds tehe parent node of curr
                pred = pred->right;
            }

            if (pred->right != nullptr) { 
                pred->right = nullptr;
                temp = curr->right;
                break;
            }
            else {
                pred->right = curr;
                curr = curr->left;

            }
        }
    }

    // Uses internal state to return next in-order value and priority
    // by reference and advances the internal state
    // Returns true if reference parameters were set, and false otherwise
    // Runs in worst-case O(H + M) or O(H)      H = height of the tree M = number of duplicate priorities
    bool next(T& value, int& priority) {
        if (curr == nullptr && temp == nullptr) { //no nodes to traverse
            return false;
        }

        if (curr == nullptr) { //go to the parent of curr
            curr = temp;

            while (curr != nullptr) {

                if (curr->left == nullptr) { //no left child
                    temp = curr->right;
                    break;
                }


                //below is similar to begin
                NODE* pred = curr->left;

                while (pred->right != nullptr && pred->right != curr) {
                    pred = pred->right;
                }

                if (pred->right != nullptr) {
                    pred->right = nullptr;
                    temp = curr->right;
                    break;
                    
                }
                else {
                    pred->right = curr;
                    curr = curr->left;
                }
            }

            return next(value, priority);
        }
        else { 
            value = curr->value;
            priority = curr->priority;

            if (curr->link != nullptr) {//handle the list
                curr = curr->link;
            }
            else {
                curr = nullptr; //no dopes
            }

            return true;
        }
    }

    // Converts the `prqueue` to a string representation in priority order
    // Runs in O(N)
    string as_string() const {
        ostringstream oss;
        build_as_string(root, oss);
        return oss.str();
    }

    // Checks if the contents of `this` and `other` are equivalent ie they have the same priorities,
    // values, and same internal tree structure
    // Runs in O(N) time, where N is the maximum number of nodes either `prqueue`
    bool operator==(const prqueue& other) const {
        return equal(root, other.root);
    }

    // Returns a pointer to root node of the BST
    // Runs in O(1)
    void* getRoot() {
        return root;
    }
};