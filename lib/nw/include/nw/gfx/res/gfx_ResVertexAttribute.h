#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// A vertex attribute of a shape (only the fields known so far; names are ours): a constant value, a
// stream of one attribute, or an interleaved stream of several. The streams have their data uploaded
// by Setup like the images of a texture.
struct ResVertexAttributeData
{
    enum Flag
    {
        FLAG_CONSTANT = 0x1,     // a constant value, no stream (ResVertexParamAttributeData)
        FLAG_INTERLEAVED = 0x2,  // a stream of several attributes (ResInterleavedVertexStreamData)
    };

    u32 typeInfo;  // 0x0
    u32 usage;     // 0x4 the attribute (the index into the register map of the shader program)
    u32 flags;     // 0x8
};
ASSERT_SIZE(ResVertexAttributeData, 0xC);

// the uploadable part of a stream
struct ResVertexStreamBaseData : public ResVertexAttributeData
{
    const void* pLoaded;     // 0x0C the attribute itself once uploaded (the owner of the copy)
    u32 locationFlag;        // 0x10 where the data go (0: LOCATION_NO_COPY)
    u32 size;                // 0x14 bytes
    ut::Offset data;         // 0x18
    void* pLocationAddress;  // 0x1C the uploaded copy
    u32 locationArea;        // 0x20 the area of the copy
};
ASSERT_SIZE(ResVertexStreamBaseData, 0x24);

// a stream of one attribute (also an attribute of an interleaved stream, without the stream part)
struct ResVertexStreamData : public ResVertexStreamBaseData
{
    u32 formatType;  // 0x24 GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT or GL_FLOAT
    u8 dimension;    // 0x28 the components
    f32 scale;       // 0x2C the factor the shader multiplies the values with
};
ASSERT_OFFSET(ResVertexStreamData, dimension, 0x28);
ASSERT_OFFSET(ResVertexStreamData, scale, 0x2C);

struct ResInterleavedVertexStreamData : public ResVertexStreamBaseData
{
    u32 stride;               // 0x24 bytes per vertex
    s32 attributeCount;       // 0x28
    ut::Offset attributes;    // 0x2C ResVertexStreamData (only usage, format and dimension)
};
ASSERT_OFFSET(ResInterleavedVertexStreamData, attributes, 0x2C);

// a constant value
struct ResVertexParamAttributeData : public ResVertexAttributeData
{
    u32 m_Unknown0xC;     // 0x0C
    u8 dimension;         // 0x10 the components
    u32 m_Unknown0x14;    // 0x14
    s32 valueCount;       // 0x18
    ut::Offset values;    // 0x1C f32
};
ASSERT_OFFSET(ResVertexParamAttributeData, values, 0x1C);

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
