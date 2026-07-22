#ifndef AST_H
#define AST_H

#include "token.h"
#include "utils.h"

typedef enum {

    AST_INT,
    AST_FLOAT,
    AST_VARIABLE,
    AST_MATHOP,
    AST_SIGNAL,
    AST_ASSIGN,
    AST_FUNCTION,

} ASTNodeType;

typedef struct ASTNode {

    ASTNodeType type;

    union {

        long integer;

        double floater;

        char *variable;

        struct {

            Token op;
            struct ASTNode *left;
            struct ASTNode *right;

        } Operator;

        struct {

            Token op;   //OPERATION
            struct ASTNode *child; //expression

        } signal;

        struct {

            struct ASTNode *left; //VAR
            struct ASTNode *right; //EXPRESSION

        } assign;

        struct {

            char *name;
            struct ASTNode **args;
            size_t argc;

        } function;

    } value;
} ASTNode;  

ASTNode *ast_newInt(long value);
ASTNode *ast_newFloat(double value);
ASTNode *ast_newVariable(const char *name);
ASTNode *ast_newOperator(Token op,
                  ASTNode *left,
                  ASTNode *right);
ASTNode *ast_newSignalizer(Token op, ASTNode *child);
ASTNode *ast_newAssign(ASTNode *left, ASTNode *right);
ASTNode *ast_newFunction(const char *name, ASTNode **args, size_t argc);
void ast_free(ASTNode *node);

#endif