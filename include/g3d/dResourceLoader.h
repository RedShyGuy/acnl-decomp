#pragma once

#include "decomp.h"
#include "g3d/dResourceSet.h"

namespace g3d {
// RTTI N3g3d14ResourceLoaderE @ 0x008D0CD8
// vtable 0x00903A34 (vptr 0x00903A3C), offset_to_top 0, 2 entries
class ResourceLoader : public ::g3d::ResourceSet
{
public:
    virtual void vf_0x00(); // 0x00318014 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
    virtual void vf_0x04(); // 0x004EEC5C slot 0x04 | virtual slot, introduced by g3d::ResourceSet
    void LoadAsync(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int); // 0x00317C38 | libgarden [tier A]
    void InitializeCgfx(nw::os::IAllocator*, g3d::ResourceLoader*, bool); // 0x00317C70 | libgarden [tier A]
    void Read(unsigned long, unsigned long, unsigned long); // 0x004EB42C | libgarden [tier A]
    void Load(sead::SafeStringBase<char> const&, sead::Heap*, unsigned int); // 0x004EEBE4 | libgarden [tier A]
    ResourceLoader(); // 0x004EEC1C | libgarden [tier A]
};
} // namespace g3d
