#include "parser.h"
#include "lexer.h"

int main(void) {
    //parse_json("./test_data.json");

    TokenType_t tokens[8] = {
        JSC_LCB,
            JSC_LSB,
            JSC_RSB,
            JSC_LCB,
                JSC_LSB,
                JSC_RSB,
            JSC_RCB,
        JSC_RCB,
    };

    Node_t active_parent = NULL;

    for (int i = 0; i < 8; ++i) {
        Node_t node = ast_create();
        if (i == 0) {
            active_parent = node;
        }

        TokenType_t tok = tokens[i];
        switch (tok) {
            case JSC_LCB:
                node->type = JSC_OBJ;
                if (active_parent != node) {
                    ast_add_child(active_parent, node);
                    active_parent = node;
                }
                break;
            case JSC_RCB:
                if (active_parent->parent != NULL) {
                    active_parent = active_parent->parent;
                }
                break;
            case JSC_LSB:
                node->type = JSC_ARR;
                if (active_parent != node) {
                    ast_add_child(active_parent, node);
                    active_parent = node;
                }
                break;
            case JSC_RSB:
                if (active_parent->parent != NULL) {
                    active_parent = active_parent->parent;
                }
                break;
            default:
                break;
        }

    }

    ast_print(active_parent, 0);
    ast_destroy(active_parent);

    return EXIT_SUCCESS;
}
