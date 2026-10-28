#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResShape.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// the description bit of RESULT_SHADER_NOT_FOUND: no mesh is set up without its shader
const u32 RESULT_SHADER_NOT_FOUND_BIT = 0x2;
} // namespace

// 0x00137780 | nintendogs:bytes [tier A]
void nw::gfx::res::ResModel::Cleanup()
{
    const int shapeCount = ptr()->shapes.count;
    for (int i = 0; i < shapeCount; ++i) {
        if (ResShape(GetShape(i)).ptr()->flags & ResShapeData::FLAG_SETUP_DONE) {
            ResShape(GetShape(i)).Cleanup();
        }
    }
    const int materialCount = ptr()->materials.count;
    for (int i = 0; i < materialCount; ++i) {
        GetMaterial(i).Cleanup();
    }
    const int meshCount = ptr()->meshes.count;
    for (int i = 0; i < meshCount; ++i) {
        ResMesh(GetMesh(i)).Cleanup();
    }
}

// 0x004AA4C4 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResModel::Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    const int shapeCount = ptr()->shapes.count;
    for (int i = 0; i < shapeCount; ++i) {
        if (!(ResShape(GetShape(i)).ptr()->flags & ResShapeData::FLAG_SETUP_DONE)) {
            result |= ResShape(GetShape(i)).Setup(pAllocator);
        }
    }
    const int materialCount = ptr()->materials.count;
    for (int i = 0; i < materialCount; ++i) {
        if (!(GetMaterial(i).ptr()->flags & ResMaterialData::FLAG_SETUP_DONE)) {
            result |= GetMaterial(i).Setup(pAllocator, graphicsFile);
        }
    }
    if (!(result & RESULT_SHADER_NOT_FOUND_BIT)) {
        const int meshCount = ptr()->meshes.count;
        for (int i = 0; i < meshCount; ++i) {
            ResMesh mesh(GetMesh(i));
            if (!(mesh.ptr()->flags & ResMeshData::FLAG_SETUP_DONE)) {
                mesh.Setup(*this, pAllocator, graphicsFile);
            }
        }
    }
    return result;
}

// 0x004AA41C (name is ours)
nw::gfx::res::ResMaterial nw::gfx::res::ResModel::GetMaterial(const char* name) const
{
    return ResMaterial(ptr()->materials.GetData(name));
}

// 0x004AA48C (name is ours)
nw::gfx::res::ResMaterial nw::gfx::res::ResModel::GetMaterial(int index) const
{
    return ResMaterial(ptr()->materials.GetData(index));
}

} // namespace res
} // namespace gfx
} // namespace nw
