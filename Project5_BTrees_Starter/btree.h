#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Node
{
    int* keys;
    Node** c;
    int t;
    bool leaf;
    int n;

    Node(int t, bool leaf = true);

    ~Node();
};

class BTree
{
private:
    Node* root;
    int t; // minimum degree
    // Build tree from file
    bool build_tree(const std::string &filename);

    void remove(Node* x, int k, bool x_root = false);
    int find_k(Node* x, int k);
    void remove_leaf_key(Node* x, int i);
    void remove_internal_key(Node* x, int i, int j);
    int max_key(Node* x);
    int min_key(Node* x);
    void merge_left(Node* x, Node* y, int k);
    void merge_right(Node* x, Node* y, int k);
    void swap_left(Node* x, Node* y, Node* z, int i);
    void swap_right(Node* x, Node* y, Node* z, int i);

public:
    BTree(const std::string &filename);
    // For debugging
    void print();

    void remove(int k);
};
