#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// A shape of a model (only the fields known so far; names are ours). A separate-data shape has
// primitive sets (the index streams) and vertex attributes (the vertex streams).
struct ResShapeData
{
    enum Flag
    {
        FLAG_SETUP_DONE = 0x1,
    };

    u32 typeInfo;              // 0x00
    u32 m_Unknown0x4[5];       // 0x04
    u32 flags;                 // 0x18
    u32 m_Unknown0x1C[4];      // 0x1C
};
ASSERT_SIZE(ResShapeData, 0x2C);

struct ResSeparateDataShapeData : public ResShapeData
{
    ResArrayInfo primitiveSets;     // 0x2C ResPrimitiveSetData
    u32 m_Unknown0x34;              // 0x34
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
