/*

evaluate(node)

    se INT
        devolver inteiro

    se FLOAT
        devolver float

    se VARIABLE
        procurar variável na tabela de símbolos

    se SIGNAL

        value = evaluate(child)

        aplicar operador

        devolver resultado

    se OPERATOR

        left = evaluate(left)

        right = evaluate(right)

        aplicar operador

        devolver resultado

*/

#include "ast.h"
#include "token.h"
#include "utils.h"
#include "parser.h"
#include "interpreter.h"
#include "dict.h"
#include "builtinfuncs.h"
#include "dynarr.h"
#include "runtime.h"
#include <stdio.h>
#include <math.h>
#include "error.h"

RuntimeValue interpret(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx) {

    if (node == NULL) {
        runtimeError(ctx,ERROR_MEMORY,"AST tree memory error - NULL node.");
        return INVALID_VALUE;
    }

    switch (node->type) {

        case AST_INT:
            return interpretInteger(node, ctx);

        case AST_FLOAT:
            return interpretFloat(node, ctx);

        case AST_ASSIGN:
            return interpretAssign(node,variables,functions, ctx);

        case AST_VARIABLE:
            return interpretVariable(node,variables, ctx);

        case AST_SIGNAL:
            return interpretSignal(node,variables,functions, ctx);

        case AST_MATHOP:
            return interpretOperator(node,variables,functions, ctx);

        case AST_FUNCTION:
            return interpretFunction(node,variables,functions, ctx);

        default:
        runtimeError(ctx,
                     ERROR_RUNTIME,
                     "Unknown AST node at interpreter.");
        return INVALID_VALUE;
    }
}

RuntimeValue interpretInteger(ASTNode *node, RuntimeContext *ctx) {

    RuntimeValue value;

    value.type = VALUE_INT;
    value.value.integer = node->value.integer;

    return value;
}

RuntimeValue interpretFloat(ASTNode *node, RuntimeContext *ctx) {

    RuntimeValue value;

    value.type = VALUE_FLOAT;
    value.value.floater = node->value.floater;

    return value;
}

RuntimeValue interpretSignal(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx) {

    RuntimeValue value;

    value = interpret(node->value.signal.child, variables, functions, ctx);

    switch (node->value.signal.op.value.numINT) {

        case '+':
            return value;

        case '-':

            if (value.type == VALUE_INT)
                value.value.integer = -value.value.integer;

            else if (value.type == VALUE_FLOAT)
                value.value.floater = -value.value.floater;

            else {
                runtimeError(ctx,ERROR_DOMAIN,"Math signal before a no-number at interpreter.");
                return INVALID_VALUE;
            }

            return value;

        default:
            runtimeError(ctx,ERROR_CORRUPTED_MEMORY,"Corrupted memory on process at interpreter - invalid signal before number.");
            return INVALID_VALUE;
    }
}

RuntimeValue interpretOperator(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx) {

    RuntimeValue left;
    RuntimeValue right;
    RuntimeValue result;

    left = interpret(node->value.Operator.left, variables, functions, ctx);
    right = interpret(node->value.Operator.right, variables, functions, ctx);

    if (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT) {

        double l;
        double r;

        if (left.type == VALUE_FLOAT)
            l = left.value.floater;
        else
            l = (double)left.value.integer;

        if (right.type == VALUE_FLOAT)
            r = right.value.floater;
        else
            r = (double)right.value.integer;

        result.type = VALUE_FLOAT;

        switch (node->value.Operator.op.value.numINT) {

            case '+':
                result.value.floater = l + r;
                break;

            case '-':
                result.value.floater = l - r;
                break;

            case '*':
                result.value.floater = l * r;
                break;

            case '/':
                result.value.floater = l / r;
                break;
            case '^':
                result.value.floater = pow(l,r);
                break;
            default:
                runtimeError(ctx,ERROR_UNEXPECTED_TOKEN,"Invalid math operation '%d' at interpreter.",node->value.Operator.op.value.numINT);
                return INVALID_VALUE;
        }

        return result;
    }

    /* Ambos são inteiros */
    result.type = VALUE_INT;

    switch (node->value.Operator.op.value.numINT) {

        case '+':
            result.value.integer = left.value.integer + right.value.integer;
            break;

        case '-':
            result.value.integer = left.value.integer - right.value.integer;
            break;

        case '*':
            result.value.integer = left.value.integer * right.value.integer;
            break;

        case '/':
            result.type = VALUE_FLOAT;
            result.value.floater = (double)left.value.integer / (double)right.value.integer;
            return result;

        case '%':
            result.value.integer = left.value.integer % right.value.integer;
            break;

        case '^':
            result.value.floater = pow(left.value.integer,right.value.integer);
            result.type = VALUE_FLOAT;
        default:
            runtimeError(ctx,ERROR_UNEXPECTED_TOKEN,"Invalid math operation '%d' at interpreter.",node->value.Operator.op.value.numINT);
            return INVALID_VALUE;
    }

    return result;
}

RuntimeValue interpretVariable(ASTNode *node, Dictionary *variables, RuntimeContext *ctx) {

    if (node == NULL) {
        runtimeError(ctx,ERROR_MEMORY,"AST tree memory error - NULL node.");
        return INVALID_VALUE;
    }
    char *varname = node->value.variable;
    
    RuntimeValue *value;
    if (!dictExist(variables,varname,(RuntimeValue **)&value)) {
        runtimeError(ctx,ERROR_UNKNOWN_VARIABLE,"Variable '%s' does not exist yet.",varname);
        return INVALID_VALUE;
    }
    if (value == NULL) {
        runtimeError(ctx,ERROR_CORRUPTED_MEMORY,"Variable '%s' does not have valid atributes.",varname);
        return INVALID_VALUE;
    }

    return *value;
}

RuntimeValue interpretAssign(ASTNode *node, Dictionary *variables, Dictionary *functions, RuntimeContext *ctx) {

    if (node == NULL) {
        runtimeError(ctx,ERROR_MEMORY,"AST tree memory error - NULL node.");
        return INVALID_VALUE;
    }
    ASTNode *left = node->value.assign.left;
    if (left->type != AST_VARIABLE) {
        runtimeError(ctx,ERROR_UNKNOWN_VARIABLE,"Unknown/Invalid variable to make operation.");
        return INVALID_VALUE;
    }
    const char *name = left->value.variable;

    RuntimeValue result = interpret(node->value.assign.right, variables, functions, ctx);

    RuntimeValue *stored = malloc(sizeof(RuntimeValue));
    if (stored == NULL) {
        runtimeError(ctx,ERROR_MEMORY,"Cannot allocate more memory to the program.");
        return INVALID_VALUE;
    }
    *stored = result;

    RuntimeValue *old;
    //IF ALREADY EXISTS VAR
    if (dictExist(variables, name, (RuntimeValue**)&old)) {
        dictRemove(variables, name);
        free(old);
    }
    if (!dictInsert(variables, name, stored)) {
        runtimeError(ctx,ERROR_MEMORY,"Cannot allocate more memory to the program.");
        return INVALID_VALUE;
    }
    return result;
}

RuntimeValue interpretFunction(ASTNode *node,
                               Dictionary *variables,
                               Dictionary *functions, RuntimeContext *ctx) 
{
    if (node == NULL) {
        runtimeError(ctx,ERROR_MEMORY,"AST tree memory error - NULL node.");
        return INVALID_VALUE;
    }
    
    BuiltinFunction function;
    function = (BuiltinFunction)dictGet(functions,
                                        node->value.function.name);

    if (function == NULL) {
        runtimeError(ctx,ERROR_UNKNOWN_FUNCTION,"Unknown function '%s'.", node->value.function.name);
        return INVALID_VALUE;
    }

    RuntimeValue *args;

    args = malloc(node->value.function.argc * sizeof(RuntimeValue));
    if (args == NULL) {
        MEMERRORMSG;
        return INVALID_VALUE;
    }

    for (size_t i = 0; i < node->value.function.argc; i++) {

        args[i] = interpret(node->value.function.args[i], variables, functions, ctx);
    }

    RuntimeValue result;

    result = function(args, node->value.function.argc,ctx);
    

    free(args);
    return result;

}