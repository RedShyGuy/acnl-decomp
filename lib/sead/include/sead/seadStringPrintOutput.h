#pragma once

#include "decomp.h"
#include "sead/seadPrintOutput.h"

namespace sead {
// RTTI N4sead17StringPrintOutputE @ 0x008D1C14
// vtable 0x0090606C (vptr 0x00906074), offset_to_top 0, 3 entries
class StringPrintOutput : public ::sead::PrintOutput
{
public:
    StringPrintOutput(); // ctor candidate(s) 0x0054B12C (unverified)
    virtual void vf_0x00(); // 0x00548F88 slot 0x00 | virtual slot, introduced by sead::StringPrintOutput
    virtual void vf_0x04(); // 0x00548F84 slot 0x04 | virtual slot, introduced by sead::StringPrintOutput
    virtual void vf_0x08(); // 0x00548E5C slot 0x08 | virtual slot, introduced by sead::StringPrintOutput
};
} // namespace sead
