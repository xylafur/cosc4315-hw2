#!/usr/bin/env python3

ByteCode_ISET = [
  'nop',
  'addi',
  'subi',
  'muli',
  'divi',

  'bo_and',
  'bo_or',

  'cmp_lt',
  'cmp_lte',
  'cmp_gt',
  'cmp_gte',
  'cmp_eq',

  'assn',
  'deref',

  'print_i',
  'print_ir',
  'print_sc',
  'print_sp',

  'o_push_const_ri',
  'o_push_const_rs',
  'o_push_const_rr',

  'o_pop',

  'o_popN',
  'o_clear',

  'f_push',

  'f_return',

  'f_return_item',

  'branch_if',

  'branch',
  'exit_prog'
]

byte2binstr = lambda i:(lambda n:'0'*(8-len(n)) + n)(bin(i)[2:])
ByteCode_ISET = {b:i for i,b in enumerate(ByteCode_ISET)}

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
   user_in = input(help_txt).strip()
   if not(user_in) or user_in[0] == '#': continue

   user_in = user_in.split('#')[0].split(':')

   action = user_in[0]
   if len(user_in) > 1:
      data = user_in[1]

   if action == 'M':
      print('Your program: ')
      if len(program):
         fmt = '0x{:0' + log16(len(program)*8) + 'X}        {}'
         for i, p in enumerate(program):
            p = ' '.join(['{:02X}'.format(pp) for pp in p][::-1])
            print(fmt.format(i*8, p))

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
      print('\n'.join('{} : {}'.format(k, ByteCode_ISET[k]) for k in ByteCode_ISET))
   else: break

with open('bcode.bin', 'wb') as f:
   for p in program:
      f.write(bytearray(p))

