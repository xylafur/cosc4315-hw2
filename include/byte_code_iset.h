
#ifndef BYTE_CODE_ISET_H
#define BYTE_CODE_ISET_H

#include <cstdint>
#include <stack>
#include <string>


// bytecode instruction set
enum ByteCode_ISET : uint8_t {
  nop,     // no operation

  // arithmetic consumes two operands from operand stack
  addi,    // add integers
  subi,    // subtract integers
  muli,    // multiply integers
  divi,    // divide

  // boolean ands and or consumer two operands on operand stack
  bo_and,  // boolean and
  bo_or,   // boolean or

  // comparisons consume two operands on operand stack
  cmp_lt,  // less than comparison
  cmp_lte, // less than or equal comparison
  cmp_gt,  // greater than comparison
  cmp_gte, // greater than or equal comparison
  cmp_eq,  // equal comparison

  assn,   // assignment , consumes two operands on operand stack

  deref,  // DO NOT USE, dereference reference on top of operand stack

  print_i,  // output integer to std_out, consumes operand on operand stack

  print_so, // output string constant to std_out, consumes operand
            // on the stack assumed to be a constant reference

  print_si, // print string where address is in argument of instruction, 
            // does not consume operand on top of stack

  print_sp, // output space character to std_out


  o_push_const_ri, // push reference of integer literal in main program on top of operand stack
  o_push_const_rs, // push reference of string literal on top of operand stack
  o_push_const_rr, // push a reference to stack frame variables on operand stack

  o_pop,   // pop an operand from top of stack, same as discard

  o_popN,  // pop N operands off operand stack
  o_clear, // clear operand stack

  f_push,   // push a function stack, same as calling
            // will place return value of function on top of operand stack of 
            // the function / scope that called it

  f_return,     // DO NOT USE, return from a function

  f_return_item,// return an item from top of operand_stack

  branch_if,    // branch if item at top of operand stack is true
                //  item must be either an integer or a reference to an integer
                //  cannot be a string

  branch,       // jump to address

  exit_prog          // terminate program
};

enum Operand_type : uint8_t {
  integer_ot,
  string_ot,
  reference_ot
};

struct Operand {
  Operand(int64_t value) {
    this->o_type = Operand_type::integer_ot;
    this->value = value;
  }
  Operand(uint64_t reference) {
    this->o_type = Operand_type::reference_ot;
    this->reference = reference;
  }
  ~Operand() {}

  Operand_type o_type;

  union {
    int64_t value;
    uint64_t reference;
  };

  Operand& operator = (const Operand& other) {
    o_type = other.o_type;
    this->reference = other.reference;
    return *this;
  }
};


#endif

