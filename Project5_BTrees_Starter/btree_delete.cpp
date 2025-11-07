#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/

// delete the key k from the btree
void BTree::remove(int k)
{
    
}

// delete the key k from the btree rooted at x
void BTree::remove(Node* x, int k, bool x_root)
{
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node* x, int k)
{
    return 0;
}

// remove the key at index i from a btree leaf node x
void BTree::remove_leaf_key(Node* x, int i)
{
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node* x, int i, int j)
{
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node* x)
{
    return 0;
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node* x)
{
    return 0;
}

// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node* x, Node* y, int k)
{
}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node* x, Node* y, int k)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node* x, Node* y, Node* z, int i)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node* x, Node* y, Node* z, int i)
{
}