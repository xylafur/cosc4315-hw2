
#include "bcode_generator.h"
using namespace std;

#define MAX_PROGRAM_LEN (1024*1024)

struct block {
    unsigned int num_nodes;
    uint64_t * bytecode;
};


//I felt like having a function for each of the inst creation types would make
//things cleaner down below
uint64_t create_ident_inst(char * name)
{
    return 0;
}

uint64_t create_expr_inst(int oper)
{
    switch(oper){
    case SINGLE:
        break
    case PLUS:
        break;
    }
}

uint64_t create_string_inst(int oper)
{
    return 0;
}

#define alloc_program() (uint64_t*)malloc(MAX_PROGRAM_LEN * sizeof(uint64_t));
#define create(type, arg) program[pc++] = create_type_inst(arg); \
                          block_size_stack.top()++

void generate(node_ptr root){
    //the node stack keep strack of the block stmt nodes
    stack<node_array> node_stack;
    //the block size stack keeps track of the # of instructions per block
    stack<unsigned int> block_size_stack;
    //the actual stack of blocks
    stack<block> block_stack;
    block_stack.push(block);

    uint64_t * program = alloc_program();
    unsigned int pc = 0;
   
    node_ptr current = root;
    node_stack.push(current);
    block_size_stack.push(0);


    while (node_stack.size()) {
        current = node_stack.top();
        if (current->num_children == current->curr_child ) { 
            switch(current->type){
            //we can handle most things just on the way up, we can just write
            //them directly to the program
            case IDENTIFIER_NODE:
                create(ident, current->value.s_value);
                break;
            case EXPR_NODE:
                create(expr, current->i_operator);
                break;
            case STRING_NODE:
                create(string, current->value.s_value);
                break;
            case NUMBER_NODE:
                break;


            //we just need to store the program that is for this block and then
            //wipe the program for the parent
            case BLOCK_STMT_NODE:
                block_stack.top().bytecode = program;
                program = alloc_program();
                break;
            }
            node_stack.pop();
        //else is the case where this is not the right most child of a node
        }else{
            switch(current->type){
            //if we enter a block stmt we have to keep track of this blocks isnt
            case BLOCK_STMT_NODE:
                block_size_stack.push(0);
                block_stack.push(block);
                break;
            }
            node_stack.push(current->children[current->curr_child++]);
        }
}

/*
void generate(ParseTreeNode *root, vector<literal> constants) {
  // need a datastructure to keep track of constants
  vector<Instruction> main_program;

  stack<Block> block_stack;
  Block block;
  stack<map<string, uint8_t>> scope_stack;
  string temp_str;


  stack<ParseTreeNode**> node_stack;
  ParseTreeNode **current = &root, **dptr, *temp=nullptr;
  node_stack.push(current);
  size_t curr_child, num_children;

  size_t block_depth = 0;

  vector<FunctionDef> functions;

  Instruction ins;

  #define TAPE_REF() ((block_depth) ? block_stack.top().instructions : main_program)
  #define ADDSTMT(I) TAPE_REF().push_back(I)
  #define ADDSTMTS(P,V) P.insert(P.end(), V.begin(), V.end())

  bool ident_ref_must_exist = false;
  bool conditional_if_no_else_block = false;
  bool conditional_if_else_block = false;

  while (node_stack.size()) {
    current = node_stack.top(); //node_stack.pop();
    temp = *current;
    if ( temp->curr_child == temp->num_children ) { // processed all children or leaf node
      switch (temp->type) {
        case program_node:
          break;
        case block_stmt_node:
          block_depth--;
          if (conditional_if_else_block) { // TODO
            block = block_stack.top(); block_stack.pop();
          } else if (conditional_if_no_else_block) {
            block = block_stack.top(); block_stack.pop();
            ADDSTMT(Instruction(ByteCode_ISET::branch_if_f, 1));
            ADDSTMT(Instruction(ByteCode_ISET::branch_f, block.instructions.size()));
            ADDSTMTS(TAPE_REF(), block.instructions);
            conditional_if_no_else_block = false;
          }
          break;
        case func_def_node: //TODO
          break;
        case branch_stmt_no_else_node:
          break;
        case branch_stmt_else_node:
          break;
        case print_stmt_node:
          ADDSTMT(Instruction(ByteCode_ISET::print_sc, ));// TODO
          break;
        case assignment_node:
          ADDSTMT(Instruction(ByteCode_ISET::assn));// TODO
          break;
        case func_call_node:
          ident_ref_must_exist = false;

          break;
        case return_stmt_node: // DONE
          ADDSTMT(Instruction(ByteCode_ISET::f_return_item));
          break;
        case expr_node: // DONE
          switch (temp->i_operator) {
            case STAR:
              ADDSTMT(Instruction(ByteCode_ISET::muli));
              break;
            case PLUS:
              ADDSTMT(Instruction(ByteCode_ISET::addi));
              break;
            case SLASH:
              ADDSTMT(Instruction(ByteCode_ISET::divi));
              break;
            case MINUS:
              ADDSTMT(Instruction(ByteCode_ISET::subi));
              break;
            case EQUALSEQUALS:
              ADDSTMT(Instruction(ByteCode_ISET::cmp_eq));
              break;
            case GREATEREQUALS:
              ADDSTMT(Instruction(ByteCode_ISET::cmp_gte));
              break;
            case LESSEQUALS:
              ADDSTMT(Instruction(ByteCode_ISET::cmp_lte));
              break;
            case GREATER:
              ADDSTMT(Instruction(ByteCode_ISET::cmp_gt));
              break;
            case LESS:
              ADDSTMT(Instruction(ByteCode_ISET::cmp_lt));
              break;
          }
          ident_ref_must_exist = false;
          break;
        case string_node: // relatively done, TODO update string instruction to print from operand stack ref
          // TODO 
          ADDSTMT(Instruction(
                ByteCode_ISET::o_push_const_rs,
                get_string_index(temp->value.s_value)));
          break;
        case number_node: // DONE
          // TODO
          ADDSTMT(Instruction(
                ByteCode_ISET::o_push_const_ri,
                get_int_index(temp->value.n_value)));
          break;
        case identifier_node: // DONE
          temp_str = std::string(temp->value.s_value)
          if (ident_ref_must_exist) {
            // TODO check that the identifier exists in the scope
            // if not throw a fit
            if (scope_stack.top().find(temp_str) == m.end()) {
              cerr << "ERROR local variable referenced before assignment: " << temp << endl;
              throw std::exception();
            }
          } else {
            scope_stack.top().insert(pair<string,uint8_t>(temp, scope_stack.top().size()));
          }
          ADDSTMT(Instruction(ByteCode_ISET::o_push_const_rr, scope_stack.top()[temp]));
          break;
      }
      node_stack.pop();
    } else { // push a child and move on
      dptr = &(temp->children[temp->curr_child++]);
      node_stack.push(dptr);
      switch (temp->type) { // what to do before going in
        case program_node: break;
        case block_stmt_node:
          block_stack.push(Block()); block_depth++;
          break;
        case func_def_node:
            // push scope stack here for a new scope
            break;
        case branch_stmt_no_else_node:
            conditional_if_no_else_block = true;
            break;
        case branch_stmt_else_node:
            conditional_if_else_block = true;
            break;
        case print_stmt_node: break;
        case assignment_node: break;
        case func_call_node:
            ident_ref_must_exist = true;
        case return_stmt_node: break;
        case expr_node:
            ident_ref_must_exist = true;
            break;
        case string_node:
            break;
        case number_node:
            break;
        case identifier_node: // o_push_rr
            main_program.top().push_back(bytecode_iset::o_push_const_rr);
            break;
      }
    }
  }


  
}
*/
