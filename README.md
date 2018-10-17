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

    But we were unfortunately unable to complete the code which coverts the AST
    into bytecode.


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


VIRTUAL MACHINE

    The virtual machine is a operand stack based computer, ie it operates view
    an operand stack as opposed to a registers. This decision was based on 
    our initial presumptions that it would be easier in to generate code for a 
    stack machine when traversing the AST in post order.

    to run the VM, see the SECTION RUNNING THE BYTECODE VM at the bottom of this readme

    For example traversing the following tree in post order yields the following
    bytecode.


            (assn)                      push_reference  x
           /      \                     push_num        2
        ident      (expr: +)        ->  push_reference  y
          x       /        \            addi
                 num      ident         assn
                  2         y

    The order of execution is that the x reference is pushed onto the operand
    stack. Then the number 2, then the reference y

    When the addi instruciton is executed, it automatically dereferences
        any references so that it is working with only integer values.
        it then computes 2 + deref(y) and pushes the result on the operand stack

    Which leaves two items on the stack now

     top ->[ num: 2+y ]
           [ ident: x ]

    the assignment operator then pops these two values off and assigns the number
     2+y whatever it may have computed to and writes it to to x.

    References in this case are indices which index the stack frame variable

    IMPLEMENTATION
        This virtual machine reads instructions 8 bytes at a time
        the most significant byte is the bytecode operation and the last
        7 are used for arguments

        most binary operator expressions like integer arithmetic and arithmetic
        comparison do not use the 7 bytes, however instructions like branching
        do use the last 7 bytes as the amount to increase the program counter by
        These instructions take this form:
            most significant  least significant
                      [code] [ 7 bytes ... ]

        The only special instruction that has a different shape is the
        function call instruction or fpush:
            most significant  least significant
                      [code] [# stack frame variables] [ 6 bytes ... ]

        It was planned that the second byte after the code was to denote the
        amount of stack frame variables needed for that function and the last
        6 bytes were for the address of where the function begins.

        When f_push is called three things occur:
           * the current program counter is pushed onto the function_stack,
           * a new vector<uint64_t> is pushed to the function_stack_variables
             to be used for storing the variables in that function. It is initialized
             using the second byte as argument
           * since there is only one operand stack it is important to give the
             stack some scope so we push 0 to operand_stack_scope stack.
           * the program counter is then set to whatever that last 6 bytes were

        By implementing functions in this manner, it is possible to do recursion

        functions return with a f_return_item instruction
            * whatever is currently on top of the operand stack is stowed away
            * all operands are popped off the operand stack until the top of 
              operand_stack_scope is zero. Then the top of operand_stack_scope
              is popped
            * the previous program counter value is popped off the function stack and
            assigned to the program counter


RUNNING THE BYTECODE VM
    All of the vm files are in the folder "bytecode_stuff"
    simply run make when inside the directory and it will compile
    virtual_machine.cpp into an executable called breader

    to run the VM on a bytecode binary:
        ./breader bcode.bin

    to make custom bytecode files an editor is include called editor.py
    this editor is interactive however it is also scriptable, simply use standard input
    piping
        example:
            ./editor.py < bcode.txt

    The bcode.txt included implements this program:
        def f():
           x = 3 * 2
           y = 3 - 2
           z = 3 + 2
           x = l
           if y > l:
              l = y
           if z > l:
              l = z
           return l
        print(f())

    this will print out 6

    The file hello.txt implements this program
        print("Hello, World!")

    To write your own bytecode simply follow the style as the sample txt files
        -> A line which does not match an editor instruction is assumed to be
            the name of the file to output the binary

    then compile it like this:
        ./editor.py < [name of editor file]

    then run it like this
        ./breader [name of saved binary]

    Currently the virtual_machine has support for recursion as it has a working
    function stack which stores the program counters to return to and proper
    function stack frame variables.


