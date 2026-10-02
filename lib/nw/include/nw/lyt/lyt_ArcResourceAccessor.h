#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceAccessor.h"

namespace nw {
namespace lyt {
// RTTI N2nw3lyt19ArcResourceAccessorE @ 0x008D085C
// vtable 0x00902B74 (vptr 0x00902B7C), offset_to_top 0, 7 entries
class ArcResourceAccessor : public ::nw::lyt::ResourceAccessor
{
public:
    ArcResourceAccessor(); // ctor candidate(s) 0x0012BF18 (unverified)
    virtual ~ArcResourceAccessor(); // 0x004B6D04 slot 0x00 | slot vf_0x00 of nw::lyt::ResourceAccessor
    // 0x004B6CD8 slot 0x04 | slot vf_0x04 of nw::lyt::ResourceAccessor (deleting dtor)
    virtual void vf_0x08(); // 0x004B6BCC slot 0x08 | virtual slot, introduced by nw::lyt::ResourceAccessor
    virtual void vf_0x0C(); // 0x004B6C84 slot 0x0C | virtual slot, introduced by nw::lyt::ResourceAccessor
    virtual void GetTexture(char const*); // 0x004B6B64 slot 0x10 | libgarden
    void RegistFont(char const*, nw::font::Font const*); // 0x004B52F4 | libgarden [tier A]
};
} // namespace lyt
} // namespace nw
