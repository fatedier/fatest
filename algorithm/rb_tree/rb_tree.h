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

/*
 * RbTree is a special balance binary tree
 * key and data are needed when insert and key mustn't be repeated
 */
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
     * remove a node from rb_tree by key
     * return -1 when the key isn't exist,else return 0
     */
    int remove(const T1 &key);
    
    /*
     * find data by key
     * if success, return 0 else -1
     */
    int find(const T1 &key, T2 &data);
    
    /* test
    void print_tree();
    void print_tree_loop(Rb_tree_node<T1, T2> *node);
    */

private:
    /*
     * create a new red Rb_tree_node
     */
    Rb_tree_node<T1, T2> *create_node(Rb_tree_node<T1, T2> *node, const T1 &key, const T2 &data);
    Rb_tree_node<T1, T2> *get_minimum_from_subtree(Rb_tree_node<T1, T2> *tree);
    void left_rotate(Rb_tree_node<T1, T2> *node);
    void right_rotate(Rb_tree_node<T1, T2> *node);
    void destroy_loop(Rb_tree_node<T1, T2> *node);
    /*
     * transplant src node to the position of dest node
     * original node isn't freed in this function
     */
    void rb_transplant(Rb_tree_node<T1, T2> *dest, Rb_tree_node<T1, T2> *src);
    void rb_insert_fix(Rb_tree_node<T1, T2> *insert);
    void rb_remove_fix(Rb_tree_node<T1, T2> *replace);
    
private:
    Rb_tree_node<T1, T2> *root;
    Rb_tree_node<T1, T2> *sentinel;
};


/* 
 * realized functions
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

/* test
template <typename T1, typename T2>
void
RbTree<T1, T2>::
print_tree()
{
    print_tree_loop(root);
    return;
}

template <typename T1, typename T2>
void
RbTree<T1, T2>::
print_tree_loop(Rb_tree_node<T1, T2> *node)
{
    if (node == sentinel)
        return;
    
    print_tree_loop(node->left);
    cout << node->key << endl;
    if (node->left != sentinel)
    {
        cout << " left:" << node->left->key << endl;
    }
    if (node->right != sentinel)
    {
        cout << " right:" << node->right->key << endl;
    }
    print_tree_loop(node->right);
    return;
}
*/

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

    rb_insert_fix(p_insert);
    return 0;
}

/*
 * remove a node from rb_tree by key
 * return -1 when the key isn't exist,else return 0
 */
template <typename T1, typename T2>
int 
RbTree<T1, T2>::
remove(const T1 &key)
{
    Rb_tree_node<T1, T2> *p_remove = root;
    
    /* if there are zero nodes */
    if (p_remove == sentinel)
        return -1;
        
    while (p_remove != sentinel)
    {
        if (key > p_remove->key)
            p_remove = p_remove->right;
        else if (key < p_remove->key)
            p_remove = p_remove->left;
        else
            break;
    }
    
    /* if remove-node isn't exist */
    if (p_remove == sentinel)
        return -1;
    
    /* keep the remove-node's color */
    char original_color = p_remove->color;
    
    Rb_tree_node<T1, T2> *p_real_del = sentinel;    /* 
                                                     * if remove-node has two childs, 
                                                     * real-delete-node is minimum node of 
                                                     * remove-node's right subtree
                                                     */
    Rb_tree_node<T1, T2> *p_replace = sentinel;     /* the node is the right child of removed node */
    
    /* if has one child, just replace p_remove with its child */
    if (p_remove->left == sentinel)
    {
        p_replace = p_remove->right;
        rb_transplant(p_remove, p_remove->right);
    }
    else if (p_remove->right == sentinel)
    {
        p_replace = p_remove->left;
        rb_transplant(p_remove, p_remove->left);
    }
    /* if has two childs, choose the minimum node in right subtree to replace */
    else
    {
        p_real_del = get_minimum_from_subtree(p_remove->right);
        original_color = p_real_del->color;
        p_replace = p_real_del->right;
        
        if (p_real_del->parent == p_remove)
        {
            p_replace->parent = p_real_del;
        }
        else
        {
            rb_transplant(p_real_del, p_real_del->right);
            p_real_del->right = p_remove->right;
            p_real_del->right->parent = p_real_del;
        }
           
        rb_transplant(p_remove, p_real_del);
        p_real_del->left = p_remove->left;
        p_real_del->left->parent = p_real_del;
        p_real_del->color = p_remove->color;
    }
    
    delete p_remove;
    
    if (original_color == 1)
        rb_remove_fix(p_replace);
    return 0;
}

/*
 * find data by key
 * if success, return 0 else -1
 */
template <typename T1, typename T2>
int 
RbTree<T1, T2>::
find(const T1 &key, T2 &data)
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
Rb_tree_node<T1, T2> *
RbTree<T1, T2>::
get_minimum_from_subtree(Rb_tree_node<T1, T2> *tree)
{
    Rb_tree_node<T1, T2> *p_search = tree;
    Rb_tree_node<T1, T2> *p_minimum = sentinel;
    while (p_search != sentinel)
    {
        p_minimum = p_search;
        p_search = p_search->left;
    }
    return p_minimum;
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
    
    leftsub->right = node;
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

/*
 * transplant src node to the position of dest node
 * original node isn't freed in this function
 */
 template <typename T1, typename T2>
void 
RbTree<T1, T2>::
rb_transplant(Rb_tree_node<T1, T2> *dest, Rb_tree_node<T1, T2> *src)
{
    if (dest->parent == sentinel)
        root = src;
    else if (dest == dest->parent->left)
        dest->parent->left = src;
    else
        dest->parent->right = src;
    
    src->parent = dest->parent;
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
                right_rotate(insert);
            }
            /* case 3 */
            else
            {
                insert->parent->color = 1;
                insert->parent->parent->color = 0;
                left_rotate(insert->parent->parent);
            }
        }
    }
    root->color = 1;
    return;
}

template <typename T1, typename T2>
void
RbTree<T1, T2>::
rb_remove_fix(Rb_tree_node<T1, T2> *replace)
{
    Rb_tree_node<T1, T2> *brother = sentinel;
    
    while (replace != root and replace->color == 1)
    {
        if (replace == replace->parent->left)
        {
            brother = replace->parent->right;
            /* case 1 */
            if (brother->color == 0)
            {
                brother->color = 1;
                replace->parent->color = 0;
                left_rotate(replace->parent);
                brother = replace->parent->right;
            }
            /* case 2 */
            if (brother->left->color == 1 && brother->right->color == 1)
            {
                brother->color = 0;
                replace = replace->parent;
            }
            /* case 3 */
            else if (brother->right->color == 1)
            {
                brother->left->color = 1;
                brother->color = 0;
                right_rotate(brother);
                brother = replace->parent->right;
            }
            /* case 4 */
            else
            {
                brother->color = replace->parent->color;
                replace->parent->color = 1;
                brother->right->color = 1;
                left_rotate(replace->parent);
                replace = root;
            }
        }
        /* almost same with left case, just exchange "left" and "right" */
        else
        {
            brother = replace->parent->left;
            /* case 1 */
            if (brother->color == 0)
            {
                brother->color = 1;
                replace->parent->color = 0;
                right_rotate(replace->parent);
                brother = replace->parent->left;
            }
            /* case 2 */
            if (brother->right->color == 1 && brother->left->color == 1)
            {
                brother->color = 0;
                replace = replace->parent;
            }
            /* case 3 */
            else if (brother->left->color == 1)
            {
                brother->right->color = 1;
                brother->color = 0;
                left_rotate(brother);
                brother = replace->parent->left;
            }
            /* case 4 */
            else
            {
                brother->color = replace->parent->color;
                replace->parent->color = 1;
                brother->left->color = 1;
                right_rotate(replace->parent);
                replace = root;
            }
        }
    }
    
    replace->color = 1;
    return;
}

#endif /* _RB_TREE_H_ */