#ifndef RBTREE_NODE_DATA_TYPE
#define RBTREE_NODE_DATA_TYPE unsigned int

typedef struct rbnode_s
{
    struct rbnode_s *left;
    struct rbnode_s *right;
    struct rbnode_s *fa;
    RBTREE_NODE_DATA_TYPE data; // basic type which can compare by '<'
    char color;
} rbnode_t;
#endif

#define GET_BRO(node) (node->fa ? (node == node->fa->left ? node->fa->right : node->fa->left) : 0)
#define IS_LEFT(node) (node->fa ? (node->fa->left == node) : 0)
#define IS_RIGHT(node) (node->fa ? (node->fa->right == node) : 0)
#define REPLACE_NODE_IN_FA(node, new_node) (node->fa ? (node->fa->left == node ? (node->fa->left = new_node) : (node->fa->right = new_node)) : 0)

void rbtree_left_rotate(rbnode_t *node, rbnode_t **groot);
void rbtree_right_rotate(rbnode_t *node, rbnode_t **groot);
void rbtree_insert(rbnode_t *node, rbnode_t **groot);
void rbtree_delete(rbnode_t *node, rbnode_t **groot);
void fix_insert(rbnode_t *node, rbnode_t **groot);
void fix_delete(rbnode_t *node, rbnode_t **groot);