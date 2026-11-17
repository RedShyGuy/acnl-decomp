#include "Resource/dResourceGetMaterial.h"

#include "g3d/dResourceLoader.h"
#include "nn/math/math_MTX34.h"

// 0x002FCF7C (name is ours)
ResourceGetMaterial::ResourceGetMaterial() : m_PlayMode(1)
{
}

// 0x002FD058 slot 0x00
// 0x002FD01C (deleting dtor)
ResourceGetMaterial::~ResourceGetMaterial()
{
    // (only the members are destroyed)
}

// 0x002FCD80 slot 0x08 (name is ours)
bool ResourceGetMaterial::Create(const char* path)
{
    if (!LoadFile(path)) {
        return false;
    }
    m_Model.Create(nw::gfx::res::ResSceneObject(m_pResourceLoader->GetGraphicsFile().GetModels(0)), &m_Allocator,
                   &m_Allocator, m_BufferOption, true, 1);
    m_MaterialAnim.Build(&m_Model, nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetTextureAnims(0)),
                         &m_Allocator, false);
    m_MaterialAnim.SetPlayMode(m_PlayMode);
    if (m_MaterialAnim.m_pAnimObject != NULL) {
        m_MaterialAnim.SetSpeed(1.0f);
    }
    m_Model.SetMaterialAnim(&m_MaterialAnim, 0);
    return true;
}

// 0x002FCF08 slot 0x0C (name is ours)
void ResourceGetMaterial::Destroy()
{
    if (m_Model.GetNode() == NULL) {
        return;
    }
    m_MaterialAnim.g3d::MaterialAnim::Finalize();
    m_Model.g3d::Model::Finalize();
}

// 0x004EE384 slot 0x10 (name is ours)
void ResourceGetMaterial::Update(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.UpdateAnims();
    m_Model.g3d::SkeletalModel::Update();
}

// 0x002FCD6C slot 0x14 (name is ours)
void ResourceGetMaterial::UpdateFrames()
{
    // empty (in the original too)
}

// 0x004EE9E0 slot 0x18 (name is ours)
void ResourceGetMaterial::AddToScene()
{
    m_Model.AddToScene(0);
}
