#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#include "parser.h"
#include <sys/stat.h>

/**
 * @brief Reads a json file and converts it to a char* for future operations.
 * @warn The string returned by this function must be freed at the end of its lifecycle.
 * @since 08-11-2025
 * @param path Relative or absolute path to the json file that will be read.
 * @returns A string representation of the json file's contents.
 */
char *read_json(const char* const path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    struct stat buf = { 0 };
    if (fstat(fileno(fp), &buf) != 0) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    size_t fsize = buf.st_size;

    char *text = malloc(fsize + 1);
    if (text == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    fread(text, fsize, 1, fp);
    text[fsize] = '\0';
    fclose(fp);

    return text;
}

static inline bool is_wspace(const char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

static void print_token(const Token_t* const tok) {
    size_t len;
    char *value = NULL;

    switch (tok->type) {
        case JSC_LCB:
            puts("Left Curly");
            break;
        case JSC_RCB:
            puts("Right Curly");
            break;
        case JSC_LSB:
            puts("Left Square");
            break;
        case JSC_RSB:
            puts("Right Square");
            break;
        case JSC_CLN:
            puts("Colon");
            break;
        case JSC_SEP:
            puts("Comma");
            break;
        case JSC_WSP:
            puts("Whitespace");
            break;
        case JSC_BOL:
            puts("Boolean");
            break;
        case JSC_NIL:
            puts("Null");
            break;
        case JSC_NUM:
            len = tok->end - tok->begin;
            value = malloc(sizeof(char) * (len + 1));
            if (value == NULL) {
                perror("Failed to allocate space for value");
                exit(EXIT_FAILURE);
            }
            strncpy(value, tok->begin, len);
            value[len] = '\0';
            printf("Number: %s\n", value);
            free(value);
            break;
        case JSC_STR:
            len = tok->end - tok->begin;
            value = malloc(sizeof(char) * (len + 1));
            if (value == NULL) {
                perror("Failed to allocate space for value");
                exit(EXIT_FAILURE);
            }
            strncpy(value, tok->begin, len);
            value[len] = '\0';
            printf("String: %s\n", value);
            free(value);
            break;
        case JSC_ERR:
            fprintf(stderr, "Undefined token\n");
            break;
    }
}

static void verify_string(Token_t *tok) {
    tok->type = JSC_STR;

    char c;
    while (true) {
        tok->end++;
        c = (char)(*tok->end);

        if (c == '"') {
            tok->end++;
            return;
        }
        // Control character
        if (c == '\\') {
            tok->end++;
            c = (char)(*tok->end);
            if (c != '"' &&
                c != '\\' &&
                c != '/' &&
                c != 'b' &&
                c != 'f' &&
                c != 'n' &&
                c != 'r' &&
                c != 't' &&
                c != 'u') {
                tok->type = JSC_ERR;
                return;
            }
            // Unicode character (\uNNNN)
            if (c == 'u') {
                for (int i = 0; i < 4; ++i) {
                    tok->end++;
                    c = (char)(*tok->end);
                    if (!isdigit(c)) {
                        tok->type = JSC_ERR;
                        return;
                    }
                }
            }
        }
    }
}

static void verify_number(Token_t *tok) {
    tok->type = JSC_NUM;
    while (!is_wspace((char)(*(tok->end++))));

    // TODO: Verification
}

static void verify_other(Token_t *tok) {
    tok->type = JSC_ERR;

    do {
        tok->end++;
    } while(isalpha((char)(*tok->end)));

    size_t len = tok->end - tok->begin;
    if (strncmp("null", tok->begin, len) == 0) {
        tok->type = JSC_NIL;
    } else if (strncmp("true", tok->begin, len) == 0 ||
        strncmp("false", tok->begin, len) == 0) {
        tok->type = JSC_BOL;
    }
}

static void verify_wspace(Token_t *tok) {
    tok->type = JSC_WSP;

    do {
        tok->end++;
    } while(is_wspace((char)(*tok->end)));
}

void parse_json(const char* const path) {
    char *text = read_json(path);

#ifdef DEBUG
    puts(text);
#endif

    Token_t tok = {
        .begin = text,
        .end = text,
        .type = JSC_ERR
    };

    char c;
    while (true) {
        // Reset state
        tok.begin = tok.end;
        c = (char)(*tok.begin);
        if (c == '\0') {
            break;
        }

        if (is_wspace(c)) {
            verify_wspace(&tok);
        }
        else if (c == '"') {
            verify_string(&tok);
        }
        else if (c == '-' || c == 'e' || c == 'E' || isdigit(c)) {
            verify_number(&tok);
        }
        else if (isalpha(c)) {
            verify_other(&tok);
        }
        else if (c == '[') {
            tok.type = JSC_LSB;
            tok.end++;
        }
        else if (c == ']') {
            tok.type = JSC_RSB;
            tok.end++;
        }
        else if (c == '{') {
            tok.type = JSC_LCB;
            tok.end++;
        }
        else if (c == '}') {
            tok.type = JSC_RCB;
            tok.end++;
        }
        else if (c == ':') {
            tok.type = JSC_CLN;
            tok.end++;
        }
        else if (c == ',') {
            tok.type = JSC_SEP;
            tok.end++;
        }

#ifdef DEBUG
        print_token(&tok);
#endif
    }

    free(text);
}
