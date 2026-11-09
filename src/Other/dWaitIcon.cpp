#include "Other/dWaitIcon.h"

// 0x006C9CA4 slot 0x00
// 0x006C9C64 (deleting dtor)
WaitIcon::~WaitIcon()
{
    // (only the members are destroyed)
}

// 0x0082D470 slot 0x08
const char* WaitIcon::GetName(Func func) const
{
    return FindName(func);
}
