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

RuntimeValue interpret(ASTNode *node, Dictionary *variables, Dictionary *functions);

RuntimeValue interpretInteger(ASTNode *node);
RuntimeValue interpretFloat(ASTNode *node);
RuntimeValue interpretSignal(ASTNode *node, Dictionary *variables, Dictionary *functions);
RuntimeValue interpretOperator(ASTNode *node, Dictionary *variables, Dictionary *functions);
RuntimeValue interpretVariable(ASTNode *node, Dictionary *variables);
RuntimeValue interpretAssign(ASTNode *node, Dictionary *variables, Dictionary *functions);
RuntimeValue interpretFunction(ASTNode *node, Dictionary *variables, Dictionary *functions);

#endif