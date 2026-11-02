#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceAccessor.h"

namespace ssys {
namespace ma {
namespace lyt {
// the access to the resources of layouts (no vtable of its own in the binary: only built as a base; names after
// libgarden except GetResource)
// RTTI N4ssys2ma3lyt15ResAccInterfaceE @ 0x008D247C
class ResAccInterface
{
public:
    // (inline)
    explicit ResAccInterface(nw::lyt::ResourceAccessor* accessor) : m_pResourceAccessor(accessor), m_pData(NULL) {}
    virtual ~ResAccInterface() {}

    nw::lyt::ResourceAccessor* GetResourceAccessor() const { return m_pResourceAccessor; }
    // (name is ours)
    DECOMP_EMIT_COPY void* GetResource(u32 type, const char* name) const // 0x00568A08
    {
        return m_pResourceAccessor->GetResource(type, name, NULL);
    }

    nw::lyt::ResourceAccessor* m_pResourceAccessor; // 0x4
    const void* m_pData;                            // 0x8 the archive
};
ASSERT_SIZE(ResAccInterface, 0xC);
} // namespace lyt
} // namespace ma
} // namespace ssys
