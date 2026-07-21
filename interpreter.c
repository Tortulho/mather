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

RuntimeValue interpret(ASTNode *node, Dictionary *variables) {

    if (node == NULL)
        exit(EXIT_FAILURE);

    switch (node->type) {

        case AST_INT:
            return interpretInteger(node);

        case AST_FLOAT:
            return interpretFloat(node);

        case AST_ASSIGN:
            return interpretAssign(node,variables);

        case AST_VARIABLE:
            return interpretVariable(node,variables);

        case AST_SIGNAL:
            return interpretSignal(node,variables);

        case AST_MATHOP:
            return interpretOperator(node,variables);

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

RuntimeValue interpretSignal(ASTNode *node, Dictionary *variables) {

    RuntimeValue value;

    value = interpret(node->value.signal.child, variables);

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

RuntimeValue interpretOperator(ASTNode *node, Dictionary *variables) {

    RuntimeValue left;
    RuntimeValue right;
    RuntimeValue result;

    left = interpret(node->value.Operator.left, variables);
    right = interpret(node->value.Operator.right, variables);

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
            default:
                exit(EXIT_FAILURE);
        }

        return result;
    }

    if (node->value.Operator.op.value.numINT == '^') {
        double l;

        if (left.type == VALUE_FLOAT)
            l = left.value.floater;
        else
            l = (double)left.value.integer;

        if (right.type == VALUE_FLOAT)
            exit(EXIT_FAILURE);
        result.type = VALUE_FLOAT;
        long r = right.value.integer;

        result.value.floater = powto(l,r);
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
            result.value.integer = left.value.integer / right.value.integer;
            break;

        case '%':
            result.value.integer = left.value.integer % right.value.integer;
            break;

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

RuntimeValue interpretAssign(ASTNode *node, Dictionary *variables) {

    ASTNode *left = node->value.assign.left;
    if (left->type != AST_VARIABLE)
        exit(EXIT_FAILURE);
    const char *name = left->value.variable;

    RuntimeValue result = interpret(node->value.assign.right, variables);

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