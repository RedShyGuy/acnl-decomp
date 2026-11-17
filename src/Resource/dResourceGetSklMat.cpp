#include "Resource/dResourceGetSklMat.h"

#include "g3d/dResourceLoader.h"

// 0x002D12C4 (name is ours)
ResourceGetSklMat::ResourceGetSklMat()
{
    // (only the members are constructed)
}

// 0x002D1380 slot 0x00
// 0x002D1328 (deleting dtor)
ResourceGetSklMat::~ResourceGetSklMat()
{
    // (only the members are destroyed)
}

// 0x002D0F90 slot 0x08 (name is ours)
bool ResourceGetSklMat::Create(const char* path)
{
    m_AnimKind = ANIM_NUM;
    if (!LoadFile(path)) {
        return false;
    }
    CreateModelAndSkeletalAnim();
    if (m_pResourceLoader->GetGraphicsFile().GetTextureAnimsCount() != 0) {
        m_MaterialAnim.Build(&m_Model,
                             nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetTextureAnims(0)),
                             &m_Allocator, false);
        m_MaterialAnim.SetPlayMode(0);
        m_MaterialAnim.SetSpeed(0.0f);
        m_Model.SetMaterialAnim(&m_MaterialAnim, 0);
    }
    m_Model.UpdateAnims();
    return true;
}

// 0x002D1174 slot 0x0C (name is ours)
void ResourceGetSklMat::Destroy()
{
    if (m_Model.GetNode() == NULL) {
        return;
    }
    if (m_pResourceLoader->GetGraphicsFile().GetTextureAnimsCount() != 0) {
        m_MaterialAnim.g3d::MaterialAnim::Finalize();
    }
    m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
    m_Model.g3d::Model::Finalize();
}

// 0x002D11C4 (name is ours)
void ResourceGetSklMat::SetAnim(u32 kind)
{
    if (kind >= ANIM_NUM || m_AnimKind == kind) {
        return;
    }
    void* skeletalAnim = m_pResourceLoader->m_GraphicsFile.GetSkeletalAnims(s_AnimNames[kind]);
    if (skeletalAnim == NULL) {
        return;
    }
    void* materialAnim = m_pResourceLoader->GetGraphicsFile().GetTextureAnims(s_AnimNames[kind]);
    if (materialAnim == NULL) {
        return;
    }
    m_AnimKind = kind;
    m_SkeletalAnim.ChangeAnim(nw::anim::res::ResAnim(skeletalAnim));
    m_SkeletalAnim.SetPlayMode(m_PlayMode);
    m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(materialAnim));
    m_MaterialAnim.SetPlayMode(m_PlayMode);
}
