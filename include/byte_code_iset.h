
#ifndef BYTE_CODE_ISET_H
#define BYTE_CODE_ISET_H

#include <cstdint>
#include <stack>
#include <string>

// bytecode instruction set
enum ByteCode_ISET : uint8_t {
  nop  = 0x00,   // no operation

  addi = 0x01,   // add on operand stack
  subi = 0x02,   // subtract on operand stack
  muli = 0x03,   // multiply on operand stack
  divi = 0x04,   // divide on operand stack
  bo_and = 0x05, // boolean and
  bo_or  = 0x06, // boolean or

  cmp_lt  = 0x07, // less than comparison
  cmp_lte = 0x08, // less than or equal comparison
  cmp_gt  = 0x09, // greater than comparison
  cmp_gte = 0x0a, // greater than or equal comparison
  cmp_eq  = 0x0b, // equal comparison
  cmp_neq = 0x0c, // not equal comparison

  assn  = 0x0d,   // assignment to function stack variable reference
  deref = 0x0e,   // dereference reference on top of operand stack

  print_i  = 0x0f,  // output integer from top of operand stack to std_out
  print_sc = 0x11, // output string constant to std_out
  print_sp = 0x12, // output space character to std_out

  o_push_const_ri = 0x13, // push reference of integer in main program on top of operand stack
  o_push_const_rs = 0x14, // push reference of string on top of operand stack
  o_push_const_rr = 0x15, // push a reference to stack frame variables on operand stack

  o_pop = 0x16,   // pop an operand from top of stack, same as discard

  o_popN  = 0x17,  // pop N operands off operand stack
  o_clear = 0x18, // clear operand stack

  f_push  = 0x19,  // push a function stack, same as calling
                   // will place return value of function on top of operand stack of 
                   // the function / scope that called it

  f_return = 0x1a,     // return from a function
  f_return_item = 0x1b, // return an item from top of operand_stack

  branch_if_f = 0x1c, // branch if top of operand stack is true -> go forwards
  branch_if_b = 0x1d, // branch if top of operand stack is true -> go backwards
  branch_f = 0x1c,    // jump forward
  branch_b = 0x1d,    // branch backward

  exit_prog = 0xff  // terminate program
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

