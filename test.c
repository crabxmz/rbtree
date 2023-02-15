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
        // if (!i)
        //     puts("");
        // else
        //     printf("data[%d]=%#x\n", i, data[i].data);
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
    // puts("now del...");
    for (i = 1; i < TEST_NODE_NUM; i++)
    {
        rbtree_delete(&data[i], &root);
        check_rbtree(root);
    }
    // print_rbtree(root);
}

void test_alloc()
{
    srand(time(NULL));
    int cnt = 10000;
    int max_node = 2000;
    int num_node = 1;
    root = (rbnode_t *)malloc(sizeof(rbnode_t));
    root->data = rand() % 10000000 + 1;
    root->color = 'b';

    while (cnt--)
    {
        if ((rand() % 2) && num_node < max_node)
        {
            rbnode_t *new_node = (rbnode_t *)malloc(sizeof(rbnode_t));
            new_node->data = rand() % 1000000000 + 1;
            new_node->left = new_node->right = new_node->fa = 0;
            while (rbnode_find(new_node->data, root))
            {
                new_node->data = rand() % 1000000000 + 1;
            }

            rbtree_insert(new_node, &root);
            num_node++;
        }
        else if (!(rand() % 2) && num_node > 1)
        {
            rbnode_t *old_rt = root;
            rbtree_delete(root, &root);
            free(old_rt);
            num_node--;
        }
    }
    while (num_node--)
    {
        rbnode_t *old_rt = root;
        rbtree_delete(root, &root);
        free(old_rt);
    }
}

int main()
{
    test1();
    puts("init test passed");
    int cnt = 10000;
    while (cnt--)
    {
        test_rand();
    }
    puts("rand test passed");
    test_alloc();
    puts("alloc test passed");
    return 0;
}