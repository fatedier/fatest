#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <iostream>
using namespace std;

template <typename T1, typename T2>
struct Rb_tree_node
{
    T1 key;
    T2 data;
    Rb_tree_node *left;
    Rb_tree_node *right;
    Rb_tree_node *parent;
    char color;             /* 0:red, 1:black */

    Rb_tree_node():left(NULL),
                   right(NULL),
                   parent(NULL),
                   color(0) {}
};

template <typename T1, typename T2>
class RbTree
{
public:
    RbTree();
    ~RbTree();

    /*
     * insert a key-data node
     * return -1 if the key is exist,else return 0
     */
    int insert(const T1 &key, const T2 &data);
    
    /*
     * find data by key
     * if success, return 0 else -1
     */
    int find_by_key(const T1 &key, T2 &data);

private:
    /*
     * create a red Rb_tree_node
     */
    Rb_tree_node<T1, T2> *create_node(Rb_tree_node<T1, T2> *node, const T1 &key, const T2 &data);
    void left_rotate(Rb_tree_node<T1, T2> *node);
    void right_rotate(Rb_tree_node<T1, T2> *node);
    void destroy_loop(Rb_tree_node<T1, T2> *node);
    void rb_insert_fix(Rb_tree_node<T1, T2> *insert);

private:
    Rb_tree_node<T1, T2> *root;
    Rb_tree_node<T1, T2> *sentinel;
};


/* 
 * function realize
 */
template <typename T1, typename T2>
RbTree<T1, T2>::
RbTree()
{
    sentinel = new Rb_tree_node<T1, T2>();
    sentinel->color = 1;
    root = sentinel;
}

template <typename T1, typename T2>
RbTree<T1, T2>::
~RbTree()
{
    destroy_loop(root);
    root = NULL;
    delete sentinel;
}

/*
 * insert a key-data node
 * return -1 if the key is exist,else return 0
 */
template <typename T1, typename T2>
int 
RbTree<T1, T2>::
insert(const T1 &key, const T2 &data)
{
    Rb_tree_node<T1, T2> *p_search = root;              /* temporary point to search the place to insert */
    Rb_tree_node<T1, T2> *p_parent = sentinel;          /* the point which is the inserted-node's parent */
    
    /* search the position to insert a new node */
    while (p_search != sentinel)
    {
        p_parent = p_search;
        if (key < p_search->key)
            p_search = p_search->left;
        else if (key > p_search->key)
            p_search = p_search->right;
        /* if the insert-node's key is exist */
        else
            return -1;
    }
    
    Rb_tree_node<T1, T2> *p_insert;
    p_insert = create_node(p_insert, key, data);        /* the node to be inserted */
    p_insert->parent = p_parent;
    
    /* if there are no nodes,just insert to root */
    if (p_parent == sentinel)
        root = p_insert;
    /* else put the node to p_parent's left or right */
    else if (p_insert->key < p_parent->key)
        p_parent->left = p_insert;
    else
        p_parent->right = p_insert;
        
    
    return 0;
}

/*
 * find data by key
 * if success, return 0 else -1
 */
template <typename T1, typename T2>
int 
RbTree<T1, T2>::
find_by_key(const T1 &key, T2 &data)
{
    if (root == sentinel)
    {
        return -1;
    }
    
    Rb_tree_node<T1, T2> *node = root;
    while (node != sentinel)
    {
        if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
        else
        {
            data = node->data;
            return 0;
        }
    }
    return -1;
}

/*
 * create a red node which left and right child point to sentinel
 * return the address of the node
 */
template <typename T1, typename T2>
Rb_tree_node<T1, T2> * 
RbTree<T1, T2>::
create_node(Rb_tree_node<T1, T2> *node, const T1 &key, const T2 &data)
{
    node = new Rb_tree_node<T1, T2>();
    node->key = key;
    node->data = data;
    node->left = sentinel;
    node->right = sentinel;
    node->parent = sentinel;
    return node;
}

template <typename T1, typename T2>
void 
RbTree<T1, T2>::
left_rotate(Rb_tree_node<T1, T2> *node)
{
    Rb_tree_node<T1, T2> *rightsub = node->right;
    node->right = rightsub->left;
    
    if (rightsub->left != sentinel)
        rightsub->left->parent = node;
    rightsub->parent = node->parent;
    
    if (node->parent == sentinel)
        root = rightsub;
    else if (node == node->parent->left)
        node->parent->left = rightsub;
    else
        node->parent->right = rightsub;
    
    rightsub->left = node;
    node->parent = rightsub;
    return;
}

template <typename T1, typename T2>
void 
RbTree<T1, T2>::
right_rotate(Rb_tree_node<T1, T2> *node)
{
    Rb_tree_node<T1, T2> *leftsub = node->left;
    node->left = leftsub->right;
    
    if (leftsub->right != sentinel)
        leftsub->right->parent = node;
    leftsub->parent = node->parent;
    
    if (node->parent == sentinel)
        root = leftsub;
    else if (node == node->parent->left)
        node->parent->left = leftsub;
    else
        node->parent->right = leftsub;
    
    leftsub->left = node;
    node->parent = leftsub;
    return;
}

template <typename T1, typename T2>
void
RbTree<T1, T2>::
destroy_loop(Rb_tree_node<T1, T2> *node)
{
    if (node == sentinel)
    {
        return;
    }
    destroy_loop(node->left);
    destroy_loop(node->right);
    delete node;
    return;
}

template <typename T1, typename T2>
void
RbTree<T1, T2>::
rb_insert_fix(Rb_tree_node<T1, T2> *insert)
{
    Rb_tree_node<T1, T2> *uncle = NULL;
    while (insert->parent->color == 0)
    {
        /* if the insert-node->parent's position is grandparent's left */
        if (insert->parent == insert->parent->parent->left)
        {
            uncle = insert->parent->parent->right;
            /* case 1 */
            if (uncle->color == 0)
            {
                insert->parent->color = 1;
                uncle->color = 1;
                insert->parent->parent->color = 0;
                insert = insert->parent->parent;
            }
            /* case 2 */
            else if (insert == insert->parent->right)
            {
                insert = insert->parent;
                left_rotate(insert);
            }
            /* case 3 */
            else
            {
                insert->parent->color = 1;
                insert->parent->parent->color = 0;
                right_rotate(insert->parent->parent);
            }
        }
        /* same as left case, with "right" and "left" exchanged */
        else
        {
            uncle = insert->parent->parent->left;
            /* case 1 */
            if (uncle->color == 0)
            {
                insert->parent->color = 1;
                uncle->color = 1;
                insert->parent->parent->color = 0;
                insert = insert->parent->parent;
            }
            /* case 2 */
            else if (insert == insert->parent->left)
            {
                insert = insert->parent;
                left_rotate(insert);
            }
            /* case 3 */
            else
            {
                insert->parent->color = 1;
                insert->parent->parent->color = 0;
                right_rotate(insert->parent->parent);
            }
        }
    }
    root->color = 1;
}

#endif /* _RB_TREE_H_ */
