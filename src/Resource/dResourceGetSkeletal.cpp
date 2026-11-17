#include "Resource/dResourceGetSkeletal.h"

#include "g3d/dResourceLoader.h"
#include "g3d/g3d_System.h"
#include "nn/math/math_MTX34.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"

// 0x00954AB4
const char* ResourceGetSkeletal::s_AnimNames[ResourceGetSkeletal::ANIM_NUM] = {
    "wait", "stop", "fly", "get", "book", "swim", "book", "get", "die0", "die1", "start",
    "wait", "finish", "inout", "wait", "out", "attack", "in", "inout", "escape", "default", "break",
};

namespace {
// the material of index of a model: its resource or its buffer (inline)
inline nw::gfx::res::ResMaterialData* GetMaterialData(nw::gfx::Model* model, s32 index, bool isOriginal)
{
    nw::gfx::Material* material = model->GetMaterial(index);
    const nw::gfx::res::ResMaterial resource = isOriginal ? material->GetOriginal() : material->GetBuffer();
    return const_cast<nw::gfx::res::ResMaterialData*>(resource.ptr());
}
} // namespace

// 0x002FE20C (name is ours)
ResourceGetSkeletal::ResourceGetSkeletal() : m_AnimKind(ANIM_NUM), m_PlayMode(1), m_IsUpdateAnims(true)
{
}

// 0x002FE28C slot 0x00
// 0x002FE250 (deleting dtor)
ResourceGetSkeletal::~ResourceGetSkeletal()
{
    // (only the members are destroyed)
}

// 0x002FDC48 slot 0x08 (name is ours)
bool ResourceGetSkeletal::Create(const char* path)
{
    m_AnimKind = ANIM_NUM;
    if (!LoadFile(path)) {
        return false;
    }
    CreateModelAndSkeletalAnim();
    m_Model.UpdateAnims();
    return true;
}

// 0x002FDE2C slot 0x0C (name is ours)
void ResourceGetSkeletal::Destroy()
{
    if (m_Model.GetNode() == NULL) {
        return;
    }
    m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
    m_Model.g3d::Model::Finalize();
}

// 0x002FD090 slot 0x10 (name is ours)
void ResourceGetSkeletal::Update(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    if (m_IsUpdateAnims) {
        m_Model.UpdateAnims();
    }
    m_Model.g3d::SkeletalModel::Update();
}

// 0x002FD3DC slot 0x14 (name is ours)
void ResourceGetSkeletal::UpdateFrames()
{
    m_Model.UpdateAnimFrames();
}

// 0x002FD3D0 slot 0x18 (name is ours)
void ResourceGetSkeletal::AddToScene()
{
    m_Model.AddToScene(0);
}

// 0x002FDF18 slot 0x1C (name is ours)
void ResourceGetSkeletal::AddToSubScene()
{
    m_Model.AddToScene(1);
}

// 0x002FDF24 slot 0x20 (name is ours)
g3d::Model* ResourceGetSkeletal::GetModel()
{
    return &m_Model;
}

// 0x002FDEA8 (name is ours)
void ResourceGetSkeletal::SetAnim(u32 kind)
{
    if (kind >= ANIM_NUM || m_AnimKind == kind) {
        return;
    }
    void* anim = m_pResourceLoader->m_GraphicsFile.GetSkeletalAnims(s_AnimNames[kind]);
    if (anim == NULL) {
        return;
    }
    m_AnimKind = kind;
    m_SkeletalAnim.ChangeAnim(nw::anim::res::ResAnim(anim));
    m_SkeletalAnim.SetPlayMode(m_PlayMode);
}

// 0x002FDF2C (name is ours)
void ResourceGetSkeletal::SetAnimFrame(f32 frame)
{
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetFrame(frame);
    }
}

// 0x002FDF44 (name is ours)
void ResourceGetSkeletal::SetAnimSpeed(f32 speed)
{
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetSpeed(speed);
    }
}

// 0x002FDF90 (name is ours)
void ResourceGetSkeletal::SetMainAlpha(s32 alpha, bool withHire, bool isOriginal)
{
    const s32 main = m_Model.GetModel()->GetResModel().ptr()->materials.GetDic().GetIndex("m_main");
    nw::gfx::res::ResMaterialData* material = GetMaterialData(m_Model.GetModel(), main, isOriginal);
    if (material != NULL) {
        g3d::SetConstantColor(material, 1, -1, -1, -1, alpha);
    }
    if (!withHire) {
        return;
    }
    const s32 hire = m_Model.GetModel()->GetResModel().ptr()->materials.GetDic().GetIndex("hire");
    material = GetMaterialData(m_Model.GetModel(), hire, isOriginal);
    if (material != NULL) {
        g3d::SetConstantColor(material, 1, -1, -1, -1, alpha);
    }
}

// 0x002FE1C0 (name is ours)
void ResourceGetSkeletal::SetMainAlphaOfBuffer(s32 alpha)
{
    Func174BB0(&m_Model, alpha, "m_main", false, 1);
}

// 0x002FE1E8 (name is ours)
void ResourceGetSkeletal::SetSeaShadowAlpha(g3d::Model* model, s32 alpha)
{
    Func174BB0(model, alpha, "sdw_sea", false, 0);
}
