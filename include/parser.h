#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jsc_common.h"

typedef enum {
    JSC_LCB, // Left curly brace
    JSC_RCB, // Right curly brace
    JSC_LSB, // Left square bracket
    JSC_RSB, // Right square bracket
    JSC_CLN, // Colon
    JSC_SEP, // Comma separator
    JSC_WSP, // White space
    JSC_NUM, // Number
    JSC_STR, // String
    JSC_BOL, // Boolean
    JSC_NIL, // Null
    JSC_ERR, // Undefined
} TokenType_t;

typedef struct {
    char *begin;
    char *end;
    TokenType_t type;
} Token_t;

char *read_json(const char* const path);
void  parse_json(const char* const path);

#ifdef __cplusplus
}
#endif

#endif
