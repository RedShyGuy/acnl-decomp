#include "Sv/dSvProc.h"

// 0x005CE220
// 0x005CE21C (deleting dtor)
SvProc::~SvProc()
{
    // empty (in the original too)
}

// the instance; the static initializer 0x007855E8 of its file builds it together with objects of the save code
// 0x0095D370
SvProc g_SvProc;
