#pragma once

#include "decomp.h"
#include "graphic/dCurrentSetter.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace ssys {
namespace ma {
class VramHeap;
} // namespace ma
} // namespace ssys

namespace g3d {
// a graphics file (CGFX) in a buffer, set up in VRAM heaps of its own (names are ours)
// RTTI N3g3d11ResourceSetE @ 0x008D0C7C
// vtable 0x00903944 (vptr 0x0090394C), offset_to_top 0, 2 entries
class ResourceSet
{
public:
    // (inline)
    ResourceSet() : m_pBuffer(NULL), m_GraphicsFile(NULL), m_SetupResult(SETUP_RESULT_NONE)
    {
        m_pHeaps[0] = NULL;
        m_pHeaps[1] = NULL;
        m_pHeaps[2] = NULL;
    }
    // cleans the file up
    virtual ~ResourceSet(); // 0x004EB764 slot 0x00
    // 0x004EB6F4 (deleting dtor)

    // the heaps of the MemoryMgr while the file is set up or cleaned up (NULL: the current one)
    void SetHeaps(ssys::ma::VramHeap* heap0, ssys::ma::VramHeap* heap1, ssys::ma::VramHeap* heap2); // 0x004EB420

    // the most member animations of the skeletal animations of the file (name is ours)
    u32 GetMaxSkeletalAnimMemberCount() const; // 0x004EB4FC
    // (inline) the file cleaned up and forgotten (nothing without a buffer)
    void Cleanup();
    // (inline)
    nw::gfx::res::ResGraphicsFile GetGraphicsFile() const { return m_GraphicsFile; }

    // m_SetupResult before the setup
    static const u32 SETUP_RESULT_NONE = 0xFFFFFFFF;

    void* m_pBuffer;                                   // 0x04 the loaded file
    nw::gfx::res::ResGraphicsFile m_GraphicsFile;      // 0x08
    u32 m_SetupResult;                                 // 0x0C of ResGraphicsFile::Setup
    ssys::ma::VramHeap* m_pHeaps[3];                   // 0x10
};
ASSERT_SIZE(ResourceSet, 0x1C);

inline void g3d::ResourceSet::Cleanup()
{
    if (m_pBuffer == NULL) {
        return;
    }
    graphic::CurrentSetter setter(m_pHeaps[2], m_pHeaps[0], m_pHeaps[1]);
    if (m_GraphicsFile.IsValid()) {
        GetGraphicsFile().Cleanup();
    }
    m_SetupResult = SETUP_RESULT_NONE;
    m_pBuffer = NULL;
}
} // namespace g3d
