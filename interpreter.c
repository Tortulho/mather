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

RuntimeValue interpret(ASTNode *node, Dictionary *variables, Dictionary *functions) {

    if (node == NULL)
        exit(EXIT_FAILURE);

    switch (node->type) {

        case AST_INT:
            return interpretInteger(node);

        case AST_FLOAT:
            return interpretFloat(node);

        case AST_ASSIGN:
            return interpretAssign(node,variables,functions);

        case AST_VARIABLE:
            return interpretVariable(node,variables);

        case AST_SIGNAL:
            return interpretSignal(node,variables,functions);

        case AST_MATHOP:
            return interpretOperator(node,variables,functions);

        case AST_FUNCTION:
            return interpretFunction(node,variables,functions);

        default:
            exit(EXIT_FAILURE);
    }
}

RuntimeValue interpretInteger(ASTNode *node) {

    RuntimeValue value;

    value.type = VALUE_INT;
    value.value.integer = node->value.integer;

    return value;
}

RuntimeValue interpretFloat(ASTNode *node) {

    RuntimeValue value;

    value.type = VALUE_FLOAT;
    value.value.floater = node->value.floater;

    return value;
}

RuntimeValue interpretSignal(ASTNode *node, Dictionary *variables, Dictionary *functions) {

    RuntimeValue value;

    value = interpret(node->value.signal.child, variables, functions);

    switch (node->value.signal.op.value.numINT) {

        case '+':
            return value;

        case '-':

            if (value.type == VALUE_INT)
                value.value.integer = -value.value.integer;

            else if (value.type == VALUE_FLOAT)
                value.value.floater = -value.value.floater;

            else
                exit(EXIT_FAILURE);

            return value;

        default:
            exit(EXIT_FAILURE);
    }
}

RuntimeValue interpretOperator(ASTNode *node, Dictionary *variables, Dictionary *functions) {

    RuntimeValue left;
    RuntimeValue right;
    RuntimeValue result;

    left = interpret(node->value.Operator.left, variables, functions);
    right = interpret(node->value.Operator.right, variables, functions);

    /* Pelo menos um float */
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
                exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
    }

    return result;
}

RuntimeValue interpretVariable(ASTNode *node, Dictionary *variables) {

    if (node == NULL) exit(EXIT_FAILURE);
    char *varname = node->value.variable;
    
    RuntimeValue *value;
    if (!dictExist(variables,varname,(RuntimeValue **)&value)) exit(EXIT_FAILURE);
    if (value == NULL) exit(EXIT_FAILURE);

    return *value;
}

RuntimeValue interpretAssign(ASTNode *node, Dictionary *variables, Dictionary *functions) {

    if (node == NULL) exit(EXIT_FAILURE);
    ASTNode *left = node->value.assign.left;
    if (left->type != AST_VARIABLE)
        exit(EXIT_FAILURE);
    const char *name = left->value.variable;

    RuntimeValue result = interpret(node->value.assign.right, variables, functions);

    RuntimeValue *stored = malloc(sizeof(RuntimeValue));
    if (stored == NULL) exit(EXIT_FAILURE);
    *stored = result;

    RuntimeValue *old;
    //IF ALREADY EXISTS VAR
    if (dictExist(variables, name, (RuntimeValue**)&old)) {
        dictRemove(variables, name);
        free(old);
    }
    if (!dictInsert(variables, name, stored)) exit(EXIT_FAILURE);
    return result;
}

RuntimeValue interpretFunction(ASTNode *node,
                               Dictionary *variables,
                               Dictionary *functions) 
{
    if (node == NULL) exit(EXIT_FAILURE);
    
    BuiltinFunction function;
    function = (BuiltinFunction)dictGet(functions,
                                        node->value.function.name);

    if (function == NULL) {
        printf("Error: Unknown function '%s'.\n", node->value.function.name);
        exit(EXIT_FAILURE);
    }

    RuntimeValue *args;

    args = malloc(node->value.function.argc * sizeof(RuntimeValue));
    if (args == NULL) exit(EXIT_FAILURE);

    for (size_t i = 0; i < node->value.function.argc; i++) {

        args[i] = interpret(node->value.function.args[i], variables, functions);
    }

    RuntimeValue result;

    result = function(args, node->value.function.argc);

    free(args);
    return result;

}