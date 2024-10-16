// Stub file for compiling against the solution
// YOUR WORK SHOULD NOT BE IN THIS FILE, WORK IN prqueue.h

#pragma once

#include <string>

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
    NODE* curr;
    NODE* temp;

    struct Private;

   public:
    // Creates an empty `prqueue`
    // Runs in O(1)
    prqueue();
   
    // Copy constructor
    // Runs in O(N), where N is the number of values in `other`
    prqueue(const prqueue& other);

    // Assignment operator; `operator=`
    // Runs in O(N + O)  N = number of values in `this` and O = number of values in `other`
    prqueue& operator=(const prqueue& other);

    // Empties the `prqueue`, freeing all memory it controls.
    // Runs in O(N)
    void clear();

    // Destructor
    // Runs in O(N)
    ~prqueue();

    // Adds `value` to the `prqueue` with the given `priority`
    // Runs in O(H + M)  H = height of the tree, and M = number of duplicate priorities
    void enqueue(T value, int priority);

    // Returns value with the smallest priority in the `prqueue` 
    // Does not modify the `prqueue`
    // If `prqueue` is empty, returns the default value for `T`
    // Runs in O(H + M)     H = height of the tree M = the number of duplicate priorities
    T peek() const;

    // Returns value with the smallest priority in the `prqueue` 
    // Removes it from the `prqueue
    // If the `prqueue` is empty, returns the default value for `T`
    // Runs in O(H + M)     H = height of the tree M = number of duplicate priorities
    T dequeue();

    // Returns the number of elements in the `prqueue`
    // Runs in O(1)
    size_t size() const;

    // Resets internal state for an iterative in-order traversal
    // O(H)     H = maximum height of the tree
    void begin();

    // Uses internal state to return next in-order value and priority
    // by reference and advances the internal state
    // Returns true if reference parameters were set, and false otherwise
    // Runs in worst-case O(H + M) or O(H)      H = height of the tree M = number of duplicate priorities
    bool next(T& value, int& priority);

    // Converts the `prqueue` to a string representation in priority order
    // Runs in O(N)
    string as_string() const;

    // Checks if the contents of `this` and `other` are equivalent ie they have the same priorities,
    // values, and same internal tree structure
    // Runs in O(N) time, where N is the maximum number of nodes either `prqueue`
    bool operator==(const prqueue& other) const;

    // Returns a pointer to root node of the BST
    // Runs in O(1)
    void* getRoot() {
        return root;
    }
};
