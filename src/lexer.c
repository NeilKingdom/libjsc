#include "lexer.h"

Node_t ast_create() {
    Node_t root = NULL;

    root = malloc(sizeof(struct Node));
    if (root == NULL) {
        perror("Failed to allocate space for node");
        return root;
    }

    root->parent = NULL;
    root->children = NULL;
    root->num_children = 0;
    return root;
}

void ast_add_child(Node_t parent, Node_t child) {
    parent->num_children++;
    parent->children = realloc(parent->children, sizeof(struct Node) * parent->num_children);
    if (parent == NULL) {
        perror("Failed to reallocate space for child node");
        return;
    }

    child->parent = parent;
    parent->children[parent->num_children - 1] = child;
}

void ast_destroy(Node_t root) {
    for (unsigned i = 0; i < root->num_children; ++i) {
        ast_destroy((Node_t)root->children[i]);
    }

    free(root);
}

void ast_print(const Node_t root, const unsigned indent_lvl) {
    for (unsigned i = 0; i < indent_lvl; ++i) {
        putchar('\t');
    }

    switch (root->type) {
        case JSC_OBJ:
            puts("Object");
            break;
        case JSC_ARR:
            puts("Array");
            break;
        case JSC_KVP:
            puts("KV Pair");
            break;
        case JSC_KEY:
            puts("Key");
            break;
        case JSC_VAL:
            puts("Val");
            break;
    }

    for (unsigned i = 0; i < root->num_children; ++i) {
        ast_print(root->children[i], indent_lvl + 1);
    }
}
