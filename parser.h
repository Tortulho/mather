#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"
#include "utils.h"

typedef struct {
    Token *tokens;
    int count;
    int pos;
} Parser;

ASTNode *parseFactor(Parser *parser);
ASTNode *parseTerm(Parser *parser);
ASTNode *parseExpression(Parser *parser);
ASTNode *parseAssignment(Parser *parser);
ASTNode *parse(Parser *parser);

#endif