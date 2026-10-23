#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// A shape of a model (only the fields known so far; names are ours). A separate-data shape has
// primitive sets (the index streams) and vertex attributes (the vertex streams).
// the bounding box of a shape (only the field known so far; names are ours)
struct ResOrientedBoundingBoxData
{
    u32 m_Unknown0x0;        // 0x0
    nn::math::VEC3_ center;  // 0x4
};

struct ResShapeData
{
    enum Flag
    {
        FLAG_SETUP_DONE = 0x1,
    };

    u32 typeInfo;              // 0x00
    u32 m_Unknown0x4[5];       // 0x04
    u32 flags;                 // 0x18
    ut::Offset boundingBox;    // 0x1C ResOrientedBoundingBoxData
    nn::math::VEC3_ positionOffset; // 0x20 added to the positions in the shader (uniform c6)
};
ASSERT_SIZE(ResShapeData, 0x2C);

struct ResSeparateDataShapeData : public ResShapeData
{
    ResArrayInfo primitiveSets;     // 0x2C ResPrimitiveSetData
    u32 vertexBufferBase;           // 0x34 the physical address the stream offsets count from (VRAM A)
    ResArrayInfo vertexAttributes;  // 0x38 ResVertexAttributeData
};
ASSERT_OFFSET(ResSeparateDataShapeData, vertexAttributes, 0x38);

// the binary resource of a shape (the class name and Setup are from the reference symbols)
class ResShape
{
public:
    // (inline)
    explicit ResShape(const void* pData = nullptr) : m_pData(static_cast<const ResShapeData*>(pData)) {}
    const ResShapeData* ptr() const { return m_pData; }
    ResShapeData* ptr() { return const_cast<ResShapeData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }

    u32 Setup(os::IAllocator* pAllocator); // 0x004AA688 | nintendogs:callgraph [tier A]
    void Cleanup(); // 0x0013B98C (name is ours)

    const ResShapeData* m_pData; // 0x0
};
ASSERT_SIZE(ResShape, 0x4);

// the binary resource of a shape with separate vertex streams (the class name is from the reference
// symbols)
class ResSeparateDataShape : public ResShape
{
public:
    // (inline)
    explicit ResSeparateDataShape(const void* pData = nullptr) : ResShape(pData) {}
    const ResSeparateDataShapeData* ptr() const { return static_cast<const ResSeparateDataShapeData*>(m_pData); }
    ResSeparateDataShapeData* ptr() { return const_cast<ResSeparateDataShapeData*>(static_cast<const ResSeparateDataShapeData*>(m_pData)); }
};
} // namespace res
} // namespace gfx
} // namespace nw
