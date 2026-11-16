#include "fgobj/dObjectDecoSwing.h"
#include "fgobj/dObjectSwing.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Globals.h"

namespace fgobj {
namespace {
// the skeletal animation of the decoration in FgObjAnim.bcres
const char* const DECO_SWING_ANIM_NAME = "fg_treeB_4";
// the fade: the frame of its start and the alpha step
const u8 FADE_START_FRAME = 10;
const u8 FADE_ALPHA_STEP = 25;
} // namespace

// 0x00595A24 (name is ours)
fgobj::ObjectDecoSwing* fgobj::ObjectDecoSwing::Create(const SvFgName& item, const s32* x, const s32* z, u8 animation)
{
    ObjectDecoSwing* deco = NewObject<ObjectDecoSwing>(s_LargeHeaps, LARGE_HEAP_NUM);
    if (deco == NULL) {
        return NULL;
    }
    deco->Setup(item, x, z, true);
    deco->m_IsCut = animation >= ObjectSwing::ANIMATION_CUT0_L;
    const void* anim = GetFgSkeletalAnim(DECO_SWING_ANIM_NAME, animation);
    deco->m_SkeletalAnim.Build(&deco->m_Model, nw::anim::res::ResAnim(anim), &deco->m_Allocator, 0);
    deco->m_Model.SetSkeletalAnim(&deco->m_SkeletalAnim);
    deco->m_FadeFrame = static_cast<u32>(deco->m_SkeletalAnim.GetEndFrame());
    return deco;
}

// 0x00595B48 slot 0x20 (name is ours)
s32 fgobj::ObjectDecoSwing::Execute()
{
    m_Model.UpdateAnims();
    UpdateMatrix(true, 0.0f);
    if (m_IsCut == true && m_FadeFrame != 0) {
        m_FadeFrame--;
        if (m_FadeFrame == FADE_START_FRAME) {
            m_Flags |= FLAG_ALPHA_STATE;
            m_Alpha = m_FadeFrame * FADE_ALPHA_STEP;
        } else if (m_FadeFrame < FADE_START_FRAME) {
            m_Alpha = m_FadeFrame * FADE_ALPHA_STEP;
        }
    }
    if (m_SkeletalAnim.IsFinished()) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    return EXECUTE_CONTINUE;
}

// 0x00595C28 slot 0x00
// 0x00595BD0 (deleting dtor)
fgobj::ObjectDecoSwing::~ObjectDecoSwing()
{
    // (only the members are destroyed)
}

} // namespace fgobj
