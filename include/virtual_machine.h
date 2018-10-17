
#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <assert.h>
#include <stack>
#include <vector>
#include <iostream>
#include "byte_code_iset.h"

int run(uint64_t *program);

#endif
