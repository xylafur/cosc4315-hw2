
#include "virtual_machine.h"
using namespace std;

int run(uint64_t *main_program, size_t length) {
  stack<Operand> operand_stack;

  stack<uint64_t> operand_stack_scope;
  operand_stack_scope.push(0);

  stack<uint64_t> function_stack;
  stack<vector<int64_t>> function_stack_variables;


  uint64_t ins, pc=0;
  uint8_t bcode;

  int64_t a, b; // temp registers a and b
  uint64_t ref; // temp registers a and b
  Operand temp(a); // temp operand

  #define VAL() operand_stack.top().value;\
                operand_stack.pop();\
                operand_stack_scope.top()--

  #define REF() operand_stack.top().reference;\
                operand_stack.pop();\
                operand_stack_scope.top()--

  #define VAB() b = VAL(); a = VAL()
  #define ARB() b = VAL(); ref = REF()

  #define ABO(O) VAB(); temp = Operand((int64_t)(a O b));\
                        operand_stack.push(temp);\
                        operand_stack_scope.top()++

  while (pc < length) {
    ins = main_program[pc++];
    bcode = ins >> 56;

    switch (bcode) {
      case ByteCode_ISET::nop:
        break;
      case ByteCode_ISET::exit_prog:
        return (ins & INS_ADDR_MASK);

      case ByteCode_ISET::addi:
        assert(operand_stack_scope.top() >= 2);
        ABO(+); break;
      case ByteCode_ISET::subi:
        assert(operand_stack_scope.top() >= 2);
        ABO(-); break;
      case ByteCode_ISET::muli:
        assert(operand_stack_scope.top() >= 2);
        ABO(*); break;
      case ByteCode_ISET::divi:
        assert(operand_stack_scope.top() >= 2);
        ABO(/); break;

      case ByteCode_ISET::bo_and:
        assert(operand_stack_scope.top() >= 2);
        ABO(&&); break;
      case ByteCode_ISET::bo_or:
        assert(operand_stack_scope.top() >= 2);
        ABO(||); break;
      case ByteCode_ISET::cmp_lt:
        assert(operand_stack_scope.top() >= 2);
        ABO(<); break;
      case ByteCode_ISET::cmp_lte:
        assert(operand_stack_scope.top() >= 2);
        ABO(<=); break;
      case ByteCode_ISET::cmp_gt:
        assert(operand_stack_scope.top() >= 2);
        ABO(>); break;
      case ByteCode_ISET::cmp_gte:
        assert(operand_stack_scope.top() >= 2);
        ABO(>=); break;
      case ByteCode_ISET::cmp_eq:
        assert(operand_stack_scope.top() >= 2);
        ABO(==); break;

      case ByteCode_ISET::print_i: // output integer on top of operand stack
        assert(operand_stack_scope.top() >= 1);
        a = VAL();
        cout << a;
        break;

      case ByteCode_ISET::print_sc: // output string constant
        cout << (((char*)(main_program)) + (ins & INS_ADDR_MASK)); break;
      case ByteCode_ISET::print_sp:
        cout << " "; break;

      case ByteCode_ISET::assn:
        assert(operand_stack_scope.top() >= 2);
        ARB();
        function_stack_variables.top()[ref] = b;
        break;

      case ByteCode_ISET::deref:
        assert(operand_stack_scope.top() >= 1);
        assert((temp = operand_stack.top()).o_type == reference_ot);
        a = function_stack_variables.top()[temp.reference];
        operand_stack.top() = Operand(a);
        break;

      case ByteCode_ISET::o_push_const_ri:
        ref = (ins & INS_ADDR_MASK) >> 3;
        a = (int64_t)(main_program[ref]);
        temp = Operand(a);
        operand_stack.push(temp);
        operand_stack_scope.top()++;
        break;

      case ByteCode_ISET::o_push_const_rr:
        ref = (ins & INS_ADDR_MASK);
        temp = Operand(ref);
        operand_stack.push(temp);
        operand_stack_scope.top()++;
        break;

      case ByteCode_ISET::o_clear:
        assert(a = operand_stack_scope.top());
        for (; a > 0; a--) operand_stack.pop();
        operand_stack_scope.top() = 0;
        break;

      case ByteCode_ISET::o_popN:
        a = (ins & INS_ADDR_MASK);
        assert(a >= 0 && a <= operand_stack_scope.top());
        for (b = a; b > 0; b--) operand_stack.pop();
        operand_stack_scope.top() -= a;
        break;

      case ByteCode_ISET::o_pop:
        assert(operand_stack_scope.top());
        operand_stack_scope.top()--;
        operand_stack.pop();
        break;

      case ByteCode_ISET::f_return_item:
        assert((a = operand_stack_scope.top()) > 0);
        temp = operand_stack.top();
        for (; a > 0; a--) operand_stack.pop(); operand_stack_scope.pop();
        operand_stack.push(temp); operand_stack_scope.top()++;
        pc = function_stack.top(); function_stack.pop();
        function_stack_variables.pop();
        break;

      case ByteCode_ISET::f_push:
        function_stack.push(pc);
        pc = (ins & (INS_ADDR_MASK >> 8)) >> 3;
        a  = (ins & INS_ADDR_MASK) >> 48;
        function_stack_variables.push(vector<int64_t>(a,0));
        operand_stack_scope.push(0);
        break;

      case ByteCode_ISET::branch_if:
        a = VAL(); if (!a) break;
      case ByteCode_ISET::branch:
        pc = (ins & INS_ADDR_MASK) >> 3; break;
    }
  }

  return 0;
}


/*
int main(int argc, char **argv) {
  //open file
  fstream infile(argv[1], ios::in | ios::binary);

  //get length of file
  infile.seekg(0, infile.end);
  size_t length = infile.tellg();
  infile.seekg(0, infile.beg);
  length = length - infile.tellg();

  //cout << "DEBUG LENGTH:" << length << endl;

  uint64_t *main_program = new uint64_t[(length / 8) + 1];

  char *buffer = (char *)(main_program);
  memset(buffer, (length / 8 + 1)*8, 0);

  //read file
  infile.read(buffer, length);
  infile.close();

  int i = run(main_program, (length / 8) + 1);

  return i;
}
*/
