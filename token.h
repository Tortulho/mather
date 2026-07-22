#ifndef TOKEN_H
#define TOKEN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "runtime.h"
#include "error.h"


typedef struct {

    int type;

    union value
    {
        long numINT;
        double numFLOAT;
        char *string;
    } value;

} Token;

typedef enum TokenType
{
    TOKEN_NULL,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_MATH_OPERATOR,
    TOKEN_LEFT_CBRACE,
    TOKEN_RIGHT_CBRACE,
    TOKEN_COMMA,

    MATH_OPERATION,
    MATH_PLUS,
    MATH_MINUS,
    MATH_MULT,
    MATH_DIV, 

}
TokenType;

void tokenize(char *text, Token *tokenarr, int tokensize, int *tokencount, RuntimeContext *ctx);

void freeTokens(Token *tokens, int count);

#endif