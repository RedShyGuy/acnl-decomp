#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResource.h"

namespace sead {
// (newResource_ of the factory falls into it)
// 0x005448A8
DECOMP_NOINLINE sead::DirectResource::DirectResource() : Resource(), mData(nullptr), mSize(0), mBufferSize(0), mFlags(0)
{
}

// 0x005448FC slot 0x08
// 0x005448CC (deleting dtor)
sead::DirectResource::~DirectResource()
{
    if (mFlags & FLAG_NEED_DELETE) {
        delete[] mData;
    }
}

// 0x00544890 slot 0x14
void sead::DirectResource::doCreate_(u8*, u32, Heap*)
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below Resource
// 0x00829804 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Resource>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
