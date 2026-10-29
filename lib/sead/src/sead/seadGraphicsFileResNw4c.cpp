#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResource.h"
#include "sead/seadGraphicsFileResNw4c.h"

namespace sead {
// (newResource_ of the factory falls into it)
// 0x0054A180
DECOMP_NOINLINE sead::GraphicsFileResNw4c::GraphicsFileResNw4c() : DirectResource(), mResFile()
{
}

// 0x0054A1DC slot 0x08
// 0x0054A1A8 (deleting dtor)
sead::GraphicsFileResNw4c::~GraphicsFileResNw4c()
{
    if (mResFile.IsValid()) {
        mResFile.Cleanup();
    }
}

// 0x0054A164 slot 0x14
void sead::GraphicsFileResNw4c::doCreate_(u8* data, u32, Heap*)
{
    mResFile = nw::gfx::res::ResGraphicsFile(data);
}
} // namespace sead

// the type information of the classes below DirectResource
// 0x00828E84 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DirectResource>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
