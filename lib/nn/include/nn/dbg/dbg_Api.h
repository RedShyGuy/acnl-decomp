#pragma once

// nndbgPanic - stops the program (C interface; the name is from the binary). The callers pass
// no arguments that it uses.

#include "types.h"

extern "C" void nndbgPanic(); // 0x0013590C
