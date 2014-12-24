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

int AvlTree::del(int data)
{
    int result = 0;
    if (root == NULL)
    {
        return -1;
    }

    int shorter = 0;
    result = delete_loop(&root, data, &shorter, NULL);
    return result;
}

int AvlTree::find(int data)
{
    if (root == NULL)
    {
        return -1;
    }
    
    AvlTreeNode *node = root;
    while (node != NULL)
    {   
        if (data == node->data)
            return 0;
            
        if (data > node->data)
            node = node->right;
        else
            node = node->left;
    }
    return -1;
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
    printf("%d %d\n", node->data, node->balance);
    print_tree_loop(node->right);
    return;
}

int AvlTree::insert_loop(AvlTreeNode **node, int data, int *taller)
{
    int result;
    if (*node == NULL)
    {
        *node = create_node(*node, data);
        if (*node != NULL)
            *taller = 1;
        return 0;
    }

    if (data > (*node)->data)
    {
        result = insert_loop(&(*node)->right, data, taller);
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
                    right_balance(node, taller, true);
                    break;
            }
        }
    }
    else if (data <= (*node)->data)
    {
        result = insert_loop(&(*node)->left, data, taller);
        if (*taller != 0)
        {
            switch ((*node)->balance)
            {
                case -1:
                    left_balance(node, taller, true);
                    break;
                case 0:
                    (*node)->balance = -1;
                    break;
                case 1:
                    (*node)->balance = 0;
                    *taller = 0;
                    break;
            }
        }
    }
    else
    {
        return -2;
    }
    return result;
}

int AvlTree::delete_loop(AvlTreeNode **node, int data, int *shorter, AvlTreeNode *delete_node)
{
    int result = 0;
    if (delete_node != NULL)
    {
        if ((*node)->right == NULL)
        {
            delete_node->data = (*node)->data;
            AvlTreeNode *leftsub = (*node)->left;
            free(*node);
            *node = leftsub;
            *shorter = 1;
            return 0;
        }
    }
    else
    {
        if (data > (*node)->data)
        {
            if ((*node)->right == NULL)
            {
                return -1;
            }
            result = delete_loop(&(*node)->right, data, shorter, delete_node);
            if (*shorter != 0)
            {
                switch ((*node)->balance)
                {
                    case -1:
                        left_balance(node, shorter, false);
                        break;
                    case 0:
                        (*node)->balance = -1;
                        *shorter = 0;
                        break;
                    case 1:
                        (*node)->balance = 0;
                        break;
                }
            }
        }
        else if (data < (*node)->data)
        {
            if ((*node)->left == NULL)
            {
                return -1;
            }
            result = delete_loop(&(*node)->left, data, shorter, delete_node);
            if (*shorter != 0)
            {
                switch ((*node)->balance)
                {
                    case -1:
                        (*node)->balance = 0;
                        break;
                    case 0:
                        (*node)->balance = 1;
                        *shorter = 0;
                        break;
                    case 1:
                        right_balance(node, shorter, false);
                        break;
                }
            }
        }
        else
        {
            AvlTreeNode *left = (*node)->left;
            AvlTreeNode *right = (*node)->right;
            if ((*node)->left == NULL)
            {
                free(*node);
                *node = right;
            }
            else if ((*node)->right == NULL)
            {
                free(*node);
                *node = left;
            }
            else
            {
                delete_loop(&(*node)->left, data, shorter, *node);
                if (*shorter != 0)
                {
                    switch ((*node)->balance)
                    {
                        case -1:
                            (*node)->balance = 0;
                            break;
                        case 0:
                            (*node)->balance = -1;
                            *shorter = 0;
                            break;
                        case 1:
                            right_balance(node, shorter, false);
                            break;
                    }
                }
                return 0;
            }
            *shorter = 1;
            return 0;
        }
    }
    return result;
}

int AvlTree::left_balance(AvlTreeNode **node, int *taller, bool is_insert)
{
    AvlTreeNode *leftsub;
    AvlTreeNode *next_rightsub;

    leftsub = (*node)->left;
    switch (leftsub->balance)
    {
        case 1:
            next_rightsub = leftsub->right;
            switch (next_rightsub->balance)
            {
                case -1:
                    (*node)->balance = 1;
                    (*node)->left->balance = 0;
                    break;
                case 0:
                    (*node)->balance = 0;
                    (*node)->left->balance = 0;
                    break;
                case 1:
                    (*node)->balance = 0;
                    (*node)->left->balance = -1;
                    break;
            }
            next_rightsub->balance = 0;
            rotate_left(&(*node)->left);
            rotate_right(node);
            if (is_insert)
            {
                *taller = 0;
            }
            break;
        case 0:
            if (!is_insert)
            {
                leftsub->balance = 1;
                rotate_right(node);
                *taller = 0;
            }
            break;
        case -1:
            leftsub->balance = 0;
            (*node)->balance = 0;
            rotate_right(node);
            if (is_insert)
            {
                *taller = 0;
            }
            break;
    }
    return 0;
}

int AvlTree::right_balance(AvlTreeNode **node, int *taller, bool is_insert)
{
    AvlTreeNode *rightsub;
    AvlTreeNode *next_leftsub;

    rightsub = (*node)->right;
    switch (rightsub->balance)
    {
        case -1:
            next_leftsub = rightsub->left;
            switch (next_leftsub->balance)
            {
                case -1:
                    (*node)->balance = 0;
                    (*node)->right->balance = 1;
                    break;
                case 0:
                    (*node)->balance = 0;
                    (*node)->right->balance = 0;
                    break;
                case 1:
                    (*node)->balance = -1;
                    (*node)->right->balance = 0;
                    break;
            }
            next_leftsub->balance = 0;
            rotate_right(&(*node)->right);
            rotate_left(node);
            if (is_insert)
            {
                *taller = 0;
            }
            break;
        case 0:
            if (!is_insert)
            {
                rightsub->balance = -1;
                rotate_left(node);
                *taller = 0;
            }
            break;
        case 1:
            rightsub->balance = 0;
            (*node)->balance = 0;
            rotate_left(node);
            if (is_insert)
            {
                *taller = 0;
            }
            break;
    }
    return 0;
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
