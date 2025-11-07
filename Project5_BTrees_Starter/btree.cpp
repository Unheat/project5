#include "btree.h"
#include <queue>

Node::Node(int t, bool leaf) : t(t), leaf(leaf), n(0)
{
    keys = new int[2 * t - 1];
    c = new Node* [2 * t];
    for (int i = 0; i < 2 * t; i++)
        c[i] = nullptr;
}

Node::~Node()
{
    delete[] keys;
    delete[] c;
}

BTree::BTree(const std::string &filename) : root(nullptr), t(0)
{
    if (!build_tree(filename))
    {
        std::cerr << "Error in building BTree from file. Check formatting in " << filename << "\n";
    }
}

// Build tree from file
bool BTree::build_tree(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        std::cerr << "Error: cannot open file " << filename << "\n";
        return false;
    }

    std::string line;

    // First line: min degree
    if (!std::getline(in, line))
        return false;
    t = std::stoi(line);

    // Remaining lines: tree levels
    std::vector<std::vector<Node* >> levels;
    std::string level;

    while (std::getline(in, level))
    {
        std::vector<Node* > curr_level;
        std::stringstream ls(level);
        std::string node_str;

        while (std::getline(ls, node_str, '-'))
        {
            Node* node = new Node(t);
            std::stringstream ns(node_str);
            std::string node_key_str;
            while (std::getline(ns, node_key_str, ','))
            {
                if (!node_key_str.empty())
                {
                    node->keys[node->n] = std::stoi(node_key_str);
                    node->n++;
                }
            }
            curr_level.push_back(node);
        }
        levels.push_back(curr_level);
    }

    if (levels.empty())
    {
        root = nullptr;
    }
    else
    {
        root = levels[0][0];

        // Link children (level i nodes point to level i+1)
        for (size_t i = 0; i < levels.size() - 1; i++) // don't need to link leaf nodes to any children
        {
            int child_index = 0;
            for (size_t j = 0; j < levels[i].size(); j++) // for each node at level i
            {
                Node* x = levels[i][j];
                // point to the x.n+1 next children at level i+1
                for (int k = 0; k <= x->n; k++)
                {
                    x->c[k] = levels[i + 1][child_index];
                    x->leaf = false;
                    child_index++;
                }
            }
        }
    }
    return true;
}

// For debugging
void BTree::print()
{
    if (!root)
        return;

    std::queue<Node* > q;
    q.push(root);

    while (!q.empty())
    {
        int level_n = q.size(); // number of nodes at this level
        for (int i = 0; i < level_n; i++)
        {
            Node* node = q.front();
            q.pop();

            // Print keys in this node
            for (int j = 0; j < node->n; j++)
            {
                std::cout << node->keys[j];
                if (j < node->n - 1)
                    std::cout << ",";
            }

            if (i < level_n - 1)
                std::cout << "\t"; // separate nodes with tabs

            // Enqueue children for next level
            if (!node->leaf)
            {
                for (int j = 0; j <= node->n; j++)
                {
                    if (node->c[j])
                        q.push(node->c[j]);
                }
            }
        }
        std::cout << "\n"; // new level
    }
}
