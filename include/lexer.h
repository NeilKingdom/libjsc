#ifndef LEXER_H
#define LEXER_H

#ifdef __cpluplus
extern "C" {
#endif

#include "jsc_common.h"

typedef enum {
    JSC_OBJ, // Object
    JSC_ARR, // Array
    JSC_KVP, // Key-Value pair
    JSC_KEY, // Key (always a string in JSON)
    JSC_VAL, // Value (Object, Array, String, Number, Boolean, Null)
} NodeType_t;

typedef struct Node {
    NodeType_t type;
    struct Node *parent;
    struct Node **children;
    unsigned num_children;
} *Node_t;

Node_t ast_create();
void   ast_add_child(Node_t parent, Node_t child);
void   ast_destroy(Node_t root);
void   ast_print(const Node_t root, const unsigned indent_lvl);

#ifdef __cplusplus
}
#endif

#endif
