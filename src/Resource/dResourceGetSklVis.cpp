#include "Resource/dResourceGetSklVis.h"

#include "g3d/dResourceLoader.h"

// 0x002D17A8 (name is ours)
ResourceGetSklVis::ResourceGetSklVis()
{
    // (only the members are constructed)
}

// 0x002D1864 slot 0x00
// 0x002D180C (deleting dtor)
ResourceGetSklVis::~ResourceGetSklVis()
{
    // (only the members are destroyed)
}

// 0x002D13E4 slot 0x08 (name is ours)
bool ResourceGetSklVis::Create(const char* path)
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
    m_Model.UpdateAnims();
    return true;
}

// 0x002D15D0 slot 0x0C (name is ours)
void ResourceGetSklVis::Destroy()
{
    if (m_Model.GetNode() == NULL) {
        return;
    }
    if (m_pResourceLoader->GetGraphicsFile().GetVisibilityAnimsCount() != 0) {
        m_VisibilityAnim.g3d::VisibilityAnim::Finalize();
    }
    m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
    m_Model.g3d::Model::Finalize();
}

// 0x002D1670 (name is ours)
void ResourceGetSklVis::SetAnim(u32 kind)
{
    if (kind >= ANIM_NUM || m_AnimKind == kind || m_pResourceLoader == NULL) {
        return;
    }
    void* skeletalAnim = m_pResourceLoader->m_GraphicsFile.GetSkeletalAnims(s_AnimNames[kind]);
    if (skeletalAnim == NULL) {
        return;
    }
    void* visibilityAnim = m_pResourceLoader->GetGraphicsFile().GetVisibilityAnims(s_AnimNames[kind]);
    if (visibilityAnim == NULL) {
        return;
    }
    m_AnimKind = kind;
    m_SkeletalAnim.ChangeAnim(nw::anim::res::ResAnim(skeletalAnim));
    m_SkeletalAnim.SetPlayMode(m_PlayMode);
    m_VisibilityAnim.ChangeAnim(nw::anim::res::ResAnim(visibilityAnim));
    m_VisibilityAnim.SetPlayMode(m_PlayMode);
}

// 0x002D1620 (name is ours)
void ResourceGetSklVis::SetAnim(u32 kind, f32 speed)
{
    SetAnim(kind);
    if (m_VisibilityAnim.m_pAnimObject != NULL) {
        m_VisibilityAnim.SetSpeed(speed);
    }
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetSpeed(speed);
    }
}

// 0x002D1778 (name is ours)
void ResourceGetSklVis::SetVisibilityAnimFrame(f32 frame)
{
    if (m_VisibilityAnim.m_pAnimObject != NULL) {
        m_VisibilityAnim.SetFrame(frame);
    }
}

// 0x002D1790 (name is ours)
void ResourceGetSklVis::SetVisibilityAnimSpeed(f32 speed)
{
    if (m_VisibilityAnim.m_pAnimObject != NULL) {
        m_VisibilityAnim.SetSpeed(speed);
    }
}
