#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"
#include "dict.h"
#include "builtinfuncs.h"
#include "dynarr.h"
#include "runtime.h"
#include "error.h"

// typedef struct {

//     enum {
//         VALUE_INT,
//         VALUE_FLOAT,
//         VALUE_VAR,
//     } type;

//     union {
//         long integer;
//         double floater;
//     } value;

// } RuntimeValue;

RuntimeValue interpret(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx);

RuntimeValue interpretInteger(ASTNode *node, RuntimeContext *ctx);
RuntimeValue interpretFloat(ASTNode *node, RuntimeContext *ctx);
RuntimeValue interpretSignal(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx);
RuntimeValue interpretOperator(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx);
RuntimeValue interpretVariable(ASTNode *node, Dictionary *variables, RuntimeContext *ctx);
RuntimeValue interpretAssign(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx);
RuntimeValue interpretFunction(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx);

#endif