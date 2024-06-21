# TODO:
#     correct grammar and AST, make it work, check an example
#     transform AST to IR (which is a linked list of instructions)
#     optimize IR (only if we have time)
#     transform IR instructions to either assembly or binary

# ARM Specification
# -- how we work with registers
# -- how we store in memory

X0 - X15 : caller saved
X15 - X30 : calle saved
SP: even though it is stack pointer, use it as return address
X0 : value returned from function
X1 - X8 : parameters to pass to function called
A stack

# Notes
Store all variable values in a map. (name to value)
Store all functions defined in a map. (name to line location mostly)

Memory: store all directives (numbers) right below the program.
Store them into a vector and output them in the end.

Do not handle much error checking at the moment.
