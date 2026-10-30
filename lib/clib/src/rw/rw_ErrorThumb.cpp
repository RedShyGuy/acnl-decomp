// The error reporting of the C++ library (Thumb code in the original). See rw/rw_String.h.

#include "decomp.h"
#include "rw/rw_String.h"
#include "clib/clib_Thumb.h"

// (calls the handler the program set; for ids above 3 it formats the message into a __rw_exception first)
// 0x0030757C (name is ours)
void __rw::ThrowError(int id, ...)
{
}
