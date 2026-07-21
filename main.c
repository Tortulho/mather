#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"
#include "interpreter.h"
#include "dict.h"

int main(void) {

    char input[1024];
    Dictionary varDict = dictCreate();

    Token tokens[256];
    int tokenCount = 0;

    Parser parser;

    ASTNode *root;

    RuntimeValue result;

    while (1) {
        printf(">>> ");

        fgets(input, sizeof(input), stdin);

        tokenize(input, tokens, 256, &tokenCount);

        //DEBUG
        // printf("Tokens (%d):\n", tokenCount);
        // for (int i = 0; i < tokenCount; i++) {

        //     printf("%d: type=%d", i, tokens[i].type);

        //     if (tokens[i].type == TOKEN_STRING)
        //         printf(" \"%s\"", tokens[i].value.string);

        //     if (tokens[i].type == TOKEN_INT)
        //         printf(" %ld", tokens[i].value.numINT);

        //     if (tokens[i].type == TOKEN_FLOAT)
        //         printf(" %f", tokens[i].value.numFLOAT);

        //     if (tokens[i].type == TOKEN_MATH_OPERATOR)
        //         printf(" '%c'", tokens[i].value.numINT);

        //     printf("\n");
        // }

        parser.tokens = tokens;
        parser.count = tokenCount;
        parser.pos = 0;

        root = parse(&parser);

        if (root == NULL) {
            printf("Error: Syntax error.\n");
            return EXIT_FAILURE;
        }

        //DEBUG
        //printf("parser.pos = %d\n", parser.pos);
        //printf("parser.count = %d\n", parser.count);
        if (parser.pos != parser.count) {
            printf("Error: Unexpected tokens to interpret.\n");
            return EXIT_FAILURE;
        }

        result = interpret(root, &varDict);

        if (result.type == VALUE_INT)
            printf("%ld\n", result.value.integer);

        else if (result.type == VALUE_FLOAT)
            printf("%lf\n", result.value.floater);

        freeTokens(tokens, tokenCount);
        ast_free(root);
    }
    
    dictDestroy(&varDict);

    return 0;
}