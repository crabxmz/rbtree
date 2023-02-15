#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "rbtree.h"
#define TEST_NODE_NUM 256

void check_rbtree(rbnode_t *rt)
{
    if (!rt)
        return;

    if (rt->left)
    {
        if (rt->left->color == 'r')
            assert(rt->color != 'r');
        assert(rt->data > rt->left->data);
        check_rbtree(rt->left);
    }
    if (rt->right)
    {
        if (rt->right->color == 'r')
            assert(rt->color != 'r');
        assert(rt->data < rt->right->data);
        check_rbtree(rt->right);
    }
}

void print_rbtree(rbnode_t *rt)
{
    if (!rt)
    {
        puts("emptry rbtree");
        return;
    }
    puts("======");
    static rbnode_t *queue[1024];
    int head = 0;
    int tail = 0;
    queue[tail++] = rt;
    int next_level_start = tail;

    while (head < tail)
    {
        rbnode_t *cur = queue[head++];
        printf("%u%c", cur->data, cur->color);
        if (cur->fa)
            printf("(%u%c) | ", cur->fa->data, cur->fa->left == cur ? 'L' : 'R');

        if (cur->left)
            queue[tail++] = cur->left;
        if (cur->right)
            queue[tail++] = cur->right;

        if (head == next_level_start)
        {
            putchar('\n');
            next_level_start = tail;
        }
    }
    printf("%d nodes in tot\n", tail);
}

rbnode_t *root;
rbnode_t data[TEST_NODE_NUM];

void test1()
{
    int i;
    for (i = 0; i < TEST_NODE_NUM; i++)
    {
        data[i].data = i;
        data[i].left = data[i].right = data[i].fa = 0;
    }

    root = &data[0];
    root->color = 'b';

    for (i = 1; i < TEST_NODE_NUM; i++)
    {
        rbtree_insert(&data[i], &root);
        // print_rbtree(root);
    }
    print_rbtree(root);
    puts("now del...");
    for (i = 1; i < TEST_NODE_NUM; i++)
    {
        rbtree_delete(&data[i], &root);
        // print_rbtree(root);
    }
    print_rbtree(root);
}

void test_rand()
{
    srand(time(NULL));
    int i;
    memset(data, 0, sizeof(data));
    for (i = 0; i < TEST_NODE_NUM; i++)
    {
        data[i].data = rand() % 10000000 + 1;
        data[i].left = data[i].right = data[i].fa = 0;
        if (!i)
            puts("");
        else
            printf("data[%d]=%#x\n", i, data[i].data);
    }

    root = &data[0];
    root->data = 0;
    root->color = 'b';

    for (i = 1; i < TEST_NODE_NUM; i++)
    {
        rbtree_insert(&data[i], &root);
        check_rbtree(root);
    }
    // print_rbtree(root);
    puts("now del...");
    for (i = 1; i < TEST_NODE_NUM; i++)
    {
        rbtree_delete(&data[i], &root);
        check_rbtree(root);
    }
    // print_rbtree(root);
}

int main()
{
    test1();
    while (1)
    {
        test_rand();
    }

    return 0;
}