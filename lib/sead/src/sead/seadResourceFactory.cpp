#include "sead/seadTListNode.h"
#include "sead/seadIDisposer.h"
#include "sead/seadResourceFactory.h"

namespace sead {
// ctor address unknown
sead::ResourceFactory::ResourceFactory()
{
}

// 0x00546590 slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::ResourceFactory::~ResourceFactory()
{
}

// 0x0074C714 slot 0x08 | virtual slot, introduced by sead::ResourceFactory
void sead::ResourceFactory::vf_0x08()
{
}

// 0x0074C6C8 slot 0x0C | virtual slot, introduced by sead::ResourceFactory
void sead::ResourceFactory::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void sead::ResourceFactory::vf_0x10()
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void sead::ResourceFactory::vf_0x14()
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void sead::ResourceFactory::vf_0x18()
{
}

} // namespace sead
