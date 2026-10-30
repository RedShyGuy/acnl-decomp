// abort of the C library: the system stops the program (nndbgPanic)

#include "nn/dbg/dbg_Api.h"

// (falls into nndbgPanic)
// 0x00135908 | nintendogs:callgraph [tier C]
extern "C" void abort()
{
    nndbgPanic();
}
