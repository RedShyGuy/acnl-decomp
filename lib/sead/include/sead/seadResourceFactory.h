#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadTListNode.h"

namespace sead {
// RTTI N4sead15ResourceFactoryE @ 0x008D18B0
// vtable 0x00905B34 (vptr 0x00905B3C), offset_to_top 0, 7 entries
class ResourceFactory : public ::sead::TListNode<sead::ResourceFactory*>, public ::sead::IDisposer
{
public:
    ResourceFactory(); // ctor address unknown
    virtual ~ResourceFactory(); // 0x00546590 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0054652C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074C714 slot 0x08 | virtual slot, introduced by sead::ResourceFactory
    virtual void vf_0x0C(); // 0x0074C6C8 slot 0x0C | virtual slot, introduced by sead::ResourceFactory
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
