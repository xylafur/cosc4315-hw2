
#include "bcode_generator.h"
using namespace std;

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





