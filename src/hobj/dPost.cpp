#include "hobj/dPost.h"

#include "Other/dHeapInit.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "hobj/hobj_Extern.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "script/dScriptExtern.h"
#include "Bs/dBsStrcMgr.h"

namespace hobj {
namespace {
// the last frame of an animation resource
inline f32 GetFrameSize(const void* anim)
{
    return nw::anim::res::ResAnim(anim).ptr()->frameSize;
}
} // namespace

// 0x00531C34 (name is ours)
void Post::Build(const nn::math::VEC3* world)
{
    g3d::ResourceLoader* shared = BsStrcMgr::s_pInstance->GetHouseLut();
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* textures = m_ResourceMgr.Get(m_TextureKey);
    textures->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    model->InitializeCgfx(&m_Allocator, NULL, true);
    model->InitializeCgfx(&m_Allocator, shared, true);
    model->InitializeCgfx(&m_Allocator, textures, true);
    m_Model.Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), &m_Allocator, &m_Allocator, 0,
                   true, 1);
    model = m_ResourceMgr.Get(m_ModelKey);
    // (armlink replaced the calls of a tiny function that decides whether the animations play by "mov r0, #0")
    const bool isPlaying = false;
    m_pBlinkAnim = model->GetGraphicsFile().GetTextureAnims("hobj_post_brink");
    m_MaterialAnim.Build(&m_Model, nw::anim::res::ResAnim(m_pBlinkAnim), &m_Allocator, false);
    m_Model.SetMaterialAnim(&m_MaterialAnim, 0);
    m_MaterialAnim.SetSpeed(isPlaying ? 1.0f : 0.0f);
    m_pBlinkMaxAnim = model->GetGraphicsFile().GetTextureAnims("hobj_post_brink_max");
    m_pNewMailAnim = model->GetGraphicsFile().ptr()->skeletalAnims.GetData("hobj_post_newmail");
    m_pOpenAnim = model->GetGraphicsFile().ptr()->skeletalAnims.GetData("hobj_post_open");
    m_pMaxAnim = model->GetGraphicsFile().ptr()->skeletalAnims.GetData("hobj_post_max");
    m_Anim.Build(&m_Model, nw::anim::res::ResAnim(m_pNewMailAnim), &m_Allocator, 0);
    m_Model.SetSkeletalAnim(&m_Anim);
    m_Anim.SetSpeed(isPlaying ? 1.0f : 0.0f);
    m_Sound.Start(world);
    m_LidSound.Start(world);
    if (Func5C3750()) {
        m_LidSound.SetIsIgnoreMaster(true);
    } else {
        m_LidSound.SetIsIgnoreMaster(false);
    }
}

// 0x005321AC (name is ours)
bool Post::Load()
{
    sead::FixedSafeString<64> path;
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/post/hobj_post_%02d/hobj_post_%02d.bcres");
        formatter.appendValue(kind);
        formatter.appendValue(kind);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/post/hobj_post_%02d/Textures/%s.bcres");
        formatter.appendValue(kind);
        formatter.append(g_SeasonTextureFolders[Func630618(BsStrcMgr::s_pInstance)]);
        formatter.flush();
    }
    m_TextureKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_TextureKey.IsValid();
}

// 0x005323CC (name is ours)
void Post::Initialize(u8 kind)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x2000, parent, "hobj::Post::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_ResourceMgr.Initialize(parent, 0x5400);
    m_Kind = kind;
}

// 0x00532450 (name is ours)
bool Post::IsOpened()
{
    return m_Anim.IsFinished() && m_Anim.GetFrame() == m_Anim.GetEndFrame() && IsOpenAnim();
}

// 0x005324B8 (name is ours)
bool Post::IsClosed()
{
    return m_Anim.IsFinished() && m_Anim.GetFrame() == 0.0f && IsOpenAnim();
}

// 0x0053251C (name is ours)
f32 Post::GetBlinkFrame()
{
    if (m_MaterialAnim.IsFinished()) {
        return -1.0f;
    }
    return m_MaterialAnim.GetFrame();
}

// 0x0053254C (name is ours)
bool Post::IsOpenFinished()
{
    return m_Anim.IsFinished() && m_Anim.GetFrame() == m_Anim.GetEndFrame() && IsOpenAnim();
}

// 0x005325B4 (name is ours)
void Post::AddToScene()
{
    if (m_IsVisible) {
        m_Model.AddToScene(0);
    }
}

// 0x005325D0 (name is ours)
void Post::UpdateMatrix(const nn::math::MTX34& matrix)
{
    m_Model.EvaluateAnims();
    m_Model.SetMatrix(matrix);
    m_Model.g3d::SkeletalModel::Update();
}

// 0x005325FC (name is ours)
void Post::UpdateNewMail(u32 state)
{
    const u32 current = m_State;
    if (current != STATE_OPEN && current != STATE_CLOSING && current != state) {
        if (state == STATE_NO_MAIL) {
            if (current == STATE_NEW_MAIL) {
                m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pNewMailAnim));
                m_Anim.SetSpeed(-1.0f);
                m_Anim.SetFrame(m_Anim.GetEndFrame());
                m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(m_pBlinkAnim));
                m_MaterialAnim.SetSpeed(0.0f);
                m_MaterialAnim.SetFrame(0.0f);
                m_State = STATE_NO_MAIL;
                m_Sound.StartSound(SE_OBJ_MAIL_BOX_SIGN_DOWN);
            }
        } else if (state == STATE_NEW_MAIL) {
            if (current == STATE_NO_MAIL) {
                m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pNewMailAnim));
                m_Anim.SetSpeed(1.0f);
                m_State = STATE_RAISING;
                m_Sound.StartSound(SE_OBJ_MAIL_BOX_SIGN_UP);
            } else if (current == STATE_RAISING) {
                if (m_Anim.IsFinished()) {
                    m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(m_pBlinkAnim));
                    m_MaterialAnim.SetSpeed(1.0f);
                    m_MaterialAnim.SetFrame(0.0f);
                    m_State = STATE_NEW_MAIL;
                }
            }
        }
    }
    if (!m_MaterialAnim.IsFinished() && m_MaterialAnim.GetEndFrame() * 0.5f == m_MaterialAnim.GetFrame()) {
        m_Sound.StartSound(SE_OBJ_MAIL_BOX_ALERT);
    }
}

// 0x005327B0 (name is ours)
void Post::UpdateFull(u32 state)
{
    if (state == STATE_FULL && m_State != STATE_FULL) {
        m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pMaxAnim));
        m_Anim.SetSpeed(1.0f);
        m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(m_pBlinkMaxAnim));
        m_MaterialAnim.SetFrame(0.0f);
        m_State = STATE_FULL;
    }
    if (m_MaterialAnim.GetFrame() == 8.0f) {
        m_Sound.StartSound(SE_OBJ_MAIL_BOX_ALERT);
    }
}

// 0x00532938 (name is ours)
void Post::UpdateOpen(u32 state)
{
    const f32 endFrame = m_Anim.GetEndFrame();
    if (GetFrameSize(m_pOpenAnim) == endFrame) {
        const f32 frame = m_Anim.GetFrame();
        if (m_Anim.GetSpeed() == -1.0f && frame == endFrame) {
            m_LidSound.StartSound(SE_OBJ_MAIL_BOX_CLOSE);
        } else if (m_Anim.GetSpeed() == 1.0f && frame == 15.0f) {
            m_LidSound.StartSound(SE_OBJ_MAIL_BOX_OPEN);
        }
    }
    const u32 current = m_State;
    if (current == state) {
        return;
    }
    if (current == STATE_OPEN) {
        if (state == STATE_NEW_MAIL || state == STATE_NO_MAIL) {
            m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pOpenAnim));
            m_Anim.SetSpeed(-1.0f);
            m_Anim.SetFrame(m_Anim.GetEndFrame());
            m_State = STATE_CLOSING;
            return;
        }
    } else if (current == STATE_CLOSING) {
        if (m_Anim.IsFinished()) {
            if (m_MaterialAnim.GetEndFrame() == GetFrameSize(m_pMaxAnim)) {
                m_MaterialAnim.ChangeAnim(nw::anim::res::ResAnim(m_pBlinkAnim));
                m_MaterialAnim.SetSpeed(1.0f);
                m_MaterialAnim.SetFrame(0.0f);
            }
            m_State = STATE_NEW_MAIL;
        }
        return;
    } else if (current == STATE_NEW_MAIL) {
        if (state == STATE_OPEN) {
            m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pOpenAnim));
            m_Anim.SetFrame(0.0f);
            m_Anim.SetSpeed(1.0f);
            m_State = STATE_OPEN;
        }
        return;
    }
    if (state == STATE_OPEN) {
        m_Anim.ChangeAnim(nw::anim::res::ResAnim(m_pOpenAnim));
        m_Anim.SetSpeed(1.0f);
        m_Anim.SetFrame(0.0f);
        m_State = STATE_OPEN;
    }
}

// 0x00532838 (name is ours)
void Post::Finalize()
{
    m_Sound.Stop();
    m_LidSound.Stop();
    if (m_MaterialAnim.m_pAnimObject != NULL) {
        m_MaterialAnim.g3d::MaterialAnim::Finalize();
    }
    if (m_Anim.m_pAnimObject != NULL) {
        m_Anim.g3d::SkeletalAnim::Finalize();
    }
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}

// 0x00532B34 (name is ours)
Post::Post()
    : m_pNewMailAnim(NULL), m_pOpenAnim(NULL), m_pMaxAnim(NULL), m_pBlinkAnim(NULL),
      m_pBlinkMaxAnim(NULL), m_Kind(22), m_State(STATE_NO_MAIL), m_IsVisible(true)
{
}

// 0x00532BEC (name is ours)
Post::~Post()
{
    // (only the members are destroyed)
}
} // namespace hobj
