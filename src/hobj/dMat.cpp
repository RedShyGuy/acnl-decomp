#include "hobj/dMat.h"

#include "Other/dHeapInit.h"
#include "Sound/sound_Extern.h"
#include "hobj/hobj_Extern.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_Model.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "Bs/dBsStrcMgr.h"

namespace hobj {
// 0x00530CF4 (name is ours)
Mat::Mat() : m_ResourceMgr(), m_Kind(0)
{
}

// 0x00530D70 (name is ours)
Mat::~Mat()
{
    // (only the members are destroyed)
}

// 0x00530B44 (name is ours)
void Mat::Initialize(u8 kind)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x1000, parent, "hobj::Mat::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_ResourceMgr.Initialize(parent, 0x4400);
    m_Kind = kind;
}

// 0x00530924 (name is ours)
bool Mat::Load()
{
    sead::FixedSafeString<64> path;
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/mat/hobj_mat_%02d/hobj_mat_%02d.bcres");
        formatter.appendValue(kind);
        formatter.appendValue(kind);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/mat/hobj_mat_%02d/Textures/%s.bcres");
        formatter.appendValue(kind);
        formatter.append(g_SeasonTextureFolders[Func630618(BsStrcMgr::s_pInstance)]);
        formatter.flush();
    }
    m_TextureKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_TextureKey.IsValid();
}

// 0x00530820 (name is ours)
void Mat::Build()
{
    g3d::ResourceLoader* shared = Func2B6AC4();
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    g3d::ResourceLoader* textures = m_ResourceMgr.Get(m_TextureKey);
    Func4DBE3C(&m_Model, model, textures, shared, &m_Allocator);
}

// 0x00530C04 (name is ours)
void Mat::Finalize()
{
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}

// 0x00530BC8 (name is ours)
void Mat::AddToScene()
{
    m_Model.AddToScene(0);
}

// 0x00530BD4 (name is ours)
void Mat::Update(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.g3d::TransformNode::Update();
}

// 0x00530BF0 (name is ours)
const void* Mat::GetResource() const
{
    return m_Model.GetModel()->GetResSceneObject().ptr();
}

// 0x00530CD4 (name is ours)
nn::math::MTX34 Mat::GetLocalMatrix() const
{
    return m_Model.GetLocalTransform().m_TransformMatrix;
}
} // namespace hobj
