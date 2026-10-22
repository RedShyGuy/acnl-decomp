#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/gfx/res/gfx_ResShape.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
typedef void (*ShapeSetupFunc)(os::IAllocator* pAllocator, ResShapeData* pData);
typedef void (*ShapeCleanupFunc)(ResShapeData* pData);

// 0x004A96C0 (name is ours)
// the index streams of every primitive, then the vertex streams
void SetupSeparateDataShape(os::IAllocator* pAllocator, ResShapeData* pData)
{
    ResSeparateDataShapeData* pShape = static_cast<ResSeparateDataShapeData*>(pData);
    const int primitiveSetCount = pShape->primitiveSets.count;
    for (int i = 0; i < primitiveSetCount; ++i) {
        const ResPrimitiveSetData* pPrimitiveSet = static_cast<const ResPrimitiveSetData*>(pShape->primitiveSets.GetData(i));
        const ut::Offset* pBegin = pPrimitiveSet->primitives.array.To<ut::Offset>();
        const ut::Offset* pEnd = pBegin + pPrimitiveSet->primitives.count;
        for (const ut::Offset* pIt = pBegin; pIt != pEnd; ++pIt) {
            ResPrimitive(pIt->ToPtr()).Setup(pAllocator);
        }
    }
    const int attributeCount = pShape->vertexAttributes.count;
    for (int i = 0; i < attributeCount; ++i) {
        ResVertexAttribute(pShape->vertexAttributes.GetData(i)).Setup();
    }
}

// 0x004A8620 (name is ours)
void SetupEmptyShape(os::IAllocator* /*pAllocator*/, ResShapeData* /*pData*/)
{
    // empty (in the original too)
}

// 0x004A9964 (name is ours)
// the vertex streams, then the index streams freed
void CleanupSeparateDataShape(ResShapeData* pData)
{
    ResSeparateDataShapeData* pShape = static_cast<ResSeparateDataShapeData*>(pData);
    const int attributeCount = pShape->vertexAttributes.count;
    for (int i = 0; i < attributeCount; ++i) {
        ResVertexAttribute(pShape->vertexAttributes.GetData(i)).Cleanup();
    }
    const int primitiveSetCount = pShape->primitiveSets.count;
    for (int i = 0; i < primitiveSetCount; ++i) {
        ResPrimitiveSet(pShape->primitiveSets.GetData(i)).Cleanup();
    }
}

// 0x004A8C4C (name is ours)
void CleanupEmptyShape(ResShapeData* /*pData*/)
{
    // empty (in the original too)
}
} // namespace

// the setup and cleanup per type of shape (only the first entries are used; names are ours)
// 0x0097F4B4 (name is ours)
ShapeSetupFunc s_ShapeSetupFuncs[] = {SetupSeparateDataShape, SetupEmptyShape};
// 0x0097F4BC (name is ours)
ShapeCleanupFunc s_ShapeCleanupFuncs[] = {CleanupSeparateDataShape, CleanupEmptyShape};

// 0x004AA688 | nintendogs:callgraph [tier A]
u32 nw::gfx::res::ResShape::Setup(os::IAllocator* pAllocator)
{
    s_ShapeSetupFuncs[0](pAllocator, ptr());
    ptr()->flags |= ResShapeData::FLAG_SETUP_DONE;
    return RESULT_SUCCESS;
}

// 0x0013B98C (name is ours)
void nw::gfx::res::ResShape::Cleanup()
{
    s_ShapeCleanupFuncs[0](ptr());
    ptr()->flags &= ~ResShapeData::FLAG_SETUP_DONE;
}

} // namespace res
} // namespace gfx
} // namespace nw
