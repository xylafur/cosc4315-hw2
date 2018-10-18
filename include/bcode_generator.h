#ifndef BCODE_GEN_H
#define BCODE_GEN_H

#include "ast.h"
#include "byte_code_iset.h"
#include <string>
#include <stack>
#include <vector>
#include <map>

enum arg_type {
  narg,
  ident_ref,
  const_arg_t,
};

struct Instruction {
  uint8_t code = 0;
  uint64_t data = 0; // local data
  Instruction(uint8_t code, uint64_t data = 0) {
    this->code = code; this->data = data;
  }

  uint64_t to_code() {
    auto ret = data;
    ret = (ret << 8) | code;
    return ret;
  }
};

struct Block {
  //map<string, uint8_t> scope;
  vector<Instruction> instructions;
  Block() {
    instructions = std::move(vector<Instruction>(ins));
  }
};

struct FunctionDef {
  map<string, uint8_t> scope;
  vector<Instruction> instructions;
};

void generate(node_ptr root);
void process_func_decl();
vector<uint64_t> process_block_stmts();

#endif
