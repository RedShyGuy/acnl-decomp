#include "sead/seadMethodTreeMgr.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x005431C4 (name is ours)
DECOMP_NOINLINE sead::MethodTreeMgr::MethodTreeMgr() : mCriticalSection()
{
}

// 0x00543200 slot 0x08
// 0x005431E0 (deleting dtor)
sead::MethodTreeMgr::~MethodTreeMgr()
{
    // (only the members are destroyed)
}
} // namespace sead
