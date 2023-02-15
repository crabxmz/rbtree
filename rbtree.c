#include "stdio.h"
#include "rbtree.h"

void rbtree_left_rotate(rbnode_t *node, rbnode_t **groot)
{

    rbnode_t *x = node->right;
    if (!x)
        return;
    rbnode_t *y = node->right->left;

    y ? (y->fa = node) : 0;
    if (node->fa)
        node->fa->left == node ? (node->fa->left = x) : (node->fa->right = x);

    x->fa = node->fa;
    x->left = node;

    node->right = y;
    node->fa = x;

    if (node == *groot)
    {
        *groot = x;
    }
}

void rbtree_right_rotate(rbnode_t *node, rbnode_t **groot)
{
    rbnode_t *x = node->left;
    if (!x)
        return;
    rbnode_t *y = node->left->right;

    y ? (y->fa = node) : 0;

    if (node->fa)
        node->fa->left == node ? (node->fa->left = x) : (node->fa->right = x);

    x->fa = node->fa;
    x->right = node;

    node->fa = x;
    node->left = y;

    if (node == *groot)
    {
        *groot = x;
    }
}

void rbtree_insert(rbnode_t *new_node, rbnode_t **groot)
{
    rbnode_t *rt = *groot;
    while (rt)
    {
        if (new_node->data < rt->data)
        {
            if (rt->left)
            {
                rt = rt->left;
            }
            else
            {
                rt->left = new_node;
                new_node->fa = rt;
                new_node->color = 'r';
                break;
            }
        }
        else if (new_node->data > rt->data)
        {
            if (rt->right)
            {
                rt = rt->right;
            }
            else
            {
                rt->right = new_node;
                new_node->fa = rt;
                new_node->color = 'r';
                break;
            }
        }
        else
        {
            puts("panic:dup data");
            return;
        }
    }

    fix_insert(new_node, groot);
}

void rbtree_delete(rbnode_t *node, rbnode_t **groot)
{
    rbnode_t *node_to_fix = 0;
    char deleted_color = node->color; // root of subtree which lack a black node
    // 0 child
    if (!node->left && !node->right)
    {
        if (node->fa)
        {
            node->fa->left == node ? (node->fa->left = 0) : (node->fa->right = 0);
        }
        else
        {
            *groot = 0;
        }
        node_to_fix = node->fa;
        goto lb1;
    }
    // 1 child
    if (!node->left || !node->right)
    {
        rbnode_t *chd = node->left ? node->left : node->right;
        chd->fa = node->fa;
        if (node->fa)
        {
            node->fa->left == node ? (node->fa->left = chd) : (node->fa->right = chd);
        }
        else
        {
            *groot = chd;
        }
        node_to_fix = chd;
        goto lb1;
    }
    // 2 child
    rbnode_t *p = node->right;

    while (p->left)
        p = p->left;

    deleted_color = p->color;

    // p have at most 1 right child
    if (p->fa != node) // p and node not adjecent
    {
        node_to_fix = p->fa;
        if (p->right)
        {
            p->right->fa = p->fa;
            node_to_fix = p->right;
        }
        REPLACE_NODE_IN_FA(p, p->right);

        p->left = node->left;
        p->right = node->right;
        p->fa = node->fa;
        p->color = node->color;

        REPLACE_NODE_IN_FA(node, p);
        if (node->right)
            node->right->fa = p;
        if (node->left)
            node->left->fa = p;

        if (!node->fa)
            *groot = p;
    }
    else // p is node's right child
    {
        node_to_fix = p;
        p->left = node->left;
        p->fa = node->fa;
        p->color = node->color;

        REPLACE_NODE_IN_FA(node, p);
        if (node->left)
            node->left->fa = p;

        if (!node->fa)
            *groot = p;
    }

lb1:
    node->left = node->right = node->fa = 0;
    if (deleted_color == 'b')
    {
        fix_delete(node_to_fix, groot);
    }
}

void fix_insert(rbnode_t *node, rbnode_t **groot) // suppose node is red
{
lb3:
    if (node->fa->color != 'r')
        return;

    if (node->fa && node->fa->fa)
    {
        if (GET_BRO(node->fa) && GET_BRO(node->fa)->color == 'r') // node's fa has red bro
        {
            node->fa->fa->color = 'r';
            node->fa->color = 'b';
            GET_BRO(node->fa) ? GET_BRO(node->fa)->color = 'b' : 0;
            if (node->fa->fa->fa)
            {
                node = node->fa->fa;
                goto lb3;
                // fix_insert(node->fa->fa,groot);
            }
            else
            {
                node->fa->fa->color = 'b';
            }
        }
        else
        { // node's fa has black bro or don't have bro
            if (IS_LEFT(node->fa) && IS_RIGHT(node))
            {
                rbtree_left_rotate(node->fa, groot);
                node = node->left;
            }
            else if (IS_RIGHT(node->fa) && IS_LEFT(node))
            {
                rbtree_right_rotate(node->fa, groot);
                node = node->right;
            }

            node->fa->color = 'b';
            node->fa->fa->color = 'r';
            if (IS_LEFT(node))
            {
                rbtree_right_rotate(node->fa->fa, groot);
            }
            else if (IS_RIGHT(node))
            {
                rbtree_left_rotate(node->fa->fa, groot);
            }
        }
    }
}

void fix_delete(rbnode_t *node, rbnode_t **groot)
{
lb2:
    if (!node)
        return;
    if (node->color == 'r')
    {
        node->color = 'b';
        return;
    }

    if (GET_BRO(node))
    {
        // if node has a red bro, make it black, so all the bro is black
        if (GET_BRO(node)->color == 'r')
        {
            GET_BRO(node)->color = 'b';
            node->fa->color = 'r';
            if (node == node->fa->left)
                rbtree_left_rotate(node->fa, groot);
            else
                rbtree_right_rotate(node->fa, groot);
            goto lb2;
        }

        // if bro's child is all black
        if ((!GET_BRO(node)->left || GET_BRO(node)->left->color == 'b') &&
            (!GET_BRO(node)->right || GET_BRO(node)->right->color == 'b'))
        {
            GET_BRO(node)->color = 'r';
            if (node->fa->color == 'r')
            {
                node->fa->color = 'b';
            }
            else
            {
                node = node->fa;
                goto lb2;
                // fix_delete(node->fa, groot);
            }
            return;
        }

        // here GET_BRO(node) have at least 1 red child

        if (GET_BRO(node)->left && GET_BRO(node)->left->color == 'r') // make bro's left-child black
        {
            GET_BRO(node)->left->color = 'b';
            GET_BRO(node)->color = 'r';
            if (node == node->fa->left)
                rbtree_right_rotate(GET_BRO(node), groot);
            else
                rbtree_left_rotate(GET_BRO(node), groot);
            goto lb2;
        }

        GET_BRO(node)->color = node->fa->color;
        node->fa->color = 'b';
        GET_BRO(node)->right->color = 'b';
        if (node == node->fa->left)
            rbtree_left_rotate(node->fa, groot);
        else
            rbtree_right_rotate(node->fa, groot);
    }
    else if (node->fa)
    {
        if (node->fa->color == 'r')
            node->fa->color = 'b';
        else
            fix_delete(node->fa, groot);
        return;
    }
    else // node is root
    {
        node->color = 'b';
    }
}