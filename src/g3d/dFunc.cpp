#include "g3d/dFunc.h"
#include <new>

namespace g3d {
// 0x004F1314 (name is ours)
void g3d::Func::SetFunctions(FuncNode::DrawFunc opaqueFunc, FuncNode::DrawFunc translucentFunc, void* userData)
{
    FuncNode* node = nw::gfx::DynamicCast<FuncNode>(m_pNode);
    node->m_pOpaqueFunc = opaqueFunc;
    node->m_pTranslucentFunc = translucentFunc;
    node->m_pUserData = userData;
}

// 0x004F1370 (name is ours)
bool g3d::Func::Initialize(nw::os::IAllocator* allocator)
{
    FuncNode::Description description;
    description.isFixedSizeMemory = true;
    description.isAnimationEnabled = true;
    description.maxAnimObjectsPerGroup = 1;
    description.maxChildren = 0;
    description.maxCallbacks = 0;
    void* memory = allocator->Alloc(sizeof(FuncNode), 4);
    FuncNode* node = (memory != NULL) ? new (memory) FuncNode(allocator, description) : NULL;
    node->Initialize(allocator);
    m_pNode = node;
    return true;
}

// 0x004F1418 (name is ours)
g3d::Func::Func()
{
    // (only the base is constructed)
}

// 0x004F1458 slot 0x00
// 0x004F1430 (deleting dtor)
g3d::Func::~Func()
{
    // (only the base is destroyed)
}

} // namespace g3d
