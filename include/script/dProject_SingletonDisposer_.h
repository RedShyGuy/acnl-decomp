#pragma once

#include "decomp.h"
#include "script/dProject.h"
#include "sead/seadIDisposer.h"

// RTTI N6script7Project18SingletonDisposer_E @ 0x008D3AE4
// vtable 0x0090A950 (vptr 0x0090A958), offset_to_top 0, 2 entries
class script::Project::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x005F7DC0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005F7D5C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
