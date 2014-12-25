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
     * return value: 0 means success, -1 means fail
     */
    int insert(const T1 &key, const T2 &data);
    int find_by_key(const T1 &key, T2 &data);

private:
    /*
     * create a red Rb_tree_node
     */
    Rb_tree_node<T1, T2> *create_node(Rb_tree_node<T1, T2> *node, const T1 &key, const T2 &data);
    void destroy_loop(Rb_tree_node<T1, T2> *node);

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

template <typename T1, typename T2>
int
RbTree<T1, T2>::
insert(const T1 &key, const T2 &data)
{
    int result = 0;
    if (root == sentinel)
    {
        root = create_node(root, key, data);
        return 0;
    }
    
    Rb_tree_node<T1, T2> *p_insert;
    p_insert = create_node(p_insert, key, data);
    
    Rb_tree_node<T1, T2> *p_search = root;
    Rb_tree_node<T1, T2> *p_position = sentinel;
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

#endif /* _RB_TREE_H_ */
