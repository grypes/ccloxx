# ccloxx
This is a toy interpreter for the Lox language specified in Bob Nystrom's amazing book [Crafting Interpreters](http://craftinginterpreters.com). I writed this as an exercise following the book, so it's probably not production-quality code.

Compilation requires a C++11-compliant compiler. 

    cmake .
	make
	./ccloxx ./UserScripts/fib.lox
    ./ccloxx ./UserScripts/local_var.lox

## What ccloxx has done (following the book)
- tree-walk interpreter
- lexical scope
- environment chains for storing variables
- control flow

## What to do next
- function
- class (inheritance)
- errorHandler