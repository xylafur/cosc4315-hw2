# cosc4315-hw4

PSID        |   Name                |   Unix id
---------------------------------------------------
1495728     |   Raymond Sutrisno    |   cosc3001
1555520     |   James Richardson    |   cosc4315

Compilation:
    To compile our program, simply run `make` in this directory.

Running:
    To run the program, run the executable `mypython` (it should have the
    correct execute permissions by default) and then supply an input file.

    example:
        ./mypython <file>

    Interactive mode is extremley limited and will not evaluate the input until
    you send the EOF (ctrl^D).

    example:
        ./mypython

    If you want to see that AST that we construct as well then run the program
    as such:
        ./mypython tree <file>

Given Testcases:
    Our code will pass all testcases given in testing/*.py

Drawbacks:
    The recursion detection will only work for very specific test cases.

    Anything in the form of:

        def f(n):
            if <condition>:
                return <base case>
            else:
                return <expression with recursive function call>

Implementation:
    Orphan Else detection:
        I used a complex automata to implement this and an array to keep track
        of wether we are expecting an else statment for a given indention level
        or not.

        This can be observed in ./src/parsing/lexer.l at line 314 in the
        funciton 'get_next_token'.  It is commented and explains how it
        functions.

    Nested if Else Detection:

    Recursion Detection:



