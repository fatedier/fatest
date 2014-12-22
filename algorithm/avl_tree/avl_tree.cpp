#include "avl_tree.h"

AvlTree::AvlTree()
{
    root = NULL;
}

AvlTree::~AvlTree()
{
    destroy(root);
}

void AvlTree::print_tree()
{
    print_tree_loop(root);
    return;
}

int AvlTree::insert(int data)
{
    int result = 0;
    if (root == NULL)
    {
        root = create_node(root, data);
    }
    else
    {
        int taller = 0;
        result = insert_loop(&root, data, &taller);
    }
    return result;
}

void AvlTree::destroy(AvlTreeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->left != NULL)
    {
        destroy(node->left);
        node->left = NULL;
    }
    if (node->right != NULL)
    {
        destroy(node->right);
        node->right = NULL;
    }

    if (node->left == NULL && node->right == NULL)
    {
        delete node;
    }
    return;
}


AvlTreeNode * AvlTree::create_node(AvlTreeNode *node, int data)
{
    if (node == NULL)
    {
        node = new AvlTreeNode();
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->balance = 0;
        return node;
    }
    else
    {
        return NULL;
    }
}

void AvlTree::print_tree_loop(AvlTreeNode *node)
{
    if (node == NULL)
        return;

    print_tree_loop(node->left);
    printf("%d ", node->data);
    print_tree_loop(node->right);
    return;
}

int AvlTree::insert_loop(AvlTreeNode **node, int data, int *taller)
{
    if (*node == NULL)
    {
        *node = create_node(*node, data);
        if (*node != NULL)
            *taller = 1;
        return 0;
    }

    if (data >= (*node)->data)
    {
        insert_loop(&(*node)->right, data, taller);
        if (*taller != 0)
        {
            switch ((*node)->balance)
            {
                case -1:
                    (*node)->balance = 0;
                    *taller = 0;
                    break;
                case 0:
                    (*node)->balance = 1;
                    break;
                case 1:
                    right_balance_when_insert(node, taller);
                    break;
            }
        }
    }
    else if (data < (*node)->data)
    {
        insert_loop(&(*node)->left, data, taller);
    }
    else
    {
        return -2;
    }
    return 0;
}

int AvlTree::right_balance_when_insert(AvlTreeNode **node, int *taller)
{
    AvlTreeNode *rightsub;

    rightsub = (*node)->right;
    switch (rightsub->balance)
    {
        case -1:
            
        case 1:
            rotate_left(node);
            *taller = 0;
            rightsub->balance = 0;
            *node->balance = 0;
            break;
    }
}

void AvlTree::rotate_left(AvlTreeNode **node)
{
    AvlTreeNode *raise_node = (*node)->right;
    (*node)->right = raise_node->left;
    raise_node->left = (*node);
    (*node) = raise_node;
}

void AvlTree::rotate_right(AvlTreeNode **node)
{
    AvlTreeNode *raise_node = (*node)->left;
    (*node)->left = raise_node->right;
    raise_node->right = (*node);
    (*node) = raise_node;
}
