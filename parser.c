#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"
#include "dynarr.h"

ASTNode *parseFactor(Parser *parser) {

    if (parser->pos >= parser->count) return NULL;
    Token token = parser->tokens[parser->pos];
    if (token.type == TOKEN_INT) {
        parser->pos++;
        return ast_newInt(token.value.numINT);
    }
    if (token.type == TOKEN_FLOAT) {
        parser->pos++;
        return ast_newFloat(token.value.numFLOAT);
    }
    if (token.type == TOKEN_STRING) {

        //more tokens?
        if (parser->pos + 1 < parser->count) {

            Token next = parser->tokens[parser->pos + 1];

            //if ( :> is a func
            if (next.type == TOKEN_LEFT_CBRACE) {
                return parseFunction(parser);
            }
        }

        /* else is a var */
        parser->pos++;
        return ast_newVariable(token.value.string);
}
    if (token.type == TOKEN_MATH_OPERATOR) {

        if (token.value.numINT == '+' ||
        token.value.numINT == '-') {
            parser->pos++;
            ASTNode *child = parseFactor(parser);

            if (child == NULL) return NULL;

            return ast_newSignalizer(token, child);
        }
        return NULL;
    }
        if (token.type == TOKEN_LEFT_CBRACE) {

        parser->pos++;

        ASTNode *node = parseExpression(parser);

        if (parser->tokens[parser->pos].type != TOKEN_RIGHT_CBRACE) return NULL;
        parser->pos++;
        return node;
    }
    return NULL;

}

// + -
ASTNode *parseTerm(Parser *parser) {
    ASTNode *left = parseFactor(parser);
    if (left == NULL)
        return NULL;

    while (parser->pos < parser->count) {

        Token token = parser->tokens[parser->pos];

        if (token.type != TOKEN_MATH_OPERATOR)
            break;

        if (token.value.numINT != '*' &&
            token.value.numINT != '/' &&
            token.value.numINT != '%' &&
            token.value.numINT != '^')
            break;

        parser->pos++;

        ASTNode *right = parseFactor(parser);
        if (right == NULL)
            return NULL;

        left = ast_newOperator(token, left, right);
    }

    return left;
}

// + -
ASTNode *parseExpression(Parser *parser) {

    ASTNode *left = parseTerm(parser);
    if (left == NULL)
        return NULL;

    while (parser->pos < parser->count) {

        Token token = parser->tokens[parser->pos];

        if (token.type != TOKEN_MATH_OPERATOR)
            break;

        if (token.value.numINT != '+' &&
            token.value.numINT != '-')
            break;

        parser->pos++;

        ASTNode *right = parseTerm(parser);
        if (right == NULL)
            return NULL;

        left = ast_newOperator(token, left, right);
    }

    return left;
}

ASTNode *parseAssignment(Parser *parser) {

    ASTNode *left = parseExpression(parser);
    if (left == NULL) return NULL;

    //if NO more tokens to read
    if (parser->pos >= parser->count) return left;

    //token is assignment? if not, return left
    Token token = parser->tokens[parser->pos];
    if (token.type != TOKEN_MATH_OPERATOR) return left;
    if (token.value.numINT != '=') return left;

    if (left->type != AST_VARIABLE) return NULL;

    parser->pos++; //EAT = (yummy)
    ASTNode *right = parseAssignment(parser);

    if (right == NULL) return NULL;

    return ast_newAssign(left, right);
}

ASTNode *parseFunction(Parser *parser)
{
    if (parser == NULL) return NULL;

    Token token = parser->tokens[parser->pos];

    if (token.type != TOKEN_STRING) return NULL;

    char *name = token.value.string;
    parser->pos++;

    if (parser->pos >= parser->count) return NULL;

    if (parser->tokens[parser->pos].type != TOKEN_LEFT_CBRACE) return NULL;

    parser->pos++;

    dynarr args = dynarrCreate();

    if (args.data == NULL) return NULL;

    /* func without args */
    if (parser->tokens[parser->pos].type != TOKEN_RIGHT_CBRACE) {

        while (1) {

            ASTNode *expr = parseExpression(parser);

            if (expr == NULL) {
                dynarrDestroy(&args);
                return NULL;
            }

            if (!dynarrPush(&args, expr)) {
                ast_free(expr);
                dynarrDestroy(&args);
                return NULL;
            }

            if (parser->tokens[parser->pos].type == TOKEN_COMMA) {
                parser->pos++;
                continue;
            }

            break;
        }
    }

    if (parser->tokens[parser->pos].type != TOKEN_RIGHT_CBRACE) {

        for (size_t i = 0; i < args.size; i++) ast_free(args.data[i]);

        dynarrDestroy(&args);
        return NULL;
    }

    parser->pos++;

    return ast_newFunction(
        name,
        (ASTNode **)args.data,
        args.size
    );
}

ASTNode *parse(Parser *parser) {
    return parseAssignment(parser);
}
