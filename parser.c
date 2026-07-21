#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"

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

ASTNode *parse(Parser *parser) {
    return parseAssignment(parser);
}