#!/usr/bin/env python3

ByteCode_ISET = {
   'nop'  : 0x00,
   'addi' : 0x01,
   'subi' : 0x02,
   'muli' : 0x03,
   'divi' : 0x04,
   'bo_and' : 0x05,
   'bo_or'  : 0x06,
   'cmp_lt'  : 0x07,
   'cmp_lte' : 0x08,
   'cmp_gt'  : 0x09,
   'cmp_gte' : 0x0a,
   'cmp_eq'  : 0x0b,
   'cmp_neq' : 0x0c,
   'assn'  : 0x0d,
   'deref' : 0x0e,
   'print_i'  : 0x0f,
   'print_sc' : 0x11,
   'print_sp' : 0x12,
   'o_push_const_ri' : 0x13,
   'o_push_const_rs' : 0x14,
   'o_push_const_rr' : 0x15,
   'o_pop' : 0x16,
   'o_popN'  : 0x17,
   'o_clear' : 0x18,
   'f_push'  : 0x19,
   'f_return' : 0x1a,
   'f_return_item' : 0x1b,
   'branch_if_f' : 0x1c,
   'branch_if_b' : 0x1d,
   'branch_f' : 0x1c,
   'branch_b' : 0x1d,
   'exit_prog' : 0xff
};

byte2binstr = lambda i:(lambda n:'0'*(8-len(n)) + n)(bin(i)[2:])
instruction_fmt = '{{:>{}}}   0x{{:02X}}'.format(max(len(k) for k in ByteCode_ISET)+1)

import sys
from math import log, ceil
log16 = lambda n: str(ceil(log(n) / log(16.0)) + 1)

program = []

help_txt =\
"""======================================
(M) print program
(R) replace line - 'R:#,0 0 0 0 0 0 0 0'
(W) write ins   - 'W:0 0 0 0 0 0 0 0'
(S) write string   - 'S:"asdf..."'
(I) print codes
(anything else) write to file and quit
======================================
"""

while True:
   user_in = input(help_txt)
   print(' user_input -> ', user_in)
   user_in = user_in.strip()
   if not(user_in) or user_in[0] == '#': continue

   user_in = user_in.split('#')[0].split(':')

   action = user_in[0]
   if len(user_in) > 1:
      data = user_in[1]

   if action == 'M':
      print('Your program: ')
      if len(program):
         fmt = '0x{{:0{}X}}    '*2
         fmt = fmt.format(log16(len(program)), log16(len(program)*8))
         fmt += '   {}'
         for i, p in enumerate(program):
            p = ' '.join(['{:02X}'.format(pp) for pp in p][::-1])
            print(fmt.format(i, i*8, p))

   elif action == 'R':
      data = data.split(',')
      if len(data) != 2:
         print('WRONG INPUT')
         continue

      index, line = data
      index = int(index)
      line = line.split()
      if not(0 <= index < len(program)):
         print(' index out of range! ', index)
         continue

      if len(line) != 8:
         print('ERROR LINE')
         continue

      line = [int(i) for i in line]
      program[index] = line[::-1]

   elif action == 'W':
      line = data.split()
      if len(line) != 8:
         print('ERROR LINE')
         continue
      line = [int(i) for i in line]
      print('You entered: ', line)
      program.append(line[::-1])

   elif action == 'S':
      line = [ord(i) for i in eval(data)]
      line += [0]*(8 - (len(data) % 8))

      print('You entered: ', line)
      for i in range(len(line) // 8):
         l = line[i*8:(i+1)*8]
         print(l)
         program.append(l)

   elif action == 'I':
      print('\n'.join(instruction_fmt.format(k, ByteCode_ISET[k]) for k in ByteCode_ISET))
   else:
      if action == 'Q': exit()

      with open(action + '.bin', 'wb') as f:
         for p in program:
            f.write(bytearray(p))
      break

