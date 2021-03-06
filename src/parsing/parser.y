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

    extern int num_stmts;
    void enter_block();
    void exit_block();
}

int yyparse(void);

void yyerror(const char * str)
{
    fprintf(stderr, "Error: %s\n", str);
}

int mod = 1;
int orphan_else = 0;

int func_param_list_length = 0;
int ident_list_length = 0;

#define inc_func_param_list() func_param_list_length += 1
#define reset_func_param_list() func_param_list_length = 0

%}

%token DEF RETURN PRINT IF ELSE FOR WHILE TRUE FALSE AND OR
%token STRING NUMBER REAL IDENTIFIER
%token STAR PLUS SLASH MINUS EQUALS SINGLE
%token EQUALSEQUALS GREATEREQUALS LESSEQUALS GREATER LESS
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
       ;


stmt:   simple_stmt
    |   compound_stmt
    |   NEWLINE
    ;

simple_stmt:    single_stmt NEWLINE
           ;

mult_stmts: %empty
          | mult_stmts simple_stmt
          {
            num_stmts++;
          }
          | mult_stmts compound_stmt
          {
            num_stmts++;
          }
          | mult_stmts NEWLINE
          ;

single_stmt:    print_stmt
           |    assignment
           |    return_stmt
           |    value
           ;

compound_stmt:  branch_stmt
             |  func_def
             ;

ident_list: IDENTIFIER COMMA ident_list
          {
            node_ptr node = create_identifier_node(yyval.str);
            push_node_to_stack(node);
            ident_list_length++;
          }
          | IDENTIFIER
          {
            node_ptr node = create_identifier_node(yyval.str);
            push_node_to_stack(node);
            ident_list_length++;
          }
          ;

func_def:   DEF IDENTIFIER LPARENTH RPARENTH COLON block_stmt
        {
            node_ptr block = pop_node_from_stack();
            node_ptr func = create_func_def_node($2, block);
            push_node_to_stack(func);
        }
        |   DEF IDENTIFIER LPARENTH ident_list RPARENTH COLON block_stmt
        {
            node_ptr block = pop_node_from_stack();
            pop_parameters(ident_list_length, params);
            node_ptr func = create_func_def_node($2, ident_list_length,
                                                 params, block);

            push_node_to_stack(func);
            ident_list_length = 0;
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
           }
           ;

block_stmt: NEWLINE INDENT mult_stmts DEDENT
          {
            node_array children = alloc_children(num_stmts);
            for(int ii = 0; ii < num_stmts; ii++)
                children[num_stmts - ii - 1] = pop_node_from_stack();
            node_ptr node = create_block_stmt_node(num_stmts, children);
            push_node_to_stack(node);
            exit_block();
          }
          | NEWLINE INDENT error DEDENT
          {
            printf("Error in block stmt! Continuing...\n");
          }
          ;

print_stmt: PRINT LPARENTH value RPARENTH
          {
            node_array children = alloc_children(1);
            children[0] = pop_node_from_stack();

            node_ptr node = create_print_node(1, children);
            push_node_to_stack(node);
          }
          | PRINT LPARENTH RPARENTH
          {
            node_ptr node = create_print_node(0, 0);
            push_node_to_stack(node);           
          }
          ;

assignment: IDENTIFIER EQUALS value
          {
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
     ;

func_call:  IDENTIFIER LPARENTH RPARENTH
         {
            node_ptr node = create_func_call_node(yyval.str);
            push_node_to_stack(node);
         }
         |  IDENTIFIER LPARENTH func_params RPARENTH
         {
            node_ptr node = create_func_call_parameters_node(
                yyval.str, func_param_list_length);
            reset_func_param_list();
            push_node_to_stack(node);
         }
         ;

func_params:    func_param
           {
                inc_func_param_list();
           }
           |    func_param COMMA func_params
           {
                inc_func_param_list();
           }
           ;

func_param: expr1
          | STRING
          ;

return_stmt:    RETURN value
           {
                node_ptr value = pop_node_from_stack();
                node_ptr node = create_return_stmt(value);
                push_node_to_stack(node);
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
     | expr4 GREATEREQUALS expr4{
                                    push_operator(GREATEREQUALS);
                                }
     | expr4 LESSEQUALS expr4   {
                                    push_operator(LESSEQUALS);
                                }
     | expr4 GREATER expr4      {
                                    push_operator(GREATER);
                                }
     | expr4 LESS expr4         {
                                    push_operator(LESS);
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
     ;

expr6:  PLUS expr6
     |  MINUS expr6     {
                            mod = -1;
                            node_ptr node = pop_node_from_stack();
                            apply_negation_push();
                            mod = 1;
                        }
     |  func_call
     |  bool
     |  IDENTIFIER      {
                            node_ptr node = create_identifier_node(yyval.str);
                            push_node_to_stack(node);
                        }
     |  NUMBER          {
                            node_ptr node = create_number_node(yyval.num);
                            push_node_to_stack(node);
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
    | error
    {
        printf("Unexpected token %s! Continuing...\n", yyval);
    }
    ;
%%
