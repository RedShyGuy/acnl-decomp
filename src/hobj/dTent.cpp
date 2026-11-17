#include "hobj/dTent.h"

#include "Other/dHeapInit.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "hobj/hobj_Extern.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"

namespace hobj {
// 0x00532C58 slot 0x14 (name is ours)
void Tent::Update(const nn::math::VEC3* world, bool isPaused)
{
    if (!m_Anim.IsFinished()) {
        m_Model.UpdateAnims();
        if (!isPaused) {
            const f32 openFrame = m_Anim.GetEndFrame() * 0.5f + 1.0f;
            if (openFrame == m_Anim.GetFrame()) {
                m_Anim.SetSpeed(0.0f);
                if (m_OpenTimer == OPEN_WAIT_FRAMES) {
                    m_Anim.SetSpeed(-1.0f);
                }
                m_OpenTimer = m_OpenTimer + 1;
            } else if (static_cast<f32>(m_OpenTimer) > OPEN_WAIT_FRAMES) {
                if (!m_Sound.IsPlaying(SE_OBJ_DOOR_TENT_CLOSE)) {
                    m_Sound.StartSound(SE_OBJ_DOOR_TENT_CLOSE);
                    m_OpenTimer = 0;
                }
            }
        }
    }
    m_Sound.SetPosition(world);
}

// 0x00532D5C slot 0x0C (name is ours)
void Tent::Build(const void* houseData)
{
    g3d::ResourceLoader* shared = Func6DB618(g_Unknown9516C0.p4);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* textures = m_ResourceMgr.Get(m_TextureKey);
    textures->Read(0x10000, 0x10000, 0x10000);
    textures->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    model->InitializeCgfx(&m_Allocator, NULL, true);
    model->InitializeCgfx(&m_Allocator, textures, true);
    model->InitializeCgfx(&m_Allocator, shared, true);
    m_Model.Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), &m_Allocator, &m_Allocator, 0,
                   true, 1);
    m_Anim.Build(&m_Model, nw::anim::res::ResAnim(model->GetGraphicsFile().GetSkeletalAnims(0)), &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_Anim);
    m_Anim.SetFrame(m_Anim.GetEndFrame());
    m_Anim.SetSpeed(0.0f);
}

// 0x00532F80 slot 0x08 (name is ours)
bool Tent::Load()
{
    m_ModelKey = m_ResourceMgr.Load("Strc/hobj/tent/hobj_tent_01/hobj_tent_01.bcres");
    sead::FixedSafeString<128> path;
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/tent/hobj_tent_%02d/Textures/%s.bcres");
        formatter.appendValue(kind);
        formatter.append(g_SeasonTextureFolders[Func630618(g_Unknown9516C0.p4)]);
        formatter.flush();
    }
    m_TextureKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_TextureKey.IsValid();
}

// 0x0053313C (name is ours)
void Tent::Initialize(u8 kind, const nn::math::VEC3* world)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x1600, parent, "hobj::Tent::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    // (here the body of Initialize is inline, not its out-of-line copy)
    m_ResourceMgr.InitializeImpl(parent, 0xB400);
    m_Kind = kind;
    m_Sound.Start(world);
}

// 0x005332B4 slot 0x20 (name is ours)
void Tent::OpenDoor(bool isSecondAnim, netgame::PlayerNo player)
{
    if (m_Anim.m_pAnimObject == NULL) {
        return;
    }
    m_OpenTimer = 0;
    m_Anim.SetFrame(0.0f);
    m_Anim.SetSpeed(1.0f);
    if (netgame::GetMyPlayerNo() == player) {
        m_Sound.SetIsIgnoreMaster(true);
    }
    m_Sound.StartSound(SE_OBJ_DOOR_TENT_OPEN);
}

// 0x00533324 slot 0x28 (name is ours)
bool Tent::HoldDoorOpen(bool isSecondAnim)
{
    if (m_Anim.m_pAnimObject == NULL) {
        return false;
    }
    const f32 half = m_Anim.GetEndFrame() * 0.5f;
    if (m_Anim.GetFrame() > half) {
        m_Anim.SetSpeed(0.0f);
        return true;
    }
    return false;
}

// 0x0053338C slot 0x2C (name is ours)
void Tent::CloseDoor()
{
    if (m_Anim.m_pAnimObject == NULL) {
        return;
    }
    m_Anim.SetSpeed(-1.0f);
    m_Sound.StartSound(SE_OBJ_DOOR_TENT_CLOSE);
    m_Sound.SetIsIgnoreMaster(false);
}

// 0x005333D4 slot 0x34 (name is ours)
void Tent::vf_0x34()
{
    // empty (in the original too)
}

// 0x005333D8 slot 0x3C (name is ours)
void Tent::vf_0x3C()
{
    // empty (in the original too)
}

// 0x005333DC slot 0x38 (name is ours)
void Tent::vf_0x38()
{
    // empty (in the original too)
}

// 0x005333E0 slot 0x30 (name is ours)
void Tent::UpdateColor(const void* houseData)
{
    // empty (in the original too)
}

// 0x005333E4 slot 0x24 (name is ours)
void Tent::OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player, const u8* packedMelody)
{
    OpenDoor(isSecondAnim, player);
}

// 0x005333F0 slot 0x1C (name is ours)
void Tent::AddToScene()
{
    if (m_IsVisible) {
        m_Model.AddToScene(0);
    }
}

// 0x0053340C slot 0x18 (name is ours)
void Tent::UpdateMatrix(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.g3d::SkeletalModel::Update();
}

// 0x00533428 slot 0x10 (name is ours)
void Tent::Finalize()
{
    m_Sound.Stop();
    if (m_Anim.m_pAnimObject != NULL) {
        m_Anim.g3d::SkeletalAnim::Finalize();
    }
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}

// 0x00533510 (name is ours)
Tent::Tent() : m_ResourceMgr(), m_Kind(2), m_IsVisible(true), m_OpenTimer(0)
{
}

// 0x00533624 slot 0x00
// 0x005335B8 (deleting dtor)
Tent::~Tent()
{
    // (only the members are destroyed)
}
} // namespace hobj
