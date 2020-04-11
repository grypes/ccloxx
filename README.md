# Introduction
Ccloxx is a toy interpreter for the Lox language specified in Bob Nystrom's amazing book [Crafting Interpreters](http://craftinginterpreters.com). I writed this as an exercise following the book, so it's probably not production-quality code.

Compilation requires a C++11-compliant compiler. 

    cmake .
	make

Ccloxx supports dynamic typing, lexical scope, control flow, and functions. For example:

    // ccloxx ./UserScripts/fibonacci.lox
    fun fibonacci(n) {
        if (n <= 1) return n;
        return fibonacci(n - 2) + fibonacci(n - 1);
    }

    for (var i = 0; i < 20; i = i + 1) {
        print fibonacci(i);
    }

Ccloxx alos supports functions as *first class*, which means we can use closure. 

    // ccloxx ./UserScripts/closure.lox
    fun line_conf(a, b) {
        fun line(x) {
            return a*x + b;
        }
        return line;
    }

    var line1 = line_conf(1, 1);
    var line2 = line_conf(4, 5);
    print line1(5); // "6".
    print line2(4); // "21".

 For more details on Lox's syntax, check out the [description](http://craftinginterpreters.com/the-lox-language.html) in Bob's book.

# Usage 

Here are some examples in `./UserScripts`. To run stuff interactively using a REPL, do
    
    ./loxx

Alternatively, execute source files like so

    ./loxx <your source filename>