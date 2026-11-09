#pragma once

#include "decomp.h"
#include "script/dMailMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script7MailMgr18SingletonDisposer_E @ 0x008D3ACC
// vtable 0x0090A918 (vptr 0x0090A920), offset_to_top 0, 2 entries
//
// Destroys the mail manager when its heap is destroyed (in the first 16 bytes of the manager).
class script::MailMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    explicit SingletonDisposer_(sead::Heap* heap) : IDisposer(heap, HEAP_NULL_OPTION_CURRENT_HEAP) {}
    virtual ~SingletonDisposer_(); // 0x005F5758 slot 0x00 (name after sead)
    // 0x005F56F0 slot 0x04 (deleting dtor)
};
