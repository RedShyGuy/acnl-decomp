#pragma once

#include "decomp.h"
#include "sead/seadHandleBase.h"

namespace sead {
// RTTI N4sead10FileHandleE @ 0x008D1384
// vtable 0x00904EB4 (vptr 0x00904EBC), offset_to_top 0, 2 entries
class FileHandle : public ::sead::HandleBase
{
public:
    FileHandle(); // ctor candidate(s) 0x00133A88, 0x0013C9D8 (unverified)
    virtual void vf_0x00(); // 0x0053F1B4 slot 0x00 | virtual slot, introduced by sead::FileHandle
    virtual void vf_0x04(); // 0x0053F184 slot 0x04 | virtual slot, introduced by sead::FileHandle
};
} // namespace sead
