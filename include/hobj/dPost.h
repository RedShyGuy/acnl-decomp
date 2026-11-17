#pragma once

#include "decomp.h"
#include "Audio/dAudioObj1.h"
#include "g3d/dMaterialAnim.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "mutil/dResourceLoaderMgr.h"
#include "nw/anim/res/anim_ResAnim.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"

// The mailbox in front of a house (class name after the heap name "hobj::Post::Allocator"; the animation names are from
// the model file, the rest is ours; no vtable). The model "Strc/hobj/post/hobj_post_<kind>" with the textures of the
// season; the flag goes up for new mail (skeletal animation "hobj_post_newmail", then the blinking material animation
// "hobj_post_brink"), the lid opens ("hobj_post_open"), a full mailbox has its own animations ("hobj_post_max",
// "hobj_post_brink_max"); the sounds are SE_OBJ_MAIL_BOX_*.
namespace hobj {
class Post
{
public:
    // the state of the mailbox (m_State; also the arguments of the Update* functions)
    enum State : u8
    {
        STATE_NO_MAIL = 0,   // flag down
        STATE_NEW_MAIL = 1,  // flag up and blinking
        STATE_OPEN = 2,      // the lid opens / is open
        STATE_FULL = 3,      // full mailbox
        STATE_RAISING = 4,   // the flag goes up
        STATE_CLOSING = 5,   // the lid closes
    };

    Post(); // 0x00532B34
    ~Post(); // 0x00532BEC

    // the heaps (in the structure heap of HeapInit) for the mailbox of kind
    void Initialize(u8 kind); // 0x005323CC
    // the files of the model and of its textures are loaded (true when both have a key)
    bool Load(); // 0x005321AC
    // the model, its animations (stopped) and the sounds at world
    void Build(const nn::math::VEC3* world); // 0x00531C34
    void Finalize(); // 0x00532838
    void AddToScene(); // 0x005325B4
    // the animations, then the world matrix
    void UpdateMatrix(const nn::math::MTX34& matrix); // 0x005325D0
    // new mail (STATE_NEW_MAIL) or none (STATE_NO_MAIL): the flag goes up / down; the alert sounds in the middle of
    // the blinking
    void UpdateNewMail(u32 state); // 0x005325FC
    // a full mailbox (STATE_FULL); the alert sounds at frame 8 of the blinking
    void UpdateFull(u32 state); // 0x005327B0
    // the lid opens (STATE_OPEN) or closes (STATE_NO_MAIL, STATE_NEW_MAIL), with the open/close sounds
    void UpdateOpen(u32 state); // 0x00532938
    // whether the open animation has stopped at its end (the lid is open)
    bool IsOpened(); // 0x00532450
    // whether the open animation has stopped at frame 0 (the lid is closed)
    bool IsClosed(); // 0x005324B8
    // the frame of the blinking (-1.0f: none)
    f32 GetBlinkFrame(); // 0x0053251C
    // (the same code as IsOpened)
    bool IsOpenFinished(); // 0x0053254C
    // (inline; the modules call the copies) whether the model is built
    DECOMP_EMIT_COPY bool IsBuilt() const { return m_Model.GetNode() != NULL; } // 0x0074914C
    // whether no animation runs: full, lid closed or flag up
    DECOMP_EMIT_COPY bool IsResting(); // 0x00749098

private:
    // (inline) whether the skeletal animation is the open animation (same last frame)
    bool IsOpenAnim();

public:
    ssys::ma::HeapAllocator m_Allocator;                          // 0x000
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2> m_ResourceMgr; // 0x008
    g3d::SkeletalModel m_Model;                                   // 0x26C
    g3d::MaterialAnim m_MaterialAnim;                             // 0x278
    g3d::SkeletalAnim m_Anim;                                     // 0x298
    const void* m_pNewMailAnim;                                   // 0x2B0 nw::anim::res::ResAnimData
    const void* m_pOpenAnim;                                      // 0x2B4
    const void* m_pMaxAnim;                                       // 0x2B8
    const void* m_pBlinkAnim;                                     // 0x2BC "hobj_post_brink"
    const void* m_pBlinkMaxAnim;                                  // 0x2C0 "hobj_post_brink_max"
    u8 m_Kind;                                                    // 0x2C4
    mutil::ResourceKey m_ModelKey;                                // 0x2C8
    mutil::ResourceKey m_TextureKey;                              // 0x2D0
    u8 m_State;                                                   // 0x2D8 State
    AudioObj1 m_Sound;                                            // 0x2DC flag and alert
    AudioObj1 m_LidSound;                                         // 0x33C open and close
    bool m_IsVisible;                                             // 0x39C
};
ASSERT_SIZE(Post, 0x3A0);

inline bool Post::IsOpenAnim()
{
    return nw::anim::res::ResAnim(m_pOpenAnim).ptr()->frameSize == m_Anim.GetEndFrame();
}

inline bool Post::IsResting()
{
    if (m_State == STATE_FULL) {
        return true;
    }
    if (m_Anim.IsFinished() && m_Anim.GetFrame() == 0.0f && IsOpenAnim()) {
        return true;
    }
    return m_Anim.GetFrame() == m_Anim.GetEndFrame() &&
           nw::anim::res::ResAnim(m_pNewMailAnim).ptr()->frameSize == m_Anim.GetEndFrame();
}
} // namespace hobj
