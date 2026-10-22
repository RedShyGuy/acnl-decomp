#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResModel.h"

namespace nw {
namespace gfx {
namespace res {
// A mesh of a model: a shape drawn with a material (only the fields known so far; names are ours).
// Setup builds the commands that set up the vertex attributes for the shader of the material.
struct ResMeshData
{
    enum Flag
    {
        FLAG_SETUP_DONE = 0x8,
    };

    u32 typeInfo;                  // 0x00
    u32 m_Unknown0x4[5];           // 0x04
    s32 shapeIndex;                // 0x18
    s32 materialIndex;             // 0x1C
    u32 m_Unknown0x20[3];          // 0x20
    u32 flags;                     // 0x2C
    u32 m_Unknown0x30[12];         // 0x30
    u32* pCommand;                 // 0x60 the vertex attribute commands (memory of pAllocator)
    u32 commandSize;               // 0x64 bytes
    u32* pDeactivateCommand;       // 0x68 the commands that turn them off again (in the same memory)
    u32 deactivateCommandSize;     // 0x6C bytes
    u32 m_Unknown0x70[4];          // 0x70
    os::IAllocator* pAllocator;    // 0x80
};
ASSERT_OFFSET(ResMeshData, pAllocator, 0x80);

// the binary resource of a mesh (the class name and the functions are from the reference symbols)
class ResMesh
{
public:
    // (inline)
    explicit ResMesh(const void* pData = nullptr) : m_pData(static_cast<const ResMeshData*>(pData)) {}
    const ResMeshData* ptr() const { return m_pData; }
    ResMeshData* ptr() { return const_cast<ResMeshData*>(m_pData); }

    void Cleanup(); // 0x0013B920 | nintendogs:bytes [tier A]
    void Setup(nw::gfx::res::ResModel, nw::os::IAllocator*, nw::gfx::res::ResGraphicsFile); // 0x004A9E64 | nintendogs:callgraph [tier A]

    const ResMeshData* m_pData; // 0x0
};
ASSERT_SIZE(ResMesh, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
