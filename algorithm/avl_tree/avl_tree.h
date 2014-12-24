#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

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
    int del(int data);
    int find(int data);
    void destroy(AvlTreeNode *node);

private:
    AvlTreeNode *create_node(AvlTreeNode *node, int data);
    void print_tree_loop(AvlTreeNode *node);
    int insert_loop(AvlTreeNode **node, int data, int *taller);
    int delete_loop(AvlTreeNode **node, int data, int *shorter, AvlTreeNode *delete_node);

    int left_balance(AvlTreeNode **node, int *taller, bool is_insert);
    int right_balance(AvlTreeNode **node, int *taller, bool is_insert);

    void rotate_left(AvlTreeNode **node);
    void rotate_right(AvlTreeNode **node);
    
    AvlTreeNode *root;
};
