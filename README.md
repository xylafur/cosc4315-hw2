# cosc4315-hw2

Just run `make` and the make file will automatically create the mypython
program

Then to run mypython with a file simply call ./mypython <filename>


METHODOLOGY:
    In this assignment, we used flex to recognize our tokens, bison to generate
    our grammar and construct our ast and then a virtual machine that operates
    on an intermediate bytecode language.  The AST is converted into this
    intermediate bytecode language and then executed by the virtual machine.

    We are able to lex, parse and create the AST in a single pass.


OUTPUT:
    The mypython program will output the AST.

    There are testcases located at testing/testfiles/*.py
    all of these testfiles output the correct AST except for cases 6 and 7
        the reason for this failure is mentioned in the limitations


LIMITATIONS:
    The AST is not converted into the bytecode and executed by the VM:
        We have the virtual machine completed and it functions properly.  IF
        given a bytecode file the VM will execute it.  The grammar is also
        recognized and the AST is constructed properly for all cases (other
        than the case listed below) but we ran out of time working on the code
        that converts the AST into the bytecode.

        Please check and consider the fact that our VM functions.  See the
        Section at the bottom of this file about how to verify the
        functionality of the VM.

    The mypython program cannot parse a very specific set of commands
    correctly.
        when a function of if stmt is found directly after another if stmt
        inside of an if stmt, the parser is not able to handle this.  For
        example this code will not parse correctly:
        
            if(True):
               if(True):
                  print("Yes it's True")
               if(True):
                  print("This is also true")
        
        This is because the parser is not sure if we are parsing an ifstmt with
        the else or without the else until it sees the next token after the
        dedent.  There is a special case in our parser file in which when we
        see an if token we start a new "stmt count".  This count will let us
        know how many stmts we encountered inside of a block.  We create the
        new stmt count before we have poped all of the values from the block
        into an AST node, this causes significant problems.


HOW TO VERIFY THE BYTECODE WORKS CORRECTLY:
    Currently there is no way to verify this.  There was, but last minute
    changes broke it.  Consider this a placeholder, if my partner is able to
    fix his code then we will add instructions on how to run a bytecode file

