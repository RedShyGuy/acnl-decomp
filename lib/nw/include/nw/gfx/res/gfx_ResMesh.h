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
        FLAG_USAGE3_HAS_4_COMPONENTS = 0x1,  // the attribute of usage 3 has four components
        FLAG_USAGE8_HAS_4_COMPONENTS = 0x2,  // the attribute of usage 8 has four components
        FLAG_RENDER_KEY_CACHED = 0x4,        // renderKeyCache holds the key (a render queue that caches keys)
        FLAG_SETUP_DONE = 0x8,
    };

    // the words of scaleCommand
    static const int SCALE_COMMAND_WORDS = 12;

    u32 typeInfo;                  // 0x00
    u32 signature;                 // 0x04
    u32 revision;                  // 0x08
    ut::Offset name;               // 0x0C
    s32 userDataCount;             // 0x10
    ut::Offset userDataDic;        // 0x14
    s32 shapeIndex;                // 0x18
    s32 materialIndex;             // 0x1C
    ut::Offset owner;              // 0x20 the ResModelData (of a copy: the model it belongs to)
    bool isVisible;                // 0x24
    u8 renderPriority;             // 0x25 (in the render keys)
    s16 meshNodeVisibilityIndex;   // 0x26 the ResMeshNodeVisibilityData of the mesh (-1: none)
    s32 primitiveIndex;            // 0x28 the primitive of each primitive set that is drawn
    u32 flags;                     // 0x2C
    // the scales of the vertex attributes as two float uniforms of the vertex shader (c7: usages 0-3,
    // c8: usages 4, 5, 6 and 8; written by Setup)
    u32 scaleCommand[SCALE_COMMAND_WORDS]; // 0x30
    u32* pCommand;                 // 0x60 the vertex attribute commands (memory of pAllocator)
    u32 commandSize;               // 0x64 bytes
    u32* pDeactivateCommand;       // 0x68 the commands that turn them off again (in the same memory)
    u32 deactivateCommandSize;     // 0x6C bytes
    ut::Offset m_Unknown0x70;      // 0x70
    u32 m_Unknown0x74;             // 0x74
    u64 renderKeyCache;            // 0x78 the key of the mesh in a render queue (FLAG_RENDER_KEY_CACHED)
    os::IAllocator* pAllocator;    // 0x80
    u32 m_Unknown0x84;             // 0x84 (not copied with the resource)
};
ASSERT_OFFSET(ResMeshData, pAllocator, 0x80);
ASSERT_SIZE(ResMeshData, 0x88);

// the binary resource of a mesh (the class name and the functions are from the reference symbols)
class ResMesh
{
public:
    // (inline)
    explicit ResMesh(const void* pData = nullptr) : m_pData(static_cast<const ResMeshData*>(pData)) {}
    const ResMeshData* ptr() const { return m_pData; }
    ResMeshData* ptr() { return const_cast<ResMeshData*>(m_pData); }

    void Cleanup(); // 0x0013B920 | nintendogs:bytes [tier A]
    // the vertex attribute commands of the shape for the shader program of the material built
    u32 Setup(ResModel model, os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A9E64 | nintendogs:callgraph [tier A]

    const ResMeshData* m_pData; // 0x0
};
ASSERT_SIZE(ResMesh, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
