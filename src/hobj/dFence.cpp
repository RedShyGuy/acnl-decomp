#include "hobj/dFence.h"

#include "Other/dHeapInit.h"
#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "hobj/hobj_Extern.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"

namespace hobj {
// 0x0053368C (name is ours)
void Fence::Build()
{
    g3d::ResourceLoader* shared = Func6DB618(g_Unknown9516C0.p4);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    g3d::ResourceLoader* textures = m_ResourceMgr.Get(m_TextureKey);
    Func4DBE3C(&m_Model, model, textures, shared, &m_Allocator);
}

// 0x0053379C (name is ours)
bool Fence::Load()
{
    sead::FixedSafeString<64> path;
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        if (m_IsVariantN) {
            formatter.append("Strc/hobj/fence/hobj_fence_%02d/hobj_fence_%02d_n.bcres");
        } else {
            formatter.append("Strc/hobj/fence/hobj_fence_%02d/hobj_fence_%02d_p.bcres");
        }
        formatter.appendValue(kind);
        formatter.appendValue(kind);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/fence/hobj_fence_%02d/Textures/%s.bcres");
        formatter.appendValue(kind);
        formatter.append(g_SeasonTextureFolders[Func630618(g_Unknown9516C0.p4)]);
        formatter.flush();
    }
    m_TextureKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_TextureKey.IsValid();
}

// 0x00533A58 (name is ours)
void Fence::Initialize(bool isVariantN, u8 kind)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x800, parent, "hobj::Fence::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_ResourceMgr.Initialize(parent, 0x7800);
    m_IsVariantN = isVariantN;
    m_Kind = kind;
}

// 0x00533AE4 (name is ours)
void Fence::AddToScene()
{
    m_Model.AddToScene(0);
}

// 0x00533AF0 (name is ours)
void Fence::UpdateMatrix(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.g3d::TransformNode::Update();
}

// 0x00533B0C (name is ours)
void Fence::Finalize()
{
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}

// 0x00533BDC (name is ours)
Fence::Fence() : m_ResourceMgr(), m_IsVariantN(false), m_Kind(0)
{
}

// 0x00533C5C (name is ours)
Fence::~Fence()
{
    // (only the members are destroyed)
}
} // namespace hobj
