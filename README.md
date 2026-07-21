# mather
C-Interpreter of Math, can run variables, operations, etc...

Syntax:

varname = number

varname = varname operation varname ...

varname = varname operation number ...

varname = number operation number ...

varname = varname = varname ....

varname = varname = ... = number

varname = varname = ... = number operation number ...

Syntax examples:
x = 1
x = 1 + 2
x = x * 4
y = x * 2.3 + 4 + (x-2)/7
x = y = z = 3
x = abc = x - 4 ^ 3

Invalid varnames:
Varnames cannot have math operation signals or any type of number.

Invalid varnames (examples):

123
a12
bc1
b+a

Possible math operations:
Plus: +
Minus: -
Multiplication: *
Division "floor division": /
Division "complete": int/double or double/int or double/double (to be a double can be used for example 2. )
Module: %
Simple Pow: ^

Assignemt of variable:
varname = ...

Complex functions are to be created
