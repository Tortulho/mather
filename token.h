#ifndef TOKEN_H
#define TOKEN_H

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

    MATH_OPERATION,
    MATH_PLUS,
    MATH_MINUS,
    MATH_MULT,
    MATH_DIV, 

}
TokenType;

void tokenize(char *text, Token *tokenarr, int tokensize, int *tokencount);

void freeTokens(Token *tokens, int count);

#endif