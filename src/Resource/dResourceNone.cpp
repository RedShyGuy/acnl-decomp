#include "Resource/dResourceNone.h"

#include "g3d/dResourceLoader.h"
#include "nn/math/math_MTX34.h"

// 0x002071B8 (name is ours)
ResourceNone::ResourceNone() : m_pModel(NULL), m_AnimKind(22)
{
}

// 0x00207234 slot 0x00
// 0x002071F8 (deleting dtor)
ResourceNone::~ResourceNone()
{
    // (only the members are destroyed)
}

// 0x00207048 slot 0x08 (name is ours)
bool ResourceNone::Create(const char*)
{
    if (m_pResourceLoader == NULL) {
        return true;
    }
    m_AnimKind = 22;
    m_Model.Create(nw::gfx::res::ResSceneObject(m_pResourceLoader->GetGraphicsFile().GetModels(0)), &m_Allocator,
                   &m_Allocator, m_BufferOption, true, 1);
    m_SkeletalAnim.Build(&m_Model, nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetSkeletalAnims(0)),
                         &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_SkeletalAnim);
    if (m_SkeletalAnim.m_pAnimObject != NULL) {
        m_SkeletalAnim.SetSpeed(1.0f);
    }
    m_Model.UpdateAnims();
    m_pModel = &m_Model;
    return true;
}

// 0x00207184 slot 0x0C (name is ours)
void ResourceNone::Destroy()
{
    m_pModel = NULL;
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
        m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
    }
}

// 0x00206FD4 slot 0x10 (name is ours)
void ResourceNone::Update(const nn::math::MTX34& matrix)
{
    if (m_pModel == NULL) {
        return;
    }
    m_pModel->SetMatrix(matrix);
    m_pModel->UpdateAnims();
    m_pModel->Update();
}

// 0x00207024 slot 0x14 (name is ours)
void ResourceNone::UpdateFrames()
{
    if (m_pModel != NULL) {
        m_pModel->UpdateAnimFrames();
    }
}

// 0x0020700C slot 0x18 (name is ours)
void ResourceNone::AddToScene()
{
    if (m_pModel != NULL) {
        m_pModel->AddToScene(0);
    }
}
