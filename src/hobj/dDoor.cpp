#include "hobj/dDoor.h"

#include "Other/dHeapInit.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "g3d/g3d_System.h"
#include "hobj/hobj_Extern.h"
#include "nw/gfx/gfx_Model.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "stage/stage_Api.h"

namespace hobj {
namespace {
// the frames of the two door animations: the end frame and the frame where an open door turns around
const f32 LONG_ANIM_END_FRAME = 57.0f;
const f32 LONG_ANIM_TURN_FRAME = 32.0f;
const f32 SHORT_ANIM_END_FRAME = 49.0f;
const f32 SHORT_ANIM_TURN_FRAME = 23.0f;

// whether g_Unknown9516C0 keeps the door open (mode 255 and its flag +0x6ACE)
inline bool IsKeptOpen()
{
    return static_cast<const u8*>(g_Unknown9516C0.p4)[0x6ACE] != 0;
}
} // namespace

// 0x00531B48 (name is ours)
Door::Door() : m_ResourceMgr(), m_Shape(0), m_Design(0)
{
}

// 0x00531BD8 (name is ours)
Door::~Door()
{
    // (only the members are destroyed)
}

inline u32 Door::GetOpenSound() const
{
    switch (m_Design) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 9:
    case 10:
    case 11:
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
        return SE_OBJ_DOOR_WOOD_OPEN;
    case 2:
    case 5:
    case 21:
        return SE_OBJ_DOOR_STEEL_OPEN;
    default:
        return SE_OBJ_DOOR_TRICK_OPEN;
    }
}

inline u32 Door::GetCloseSound() const
{
    switch (m_Design) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 9:
    case 10:
    case 11:
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
        return SE_OBJ_DOOR_WOOD_CLOSE;
    case 2:
    case 5:
    case 21:
        return SE_OBJ_DOOR_STEEL_CLOSE;
    default:
        return SE_OBJ_DOOR_TRICK_CLOSE;
    }
}

// 0x00530DC4 (name is ours)
void Door::Update(const nn::math::VEC3* world, bool isPaused)
{
    if (!m_Anim.IsFinished()) {
        m_Model.UpdateAnims();
        if (!isPaused) {
            const f32 endFrame = m_Anim.GetEndFrame();
            const f32 frame = m_Anim.GetFrame();
            if ((endFrame == LONG_ANIM_END_FRAME && frame == LONG_ANIM_TURN_FRAME) ||
                (endFrame == SHORT_ANIM_END_FRAME && frame == SHORT_ANIM_TURN_FRAME)) {
                m_Anim.SetSpeed(-1.0f);
                if (g_Unknown9516C0.mode == 255 && g_Unknown9516C0.p4 != NULL && IsKeptOpen() &&
                    m_Anim.GetEndFrame() == LONG_ANIM_END_FRAME) {
                    m_Anim.SetSpeed(0.0f);
                } else {
                    const u32 sound = GetCloseSound();
                    if (!m_Sound.IsPlaying(sound)) {
                        m_Sound.StartSound(sound);
                    }
                }
            }
        }
    }
    m_Sound.SetPosition(world);
}

// 0x00530FA8 (name is ours)
void Door::Build()
{
    g3d::ResourceLoader* shared = Func6DB618(g_Unknown9516C0.p4);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    g3d::ResourceLoader* textures = m_ResourceMgr.Get(m_TextureKey);
    model->Read(0x10000, 0x20000, 0x30000);
    model->InitializeCgfx(&m_Allocator, NULL, true);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    model->InitializeCgfx(&m_Allocator, shared, true);
    m_Model.Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), &m_Allocator, &m_Allocator, 0,
                   true, 1);
    textures->InitializeCgfx(&m_Allocator, NULL, true);
    const nw::gfx::res::ResTexture texture(textures->GetGraphicsFile().ptr()->textures.GetData(0));
    g3d::SetTexture(m_Model.GetModel()->GetResModel().FindMaterial("hobj_door_m"), "hobj_door_dummy", texture);
    g3d::ResourceLoader* anims = Func6DF40C(g_Unknown9516C0.p4);
    m_Anim.Build(&m_Model, nw::anim::res::ResAnim(anims->GetGraphicsFile().GetSkeletalAnims(0)), &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_Anim);
    m_Anim.SetFrame(m_Anim.GetEndFrame());
    m_Anim.SetSpeed(0.0f);
}

// 0x005312A4 (name is ours)
bool Door::Load()
{
    sead::FixedSafeString<64> path;
    {
        // 0x00866974 (initialization image)
        const char* const shapes[] = {"a", "b"};
        const u8 shape = m_Shape;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/door/hobj_door/hobj_door_%s.bcres");
        formatter.append(shapes[shape]);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        // 0x0086697C (initialization image)
        const char* const shapes[] = {"a", "b"};
        const u8 shape = m_Shape;
        const s32 design = m_Design;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/door/hobj_door/Textures/hobj_door_%02d_%s.bcres");
        formatter.appendValue(design);
        formatter.append(shapes[shape]);
        formatter.flush();
    }
    m_TextureKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_TextureKey.IsValid();
}

// 0x00531454 (name is ours)
void Door::Initialize(u8 shape, u8 design, const nn::math::VEC3* world)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x1400, parent, "hobj::Door::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_ResourceMgr.Initialize(parent, 0x4000);
    m_Shape = shape;
    m_Design = design;
    m_Sound.Start(world);
}

// 0x005314F8 (name is ours)
bool Door::HoldOpen(bool isSecondAnim)
{
    if (m_Anim.m_pAnimObject == NULL) {
        return false;
    }
    if (isSecondAnim) {
        if (m_Anim.GetFrame() <= 31.0f) {
            return false;
        }
        if (m_Anim.GetFrame() == m_Anim.GetEndFrame()) {
            return false;
        }
    } else {
        if (m_Anim.GetFrame() <= 22.0f) {
            return false;
        }
    }
    m_Anim.SetSpeed(0.0f);
    return true;
}

// 0x005315A8 (name is ours)
void Door::Open(bool isSecondAnim, netgame::PlayerNo player)
{
    if (m_Anim.m_pAnimObject == NULL) {
        return;
    }
    g3d::ResourceLoader* anims = isSecondAnim ? Func6DF244(g_Unknown9516C0.p4) : Func6DF40C(g_Unknown9516C0.p4);
    m_Anim.ChangeAnim(nw::anim::res::ResAnim(anims->GetGraphicsFile().GetSkeletalAnims(0)));
    m_Anim.SetFrame(0.0f);
    m_Anim.SetSpeed(1.0f);
    if (netgame::GetMyPlayerNo() == player) {
        m_Sound.SetIsIgnoreMaster(true);
    } else {
        m_Sound.SetIsIgnoreMaster(false);
    }
    if (!stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
        m_Sound.m_Obj.PlayDoorChime(SE_MELODY_ONESEQ_DOOR_HOME);
    }
    m_Sound.StartSound(GetOpenSound());
}

// 0x00531758 (name is ours)
void Door::Close()
{
    if (g_Unknown9516C0.mode != 255 || g_Unknown9516C0.p4 == NULL || m_Anim.m_pAnimObject == NULL || IsKeptOpen()) {
        return;
    }
    m_Anim.SetSpeed(-1.0f);
    const u32 sound = GetCloseSound();
    if (!m_Sound.IsPlaying(sound)) {
        m_Sound.StartSound(sound);
    }
}

// 0x005318A8 (name is ours)
void Door::OpenWithChime(bool isSecondAnim, const u8* packedMelody, netgame::PlayerNo player)
{
    if (m_Anim.m_pAnimObject == NULL) {
        return;
    }
    g3d::ResourceLoader* anims = isSecondAnim ? Func6DF244(g_Unknown9516C0.p4) : Func6DF40C(g_Unknown9516C0.p4);
    m_Anim.ChangeAnim(nw::anim::res::ResAnim(anims->GetGraphicsFile().GetSkeletalAnims(0)));
    m_Anim.SetFrame(0.0f);
    m_Anim.SetSpeed(1.0f);
    if (netgame::GetMyPlayerNo() == player) {
        m_Sound.SetIsIgnoreMaster(true);
    } else {
        m_Sound.SetIsIgnoreMaster(false);
    }
    m_Sound.PlayDoorChime(SE_MELODY_ONESEQ_DOOR_HOME, packedMelody);
    m_Sound.StartSound(GetOpenSound());
}

// 0x00531A38 (name is ours)
void Door::AddToScene()
{
    m_Model.AddToScene(0);
}

// 0x00531A44 (name is ours)
void Door::UpdateMatrix(const nn::math::MTX34& matrix)
{
    m_Model.SetMatrix(matrix);
    m_Model.g3d::SkeletalModel::Update();
}

// 0x00531A60 (name is ours)
void Door::Finalize()
{
    m_Sound.Stop();
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    if (m_Anim.m_pAnimObject != NULL) {
        m_Anim.g3d::SkeletalAnim::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}
} // namespace hobj
