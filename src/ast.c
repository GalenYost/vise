#include "ast.h"
#include "arena.h"

#include <string.h>

Node *node_new(Arena *a, NodeKind kind, size_t line, size_t col, const char *path) {
    Node *node = (Node *)arena_alloc(a, sizeof(Node));
    memset(node, 0, sizeof(Node));

    node->kind = kind;
    node->line = line;
    node->col = col;
    node->path = path;

    return node;
}
