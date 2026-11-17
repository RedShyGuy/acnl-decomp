#include "Resource/dResourceGetSklVisMat.h"

#include "g3d/dResourceLoader.h"

// 0x00321C24 (name is ours)
ResourceGetSklVisMat::ResourceGetSklVisMat()
{
    // (only the members are constructed)
}

// 0x00321D18 slot 0x00
// 0x00321CA4 (deleting dtor)
ResourceGetSklVisMat::~ResourceGetSklVisMat()
{
    // (only the members are destroyed)
}

// 0x003215E4 slot 0x08 (name is ours)
bool ResourceGetSklVisMat::Create(const char* path)
{
    m_AnimKind = ANIM_NUM;
    if (!LoadFile(path)) {
        return false;
    }
    CreateModelAndSkeletalAnim();
    if (m_pResourceLoader->GetGraphicsFile().GetVisibilityAnimsCount() != 0) {
        m_VisibilityAnim.Build(&m_Model,
                               nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetVisibilityAnims(0)),
                               &m_Allocator);
        m_VisibilityAnim.SetPlayMode(m_PlayMode);
        if (m_VisibilityAnim.m_pAnimObject != NULL) {
            m_VisibilityAnim.SetSpeed(1.0f);
        }
        m_Model.SetVisibilityAnim(&m_VisibilityAnim);
    }
    if (m_pResourceLoader->GetGraphicsFile().GetTextureAnimsCount() != 0) {
        m_MaterialAnim.Build(&m_Model,
                             nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetTextureAnims(0)),
                             &m_Allocator, false);
        m_MaterialAnim.SetPlayMode(0);
        if (m_MaterialAnim.m_pAnimObject != NULL) {
            m_MaterialAnim.SetSpeed(0.0f);
        }
        m_Model.SetMaterialAnim(&m_MaterialAnim, 0);
    }
    m_Model.UpdateAnims();
    return true;
}

// 0x00321868 slot 0x0C (name is ours)
void ResourceGetSklVisMat::Destroy()
{
    if (m_Model.GetNode() == NULL) {
        return;
    }
    if (m_pResourceLoader->GetGraphicsFile().GetVisibilityAnimsCount() != 0) {
        m_VisibilityAnim.g3d::VisibilityAnim::Finalize();
    }
    if (m_pResourceLoader->GetGraphicsFile().GetTextureAnimsCount() != 0) {
        m_MaterialAnim.g3d::MaterialAnim::Finalize();
    }
    m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
    m_Model.g3d::Model::Finalize();
}

// 0x00321928 (name is ours)
void ResourceGetSklVisMat::SetMaterialAnim(u32 kind)
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

// 0x003218D8 (name is ours)
void ResourceGetSklVisMat::SetMaterialAnim(u32 kind, f32 speed)
{
    SetMaterialAnim(kind);
    if (m_MaterialAnim.m_pAnimObject != NULL) {
        m_MaterialAnim.SetSpeed(speed);
    }
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetSpeed(speed);
    }
}

// 0x00321A8C (name is ours)
void ResourceGetSklVisMat::SetAllAnims(u32 kind)
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
    void* visibilityAnim = m_pResourceLoader->GetGraphicsFile().GetVisibilityAnims(s_AnimNames[kind]);
    if (visibilityAnim == NULL) {
        return;
    }
    m_AnimKind = kind;
    m_SkeletalAnim.ChangeAnim(nw::anim::res::ResAnim(skeletalAnim));
    m_SkeletalAnim.SetPlayMode(m_PlayMode);
    m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(materialAnim));
    m_MaterialAnim.SetPlayMode(m_PlayMode);
    m_VisibilityAnim.ChangeAnim(nw::anim::res::ResAnim(visibilityAnim));
    m_VisibilityAnim.SetPlayMode(m_PlayMode);
}

// 0x00321A28 (name is ours)
void ResourceGetSklVisMat::SetAllAnims(u32 kind, f32 speed)
{
    SetAllAnims(kind);
    if (m_MaterialAnim.m_pAnimObject != NULL) {
        m_MaterialAnim.SetSpeed(speed);
    }
    if (m_VisibilityAnim.m_pAnimObject != NULL) {
        m_VisibilityAnim.SetSpeed(speed);
    }
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetSpeed(speed);
    }
}
