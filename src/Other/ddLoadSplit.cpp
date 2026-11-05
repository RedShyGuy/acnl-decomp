#include "Other/ddLoadSplit.h"

// 0x0013CA04 slot 0x00 (name after the RTTI)
// 0x001BA034 (deleting dtor)
dLoadSplit::~dLoadSplit()
{
    // (only the base is destroyed)
}
