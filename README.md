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

varname = func(parameters) operation number


Syntax examples:
x = 1
x = 1 + 2
x = x * 4
y = x * 2.3 + 4 + (x-2)/7
x = y = z = 3
x = abc = x - 4 ^ 3
x = randint(1,100) + 3


Invalid varnames:
Varnames cannot have math operation signals, brackets or any type of number.


Invalid varnames (examples):

123

a12

bc1

b+a

exit()

Possible math operations:

Plus: +

Minus: -

Multiplication: *

Division (complete division) : /

Module: %

Pow: ^

Assignemt of variable:

varname = ...


Functions already developed:

| NOTE - vars can be used instead of directly using numbers |

test(number)

exit()

sqrt(non-negative number)

sin(number)

cos(number)

randint(num_min,num_max)

randprob() //returns a probability (from 0.0 to 1.0)

Functions can be added on builtin_funcs.c and .h by the schematics on file

Example of test and randint can be helpful

Just don't forget to add to the funcs dictionary on the register

Every argument in the functions are RuntimeValue.

If you want to raise a error use RuntimeError and print your description of the error. (probably you need to import in the function the var RuntimeContext to auto-put errors in the interpreter)

Bug: Adding strings can do a little of tricky numbers, so if you want big numers try to add strings


