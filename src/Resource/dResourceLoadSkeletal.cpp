#include "Resource/dResourceLoadSkeletal.h"

#include "nn/math/math_MTX34.h"
#include "sead/seadSafeStringBase.h"

// 0x00321E5C (name is ours)
ResourceLoadSkeletal::ResourceLoadSkeletal()
{
    m_pResourceLoader = &m_Loader;
}

// (both entries call the body with the VTT argument at 0x00321F30, which GCC writes as the base-object destructor;
// check.py does not compare that variant)
// 0x00321F20 slot 0x00
// 0x00321F0C (deleting dtor)
ResourceLoadSkeletal::~ResourceLoadSkeletal()
{
    // (only the members are destroyed)
}

// 0x00321E2C slot 0x0C (name is ours)
void ResourceLoadSkeletal::Destroy()
{
    if (m_Model.GetNode() != NULL) {
        m_SkeletalAnim.g3d::SkeletalAnim::Finalize();
        m_Model.g3d::Model::Finalize();
    }
    m_Loader.Unload();
}

// 0x00321D8C slot 0x10 (name is ours)
void ResourceLoadSkeletal::Update(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.UpdateAnims();
    OnUpdate();
    m_Model.g3d::SkeletalModel::Update();
}

// 0x00321DC0 slot 0x24 (name is ours)
bool ResourceLoadSkeletal::LoadFile(const char* path)
{
    m_Loader.Load(path, m_Allocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT);
    m_Loader.InitializeCgfx(&m_Allocator, NULL, true);
    return true;
}

// 0x00321D88 slot 0x28 (name is ours)
void ResourceLoadSkeletal::OnUpdate()
{
    // empty (in the original too)
}
