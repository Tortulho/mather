#include "token.h"
#include "ast.h"

ASTNode *ast_newInt(long value) {

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;

    node->type = AST_INT;
    node->value.integer = value;

    return node;

}

ASTNode *ast_newFloat(double value) {

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;

    node->type = AST_FLOAT;
    node->value.floater = value;

    return node;

}

ASTNode *ast_newVariable(const char *name) {

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    char *var = calloc(strlen(name)+1,1);
    if (var == NULL) {
        free(node);
        return NULL;
    }
    strcpy(var,name);

    node->type = AST_VARIABLE;
    node->value.variable = var; //FIXED

    return node;
}

ASTNode *ast_newAssign(ASTNode *left, ASTNode *right) {

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL;

    node->type = AST_ASSIGN;

    node->value.assign.left = left;
    node->value.assign.right = right;

    return node;
}

ASTNode *ast_newOperator(Token op,
                  ASTNode *left,
                  ASTNode *right) 
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;

    node->type = AST_MATHOP;
    node->value.Operator.op = op;
    node->value.Operator.left = left;
    node->value.Operator.right = right;

    return node;
}

ASTNode *ast_newSignalizer(Token op, ASTNode *child) {

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;

    node->type = AST_SIGNAL;

    node->value.signal.op = op;
    node->value.signal.child = child;

    return node;
}

ASTNode *ast_newFunction(
    const char *name,
    ASTNode **args,
    size_t argc
) {
    if (name == NULL) return NULL;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;

    char *funcname = calloc(strlen(name) + 1, 1);
    if (funcname == NULL) {
        free(node);
        return NULL;
    }

    strcpy(funcname, name);

    node->type = AST_FUNCTION;

    node->value.function.name = funcname;
    node->value.function.args = args;
    node->value.function.argc = argc;

    return node;
}


void ast_free(ASTNode *node) {

    if (node == NULL)
        return;

    switch (node->type) {

        case AST_INT:
            break;

        case AST_FLOAT:
            break;

        case AST_VARIABLE:

            //free(node->value.variable);
            break;

        case AST_SIGNAL:

            ast_free(node->value.signal.child);
            break;

        case AST_MATHOP:

            ast_free(node->value.Operator.left);
            ast_free(node->value.Operator.right);
            break;

        case AST_ASSIGN:

            ast_free(node->value.assign.left);
            ast_free(node->value.assign.right);
            break;

        case AST_FUNCTION:

            for (size_t i = 0; i < node->value.function.argc; i++) {
                ast_free(node->value.function.args[i]);
            }

            free(node->value.function.args);
            free(node->value.function.name);

            break;

        default:
            break;
    }

    free(node);
}