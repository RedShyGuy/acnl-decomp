#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// An index stream of a primitive (only the fields known so far; names are ours). Setup uploads the
// indices like the images of a texture (to the area of the location flag) and gives the stream a
// buffer for its draw commands.
struct ResIndexStreamData
{
    u32 format;              // 0x00 the GL type of the indices (GL_UNSIGNED_BYTE / GL_UNSIGNED_SHORT)
    u8 primitiveMode;        // 0x04
    bool isEnabled;          // 0x05 drawn by RenderContext::RenderPrimitive
    u8 m_Unknown0x6[2];      // 0x06
    u32 size;                // 0x08 bytes
    ut::Offset data;         // 0x0C
    u32 m_Unknown0x10;       // 0x10
    u32 locationFlag;        // 0x14 where the indices go (0: LOCATION_NO_COPY)
    u32* pCommand;           // 0x18 the draw commands (memory of the allocator of the primitive)
    u32 commandSize;         // 0x1C bytes
    void* pLocationAddress;  // 0x20 the uploaded copy
    u32 locationArea;        // 0x24 the area of the copy
};
ASSERT_SIZE(ResIndexStreamData, 0x28);

// the binary resource of an index stream (the class name is from the reference symbols)
class ResIndexStream
{
public:
    // (inline)
    explicit ResIndexStream(const void* pData = nullptr) : m_pData(static_cast<const ResIndexStreamData*>(pData)) {}
    const ResIndexStreamData* ptr() const { return m_pData; }
    ResIndexStreamData* ptr() { return const_cast<ResIndexStreamData*>(m_pData); }

    const ResIndexStreamData* m_pData; // 0x0
};
ASSERT_SIZE(ResIndexStream, 0x4);

// a primitive: its index streams (names are ours)
struct ResPrimitiveData
{
    enum Flag
    {
        FLAG_DRAW_COMMAND_DONE = 0x1,
    };

    ResArrayInfo indexStreams;    // 0x00 ResIndexStreamData
    s32 bufferObjectCount;        // 0x08
    ut::Offset bufferObjects;     // 0x0C per index stream: the stream once uploaded (the owner of its copy)
    u32 flags;                    // 0x10
    os::IAllocator* pAllocator;   // 0x14 the allocator of the command buffers
};
ASSERT_SIZE(ResPrimitiveData, 0x18);

// the binary resource of a primitive (the class name and the functions are from the reference symbols)
class ResPrimitive
{
public:
    // the size of the draw command buffer of an index stream
    static const u32 DRAW_COMMAND_SIZE = 112;

    // (inline)
    explicit ResPrimitive(const void* pData = nullptr) : m_pData(static_cast<const ResPrimitiveData*>(pData)) {}
    const ResPrimitiveData* ptr() const { return m_pData; }
    ResPrimitiveData* ptr() { return const_cast<ResPrimitiveData*>(m_pData); }

    // the draw commands of each index stream built
    void SetupDrawCommand(bool isFixedPrimitive); // 0x004A67E8 | nintendogs:bytes [tier A]
    // the index streams uploaded, their command buffers allocated
    void Setup(os::IAllocator* pAllocator); // 0x004A6880 | nintendogs:bytes [tier A]

    const ResPrimitiveData* m_pData; // 0x0
};
ASSERT_SIZE(ResPrimitive, 0x4);

// the purpose of the memory for the allocator of nngx (a vertex or index buffer)
const u32 PURPOSE_VERTEX_BUFFER = 0x102;
} // namespace res
} // namespace gfx
} // namespace nw
