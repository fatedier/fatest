#include "stdio.h"

struct AvlTreeNode
{
    int data;
    AvlTreeNode *left;
    AvlTreeNode *right;
    int balance;
};

class AvlTree
{
public:
    AvlTree();
    ~AvlTree();

    void print_tree();
    int insert(int data);
    void destroy(AvlTreeNode *node);

private:
    AvlTreeNode *create_node(AvlTreeNode *node, int data);
    void print_tree_loop(AvlTreeNode *node);
    int insert_loop(AvlTreeNode **node, int data, int *taller);
    
    int left_balance_when_insert(AvlTreeNode **node, int *taller);
    int right_balance_when_insert(AvlTreeNode **node, int *taller);
    
    void rotate_left(AvlTreeNode **node);
    void rotate_right(AvlTreeNode **node);
    
    AvlTreeNode *root;
};
