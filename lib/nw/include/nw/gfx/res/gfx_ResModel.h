#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
namespace res {
// A model (3dbrew "CGFX": CMDL; only the fields known so far, names are ours): a transform node with
// meshes, materials (MTOB, by name) and shapes.
struct ResModelData : public ResTransformNodeData
{
    ResArrayInfo meshes;     // 0xB4 ResMeshData
    ResDicInfo materials;    // 0xBC ResMaterialData
    ResArrayInfo shapes;     // 0xC4 ResShapeData
};
ASSERT_OFFSET(ResModelData, meshes, 0xB4);
ASSERT_OFFSET(ResModelData, shapes, 0xC4);

// the binary resource of a model (the class name and the functions are from the reference symbols)
class ResModel : public ResTransformNode
{
public:
    // (inline)
    explicit ResModel(const void* pData = nullptr) : ResTransformNode(pData) {}
    const ResModelData* ptr() const { return static_cast<const ResModelData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }
    void* GetMesh(int index) const { return ptr()->meshes.GetData(index); }
    void* GetShape(int index) const { return ptr()->shapes.GetData(index); }

    void Cleanup(); // 0x00137780 | nintendogs:bytes [tier A]
    // the shapes, the materials and then the meshes (not if a shader is missing)
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004AA4C4 | nintendogs:bytes [tier A]
    // the material of the name (an invalid one: none; name is ours)
    ResMaterial GetMaterial(const char* name) const; // 0x004AA41C (name is ours)
    // the material at index of the dictionary (name is ours)
    ResMaterial GetMaterial(int index) const; // 0x004AA48C (name is ours)
};
ASSERT_SIZE(ResModel, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
