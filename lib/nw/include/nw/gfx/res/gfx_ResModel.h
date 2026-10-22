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
    static const u32 TYPE = 0x40000012;

    ResArrayInfo meshes;              // 0xB4 ResMeshData
    ResDicInfo materials;             // 0xBC ResMaterialData
    ResArrayInfo shapes;              // 0xC4 ResShapeData
    ResDicInfo meshNodeVisibilities;  // 0xCC ResMeshNodeVisibilityData
    bool isVisible;                   // 0xD4
};
ASSERT_OFFSET(ResModelData, meshes, 0xB4);
ASSERT_OFFSET(ResModelData, shapes, 0xC4);
ASSERT_OFFSET(ResModelData, isVisible, 0xD4);

// a model with a skeleton (3dbrew "CGFX": the skeletal model; names are ours)
struct ResSkeletalModelData : public ResModelData
{
    static const u32 TYPE = 0x40000092;

    u32 m_Unknown0xD8[2];  // 0xD8
    ut::Offset skeleton;   // 0xE0 ResSkeletonData
};
ASSERT_OFFSET(ResSkeletalModelData, skeleton, 0xE0);

// the visibility of a group of meshes by name (a visibility animation changes it; names are ours)
struct ResMeshNodeVisibilityData
{
    ut::Offset name; // 0x0
    bool isVisible;  // 0x4
};
ASSERT_SIZE(ResMeshNodeVisibilityData, 0x8);

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
    ResMeshNodeVisibilityData* GetMeshNodeVisibility(int index) const
    {
        return static_cast<ResMeshNodeVisibilityData*>(ptr()->meshNodeVisibilities.GetData(index));
    }

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
