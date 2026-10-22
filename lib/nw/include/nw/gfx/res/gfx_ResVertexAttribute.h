#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// A vertex attribute of a shape (only the fields known so far; names are ours). One with a stream
// has its data uploaded by Setup like the images of a texture.
struct ResVertexAttributeData
{
    enum Flag
    {
        FLAG_CONSTANT = 0x1,     // a constant value, no stream
        FLAG_INTERLEAVED = 0x2,  // (set up the same way)
    };

    u32 typeInfo;            // 0x00
    u32 m_Unknown0x4;        // 0x04
    u32 flags;               // 0x08
    const void* pLoaded;     // 0x0C the attribute itself once uploaded (the owner of the copy)
    u32 locationFlag;        // 0x10 where the data go (0: LOCATION_NO_COPY)
    u32 size;                // 0x14 bytes
    ut::Offset data;         // 0x18
    void* pLocationAddress;  // 0x1C the uploaded copy
    u32 locationArea;        // 0x20 the area of the copy
};
ASSERT_SIZE(ResVertexAttributeData, 0x24);

// the binary resource of a vertex attribute (the class name and Setup are from the reference symbols)
class ResVertexAttribute
{
public:
    // (inline)
    explicit ResVertexAttribute(const void* pData = nullptr) : m_pData(static_cast<const ResVertexAttributeData*>(pData)) {}
    const ResVertexAttributeData* ptr() const { return m_pData; }
    ResVertexAttributeData* ptr() { return const_cast<ResVertexAttributeData*>(m_pData); }

    void Setup(); // 0x004A7E20 | nintendogs:bytes [tier A]
    // the copy of the stream freed (name is ours)
    void Cleanup(); // 0x004A7EC0 (name is ours)

    const ResVertexAttributeData* m_pData; // 0x0
};
ASSERT_SIZE(ResVertexAttribute, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
