#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/

// delete the key k from the btree
void BTree::remove(int k)
{
    if (!root){
        // The tree is empty
        return;
    }
    remove(root, k, true);
    if (root->n == 0){
        Node* old_root = root;
        if(root->leaf){
            root = nullptr;
        }else {
        root = root->c[0];
        }
    delete old_root;
    }
}

// delete the key k from the btree rooted at x
void BTree::remove(Node* x, int k, bool x_root)
{
    int i = find_k(x, k);
    //case 1 and 2
    if (i < x->n and x->keys[i] == k){
        if(x->leaf){ //case 1
            remove_leaf_key(x, i);
            return;
        } else { //case 2
            Node* y = x->c[i];    // Predecessor child
            Node* z = x->c[i+1];  // Successor child
            if (y->n >= t){
                //case 2a
                int pre = max_key(y);
                x->keys[i] = pre;
                remove(y, pre, false);
            } else if(z->n >= t) {
                //case 2b
                int suc = min_key(z);
                x->keys[i] = suc;
                remove(z, suc, false);
            } else {
                //case 2c
                merge_left(y, z, x->keys[i]); 
                remove_internal_key(x, i, i+1); // Clean up the parent x.
                remove(y, k, false); 
            }
        } 
    } else {
        //case 3
        if (x->leaf){
        //"Key not found"
        return;
        }
        Node* child_to_descend = x->c[i];
        if (child_to_descend->n == t - 1){
            //case 3a
            if(i < x->n && x->c[i+1]->n >=t){// A right sibling exists if i < x->n.
                swap_right(x, child_to_descend, x->c[i+1], i);
            // A left sibling exists if i > 0.
            }
            else if(i > 0 && x->c[i-1]->n >=t){
                swap_left(x, child_to_descend, x->c[i-1], i-1);
            } else { //case 3b
                // If the child has a right sibling, merge with it.
                if (i < x->n){ 
                    int parent_key = x->keys[i];
                    merge_left(child_to_descend,x->c[i+1], parent_key);
                    remove_internal_key(x, i, i+1);
                } else { // i must be == x->n
                // If the child does not have a right sibling, merge the left sibling.
                    int parent_key = x->keys[i-1];    
                    merge_right(child_to_descend, x->c[i-1], parent_key);
                    remove_internal_key(x, i-1, i-1);
                }
            }

        }
        remove(child_to_descend, k, false);
    }
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node* x, int k)
{
    int i = 0;
    while(i < x->n && k > x->keys[i]){
        i++;
    }
    return i;
}

// remove the key at index i from a btree leaf node x
void BTree::remove_leaf_key(Node* x, int i)
{
    for(int j = i; j < x->n-1; j++){
        x->keys[j] = x->keys[j+1];
    }
    x->n--;
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node* x, int i, int j)
{
    for(int key = i; key < x->n-1; key++){
        x->keys[key] = x->keys[key+1];
    }
    for(int child = j; child < x->n; child++){
        x->c[child] = x->c[child+1];
    }
    x->n--;
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node* x)
{   
    Node* current = x;
    while(!current->leaf){
         current = current->c[current->n];
    }
    return current->keys[current->n-1];
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node* x)
{
    Node* current = x;
    while(!current->leaf){
        current = current->c[0];
    }
    return current->keys[0];
}

// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node* x, Node* y, int k)
{   
    int w = x->n;
    
    x->keys[x->n] = k;
    for(int i = 0; i < y->n; i++){
        x->keys[x->n +1 + i] = y->keys[i];
    }
    if(!x->leaf){
        for(int i = 0; i <= y->n; i++){
        //x had x->n children (0..x->n). After adding k,
        //the next child slot to fill is at index x->n + 1.
            x->c[x->n +1 + i] = y->c[i];
        }
    }
    x->n = x->n + y->n +1;
    delete y;
}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node* x, Node* y, int k)
{
    int total_to_shift = y->n + 1;
    for(int i = x->n-1; i >= 0; i--){
        x->keys[i + total_to_shift] = x->keys[i];
    }
    if(!x->leaf){
        for(int i = x->n; i>=0 ; i--){
            x->c[i + total_to_shift] = x->c[i];
        }
    }
    // copy from y to x
    for(int i = 0; i <y->n; i++){
        x->keys[i] = y->keys[i];
    }
    x->keys[y->n] = k;

    if(!x->leaf){
        for(int i = 0; i <= y->n; i++){
            x->c[i] = y->c[i];
        }
    }
    x->n = x->n +y->n+1; // +1 for the key k
    delete y;
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node* x, Node* y, Node* z, int i)
{   
    for (int j = y->n - 1; j >= 0; j--)
    {
        y->keys[j + 1] = y->keys[j];
    }
    //move th i key in x to the left of first left in y so we have to shift child
    if (!y->leaf)
    {
        for (int j = y->n; j >= 0; j--)
        {
            y->c[j + 1] = y->c[j];
        }
    }
    y->keys[0] = x->keys[i];

    if (!y->leaf)
    {
        // The rightmost child of 'z' becomes the new leftmost child of 'y'
        y->c[0] = z->c[z->n];
    }
    x->keys[i] = z->keys[z->n - 1];
    y->n++;
    z->n--;


}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node* x, Node* y, Node* z, int i)
{
    y->keys[y->n] =  x->keys[i];

    x->keys[i] = z->keys[0];
    if (!y->leaf) 
    {
        // The leftmost child of 'z' becomes the new rightmost child of 'y'
        y->c[y->n+1] = z->c[0];
    }

    y->n++;

    for (int j = 0; j < z->n-1; j++)
    {
        z->keys[j] = z->keys[j+1];
    }
    
    if(!z->leaf){
        for (int j = 0; j < z->n; j++){
            z->c[j] = z->c[j+1];
        }
    }
    z->n--;
}