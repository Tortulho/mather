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
#include "builtinfuncs.h"
#include "runtime.h"

int main(void) {

    char input[1024];
    Dictionary varDict = dictCreate();
    Dictionary functions = dictCreate();

    Token tokens[256];
    int tokenCount = 0;

    Parser parser;
    ASTNode *root;
    RuntimeValue result;

    RuntimeContext ctx;
    runtimeClearError(&ctx);

    builtinRegister(&functions);

    while (1) {
        printf(">>> ");

        fgets(input, sizeof(input), stdin);

        runtimeClearError(&ctx);

        tokenize(input, tokens, 256, &tokenCount, &ctx);

        if (ctx.hasError) {

            printf("Error: %s\n", ctx.message);

            //ast_free(root);
            freeTokens(tokens, tokenCount);

            continue;
        }

        parser.tokens = tokens;
        parser.count = tokenCount;
        parser.pos = 0;

        root = parse(&parser);

        if (root == NULL) {
            runtimeError(&ctx,ERROR_SYNTAX,"Syntax error.");
            printf("Error: %s\n", ctx.message);
            ast_free(root);
            freeTokens(tokens, tokenCount);
            continue;
        }

        if (parser.pos != parser.count) {
            runtimeError(&ctx,ERROR_UNEXPECTED_TOKEN,"Unexpected tokens to interpret.");
            printf("Error: %s\n", ctx.message);
            ast_free(root);
            freeTokens(tokens, tokenCount);
            continue;
        }

        result = interpret(root, &varDict, &functions, &ctx);

        if (ctx.hasError) {

            printf("Error: %s\n", ctx.message);

            ast_free(root);
            freeTokens(tokens, tokenCount);

            continue;
        }

        if (result.type == VALUE_INT)
            printf("%ld\n", result.value.integer);

        else if (result.type == VALUE_FLOAT)
            printf("%lf\n", result.value.floater);

        freeTokens(tokens, tokenCount);
        ast_free(root);
    }
    
    //FREE
    dictDestroy(&varDict);
    dictDestroy(&functions);

    return 0;
}