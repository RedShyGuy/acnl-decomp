#pragma once

#include "decomp.h"
#include "g3d/dFuncNode.h"
#include "g3d/dTransformNode.h"

namespace g3d {
// an object with a FuncNode: custom drawing in the passes of the renderer (names are ours)
// RTTI N3g3d4FuncE @ 0x008D0D60
// vtable 0x00903BAC (vptr 0x00903BB4), offset_to_top 0, 4 entries
class Func : public ::g3d::TransformNode
{
public:
    Func(); // 0x004F1418
    virtual ~Func(); // 0x004F1458 slot 0x00
    // 0x004F1430 (deleting dtor)

    // the node in the memory of allocator; always true
    bool Initialize(nw::os::IAllocator* allocator); // 0x004F1370
    // the functions of the node
    void SetFunctions(FuncNode::DrawFunc opaqueFunc, FuncNode::DrawFunc translucentFunc, void* userData); // 0x004F1314
};
ASSERT_SIZE(Func, 0x8);
} // namespace g3d
