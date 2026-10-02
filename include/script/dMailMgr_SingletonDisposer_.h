#pragma once

#include "decomp.h"
#include "script/dMailMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script7MailMgr18SingletonDisposer_E @ 0x008D3ACC
// vtable 0x0090A918 (vptr 0x0090A920), offset_to_top 0, 2 entries
class script::MailMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x005F5758 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005F56F0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
