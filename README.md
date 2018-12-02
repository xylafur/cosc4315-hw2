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
    Orphan Else detection (James) :
        I used a complex automata to implement this and an array to keep track
        of wether we are expecting an else statment for a given indention level
        or not.

        This can be observed in ./src/parsing/lexer.l at line 314 in the
        funciton 'get_next_token'.  It is commented and explains how it
        functions.


    Nested if Else Detection (Ray) :
        My algorithm is as follows:
        1st: Find all root-if-statement ParseTreeNodes. Meaning for all of the
        children of the root program ParseTreeNode, if that child is an if-statement
        node, add it to this list, else if that child is a function definition, 
        recursively search inside that node for root-if-nodes

        2nd: Once all root-if-statement nodes are found. For all of root-if-statement 
        nodes, count the depth of the node

    Mutation Detection (Ray) :
        1. Create an unordered map of key:string, value:int pairs which
        record the number of assignments a variable has

        2. For each assignment ParseTreeNode in the entire ParseTree 
            - set the key equal to the variable being assigned
            - if the variable being assigned is inside of a function,
            prefix the key with the function name
            - set the value to the current frequency + 1

            ex:
                code:
                    def foo():
                        a = 0
                        a = 1
                    b = 2

                unordered map contents:
                    {"foo::a":2, "b":1}

        3. A variable is considered mutated if the number of assignments
        is greater than 1

    Recursion Detection (Ray and Kesley) :
        TBD

