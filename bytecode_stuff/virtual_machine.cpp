
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
  int64_t addr_jump; // for address jumping
  uint64_t oref; // reference argument
  char * str; // for printing strings
  Operand temp(a); // temp operand

  #define VAL() (operand_stack.top().o_type) ?\
                operand_stack.top().value :\
                function_stack_variables[operand_stack.top().reference];\
                operand_stack.pop();\
                operand_stack_scope.top()--

  #define REF() operand_stack.top().reference;\
                operand_stack.pop();\
                operand_stack_scope.top()--

  #define VAB() b = VAL(); a = VAL()
  #define ARB() b = VAL(); oref = REF()

  #define ABO(O) VAB(); temp = Operand((int64_t)(a O b));\
                        operand_stack.push(temp);\
                        operand_stack_scope.top()++

  #define INS_ADDR_MASK 0x00ffffffffffffff
  while (pc < length) {
    ins = main_program[pc++];
    bcode = ins & 0xff;
    addr_jump = ins;
    addr_jump >>= 8;
    oref = ins >> 8;

    switch (bcode) {
      case ByteCode_ISET::nop:
        break;
      case ByteCode_ISET::exit_prog:
        return addr_jump;

      case ByteCode_ISET::f_return_item:
        assert((a = operand_stack_scope.top()) > 0);
        temp = operand_stack.top();
        for (; a > 0; a--) operand_stack.pop(); operand_stack_scope.pop();
        operand_stack.push(temp); operand_stack_scope.top()++;
        pc = function_stack.top(); function_stack.pop();
        function_stack_variables.pop();
        break;

      case ByteCode_ISET::f_push: // call a function
        function_stack.push(pc); // save current pc on stack for later
        pc = oref >> 8;   // first 6 bytes to program counter
        a  = oref & 0xff; // 7th byte = number of stack frame variables
        function_stack_variables.push(vector<int64_t>(a,0));
          // push empty vector for stack frame variables
        operand_stack_scope.push(0); // push a new operand stack scope
        break;

      case ByteCode_ISET::branch_if:
        a = VAL(); if (a) pc += addr_jump; break;
      case ByteCode_ISET::branch_nif:
        a = VAL(); if (!a) pc += addr_jump; break;
      case ByteCode_ISET::branch:
        pc += addr_jump; break;
 
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

      case ByteCode_ISET::print_si: // print string from instruction addr
        str = ((char*)(main_program)) + ((char)(oref));
        cout << str << endl;
        break;

      case ByteCode_ISET::print_so: // print string from operand stack
        oref = REF();
        str = ((char*)(main_program)) + ((char)(oref));
        cout << str << endl;
        break;

      case ByteCode_ISET::print_sp:
        cout << " "; break;

      case ByteCode_ISET::assn:
        assert(operand_stack_scope.top() >= 2);
        if ((temp = operand_stack.top()).o_type == reference_ot) {
          a = function_stack_variables.top()[temp.reference];
          operand_stack.top() = Operand(a);
        }
        ARB();
        function_stack_variables.top()[oref] = b;
        break;

      case ByteCode_ISET::deref:  // made useless by assn instruction dereference check
        assert(operand_stack_scope.top() >= 1);
        assert((temp = operand_stack.top()).o_type == reference_ot);
        a = function_stack_variables.top()[temp.reference];
        operand_stack.top() = Operand(a);
        break;

      case ByteCode_ISET::o_push_const_ri:
        a = (int64_t)(main_program[oref]);
        temp = Operand(a);
        operand_stack.push(temp);
        operand_stack_scope.top()++;
        break;

      case ByteCode_ISET::o_push_const_rr:
        temp = Operand(oref);
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

   }
  }

  return 0;
}

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

