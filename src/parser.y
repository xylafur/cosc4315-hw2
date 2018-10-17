%{
/*  C declarations  */
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "node_util.h"

//we use this to allow us to compile lex as C code and then bison as c++
extern "C"
{
    int yylex(void);

    int yywrap()
    {
        return 1;
    }
}

int yyparse(void);

void yyerror(const char * str)
{
    fprintf(stderr, "Error: %s\n", str);
}

int mod = 1;
int value_list_length = 0;
int num_stmts = 0;

%}

%token DEF RETURN PRINT IF ELSE FOR WHILE TRUE FALSE AND OR
%token STRING NUMBER REAL IDENTIFIER
%token STAR PLUS SLASH MINUS EQUALSEQUALS EQUALS 
%token LPARENTH RPARENTH COMMA BACKSLASH COLON
%token INDENT DEDENT NEWLINE WHITESPACE

%union {
    long num;
    float real;
    char * str;
};

%type <str> IDENTIFIER
%type <num> NUMBER
%type <real> REAL

%%

program:    %empty
       |    program stmt
       {
            printf("Matched program\n");
       }
       ;


stmt:   simple_stmt
    |   compound_stmt
    |   NEWLINE
    {
        printf("Matched stmt\n");
    }
    ;

simple_stmt:    single_stmt NEWLINE
           {
                printf("Matched simple stmt\n");
           }
           ;

mult_stmts: %empty 
          | mult_stmts simple_stmt
          {
            printf("mult_stmts_simple\n");
            num_stmts++;
          }
          | mult_stmts compound_stmt
          { 
            printf("mult_stmts_compound\n");
            num_stmts++;
          }
          | mult_stmts NEWLINE
          ;

single_stmt:    print_stmt
           |    assignment
           |    value_list { value_list_length = 0; }
           |    return_stmt
           {
                printf("Single stmt\n");
           }
           ;

compound_stmt:  branch_stmt
             |  func_def
             {
                printf("compound stmt\n");
             }
             ;

func_def:   DEF IDENTIFIER LPARENTH RPARENTH COLON block_stmt
        {
            node_ptr block = pop_node_from_stack();
            node_ptr func = create_func_def_node($2, block);
            push_node_to_stack(func);
            printf("func def\n");
        }
        ;

branch_condition:   expr1
                |   LPARENTH expr1 RPARENTH
                ;

branch_stmt:    IF branch_condition COLON block_stmt
           {
                node_ptr block = pop_node_from_stack();
                node_ptr condition = pop_node_from_stack();
                node_ptr branch = create_branch_no_else_node(condition, block);
                push_node_to_stack(branch);
                printf("branch no else\n");
           }
           |    IF branch_condition COLON block_stmt ELSE COLON block_stmt
           {
                node_ptr else_block = pop_node_from_stack();
                node_ptr if_block = pop_node_from_stack();
                node_ptr condition = pop_node_from_stack();

                node_ptr branch = create_branch_with_else_node(condition,
                                                               if_block,
                                                               else_block);
                push_node_to_stack(branch);
                printf("branch with else\n");
           }
           ;

block_stmt: NEWLINE INDENT mult_stmts DEDENT
          {
            node_array children = alloc_children(num_stmts);
            for(int ii = 0; ii < num_stmts; ii++)
                children[num_stmts - ii - 1] = pop_node_from_stack();
            node_ptr node = create_block_stmt_node(num_stmts, children);
            push_node_to_stack(node);
            num_stmts = 0;
            printf("block stmt\n");
          }
          ;

print_stmt: PRINT LPARENTH value_list RPARENTH
          {
            node_array children = alloc_children(value_list_length);
            for(int ii = 0; ii < value_list_length; ii++){
                children[value_list_length - ii - 1] = pop_node_from_stack();
            }
            node_ptr node = create_print_node(value_list_length, children);
            push_node_to_stack(node);
            value_list_length = 0;
            printf("print stmt\n");
          }
          ;

assignment: IDENTIFIER EQUALS value
          {
            printf("Assignment!\n");

            node_ptr value = pop_node_from_stack();
            node_ptr ident = create_identifier_node($1);
            node_ptr ass = create_assignment_node(ident, value);
            push_node_to_stack(ass);
          }
          ;

value:  STRING  {
                    node_ptr node = create_string_node(yyval.str);
                    push_node_to_stack(node);
                }
     |  expr1
     {
        printf("found value\n");
     }
     ;

value_list: value_list COMMA value
          | value
          {
            value_list_length += 1;
            printf("value list\n");
          }
          ;

func_call:  IDENTIFIER LPARENTH RPARENTH
         {
            node_ptr node = create_func_call_node(yyval.str);
            push_node_to_stack(node);
            printf("Function call!\n");
         }
         ;

return_stmt:    RETURN value
           {
                node_ptr value = pop_node_from_stack();
                node_ptr node = create_return_stmt(value);
                push_node_to_stack(node);
                printf("REturn stmt!\n");
           }
           ;

expr1: expr1 OR expr2           {
                                    push_operator(OR);
                                }
     | expr2 
     ;

expr2: expr2 AND expr3          {
                                    push_operator(AND);
                                }
     | expr3
     ;

expr3: expr4 EQUALSEQUALS expr4 {
                                    push_operator(EQUALSEQUALS);
                                }
     | expr4
     ;

expr4:  expr4 PLUS expr5    {
                                push_operator(PLUS);
                            }
     |  expr4 MINUS expr5   {
                                push_operator(MINUS);
                            }
     |  expr5
     ;

expr5:  expr5 STAR expr6    {
                                push_operator(STAR);
                            }
     |  expr5 SLASH expr6   {
                                push_operator(SLASH);
                            }
     |  expr6
     {
        printf("matched expr5\n");
     }
     ;

expr6:  PLUS expr6
     |  MINUS expr6     {
                            mod = mod * -1;
                        }
     |  func_call
     |  bool
     |  IDENTIFIER      {
                            node_ptr node = create_identifier_node(yyval.str);
                            apply_negation_push();
                            mod = 1;
                            printf("Found identifier %s\n", yyval.str);
                        }
     |  NUMBER          {
                            node_ptr node = create_number_node(yyval.num);
                            apply_negation_push();
                            mod = 1;
                            printf("Found number: %d\n", yyval.num);
                        }
     ;

bool:   TRUE        {
                        node_ptr node = create_number_node(1);
                        push_node_to_stack(node);
                    }
    |   FALSE       {
                        node_ptr node = create_number_node(0);
                        push_node_to_stack(node);
                    }
    ;
%%
