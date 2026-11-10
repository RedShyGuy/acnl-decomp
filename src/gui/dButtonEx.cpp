#include "gui/dButtonEx.h"

#include "Other/dController.h"

namespace gui {
// 0x00AD7618
s32 ButtonEx::s_DefaultSounds[SOUND_NUM] = {-1, -1, -1, -1, -1};

// 0x004FEFF0
bool ButtonEx::IsAnimationDone(s32 idx)
{
    lyt::AnmObject* anim = GetAnimation(idx);
    lyt::AnmObject** subAnims = GetSubAnimations(idx);
    bool isDone = anim == NULL || anim->m_State == lyt::AnmObject::STATE_STOPPING || anim->m_State != lyt::AnmObject::STATE_PLAYING;
    for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
        lyt::AnmObject* subAnim = subAnims[i];
        if (subAnim != NULL) {
            isDone &= subAnim->m_State == lyt::AnmObject::STATE_STOPPING || subAnim->m_State != lyt::AnmObject::STATE_PLAYING;
        }
    }
    return isDone;
}

// 0x004FF07C
lyt::AnmObject** ButtonEx::GetSubAnimations(s32 idx)
{
    switch (idx) {
    case 0:
        return m_Desc.subAnims[0];
    case 1:
        return m_Desc.subAnims[1];
    case 2:
        return m_Desc.subAnims[2];
    case 3:
        return m_Desc.subAnims[3];
    case 4:
        return m_Desc.subAnims[4];
    case 5:
        return m_Desc.subAnims[5];
    case 6:
        return m_Desc.subAnims[6];
    case 7:
        return m_Desc.subAnims[7];
    default:
        return NULL;
    }
}

// 0x004FF0F0 slot 0x28
void ButtonEx::UpdateIdle()
{
    if (m_IsHolding) {
        m_IsHolding = false;
    }
    if (!m_IsReset) {
        Reset(false);
        m_IsReset = true;
    }
    if (Controller::IsTrig(Controller::TOUCH) && IsTouched()) {
        PlayAnimation(ANIM_PRESS);
        PlaySound(SOUND_PRESS);
        m_IsHolding = true;
        g_IsWidgetHolding = true;
        m_State = STATE_PRESSED;
    }
}

// 0x004FF198 slot 0x30
void ButtonEx::UpdateDecided()
{
    if (IsAnimationDone(ANIM_DECIDE)) {
        m_State = STATE_IDLE;
    }
}

// 0x004FF1B8 slot 0x38
void ButtonEx::UpdateDisabled()
{
    if (Controller::IsTrig(Controller::TOUCH) && IsTouched()) {
        PlaySound(SOUND_DISABLED);
    }
}

// 0x004FF204
void ButtonEx::ApplyAnimationLimit(s32 idx, bool isStart)
{
    lyt::AnmObject* anim = GetAnimation(idx);
    lyt::AnmObject** subAnims = GetSubAnimations(idx);
    if (anim != NULL) {
        detail::ApplyLimit(anim, isStart);
    }
    for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
        if (subAnims[i] != NULL) {
            detail::ApplyLimit(subAnims[i], isStart);
        }
    }
}

// 0x004FF290 slot 0x2C
void ButtonEx::UpdatePressed()
{
    if (IsTouched()) {
        if (Controller::IsRelease(Controller::TOUCH)) {
            PlayAnimation(ANIM_DECIDE);
            PlaySound(SOUND_DECIDE);
            m_State = STATE_DECIDED;
            SendEvent(this, EVENT_DECIDE, 0);
        }
    } else {
        PlayAnimation(ANIM_SLIDE_OUT);
        PlaySound(SOUND_SLIDE_OUT);
        m_State = STATE_SLID_OUT;
    }
}

// 0x004FF33C slot 0x34
void ButtonEx::UpdateSlidOut()
{
    lyt::AnmObject* anim = m_Desc.anims[ANIM_SLIDE_OUT];
    if (anim == NULL || anim->m_State == lyt::AnmObject::STATE_STOPPING) {
        m_State = STATE_IDLE;
    }
    if (!Controller::IsHold(Controller::TOUCH)) {
        m_State = STATE_IDLE;
    } else if (IsTouched()) {
        PlayAnimation(ANIM_PRESS);
        PlaySound(SOUND_PRESS_AGAIN);
        m_State = STATE_PRESSED;
    }
}

// 0x004FF3B4
ButtonEx::Description::Description()
{
    pLayout = NULL;
    groupName = NULL;
    for (s32 i = 0; i < ANIM_NUM; i++) {
        anims[i] = NULL;
    }
    for (s32 i = 0; i < SOUND_NUM; i++) {
        sounds[i] = SOUND_DEFAULT;
    }
    m_Unknown0x30 = -1;
    keys = 0;
    for (s32 i = 0; i < ANIM_EX_NUM - ANIM_NUM; i++) {
        exAnims[i] = NULL;
    }
    pKeyLayout = NULL;
    for (s32 i = 0; i < ANIM_EX_NUM; i++) {
        for (s32 j = 0; j < SUB_ANIM_NUM; j++) {
            subAnims[i][j] = NULL;
        }
    }
}

// 0x004FF4A8 slot 0x1C
void ButtonEx::Reset(bool isRelease)
{
    if (m_State == STATE_DISABLED) {
        return;
    }
    StopAllAnimations();
    if (isRelease) {
        PlayAnimation(ANIM_RELEASE);
    } else {
        ApplyAnimationLimit(ANIM_PRESS, true);
    }
    m_State = STATE_IDLE;
}

// 0x004FF530 slot 0x08
void ButtonEx::Update()
{
    if (!m_IsActive) {
        return;
    }
    // another widget has the touch
    if (!m_IsHolding && g_IsWidgetHolding) {
        return;
    }
    if (m_State != STATE_IDLE && m_IsReset) {
        m_IsReset = false;
    }
    switch (m_State) {
    case STATE_IDLE:
        UpdateIdle();
        break;
    case STATE_PRESSED:
        UpdatePressed();
        break;
    case STATE_DECIDED:
        UpdateDecided();
        break;
    case STATE_SLID_OUT:
        UpdateSlidOut();
        break;
    case STATE_LOCKED:
        break;
    case STATE_DISABLED:
        UpdateDisabled();
        break;
    }
}

// 0x004FF5DC
lyt::AnmObject* ButtonEx::GetAnimation(s32 idx)
{
    switch (idx) {
    case ANIM_PRESS:
        return m_Desc.anims[ANIM_PRESS];
    case ANIM_SLIDE_OUT:
        return m_Desc.anims[ANIM_SLIDE_OUT];
    case ANIM_DECIDE:
        return m_Desc.anims[ANIM_DECIDE];
    case ANIM_RELEASE:
        return m_Desc.anims[ANIM_RELEASE];
    case ANIM_DISABLE:
        return m_Desc.anims[ANIM_DISABLE];
    case ANIM_EX_5:
        return m_Desc.exAnims[0];
    case ANIM_EX_6:
        return m_Desc.exAnims[1];
    case ANIM_EX_7:
        return m_Desc.exAnims[2];
    default:
        return NULL;
    }
}

// 0x004FF65C slot 0x10
void ButtonEx::SetEnabled(bool enabled, bool playAnimation, bool /*keepAnimation*/)
{
    if (enabled) {
        if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            ApplyAnimationLimit(ANIM_PRESS, true);
            m_State = STATE_IDLE;
        }
    } else {
        if (playAnimation) {
            StopAllAnimations();
            // (the disabling animation is played backwards)
            lyt::AnmObject* anim = GetAnimation(ANIM_DISABLE);
            lyt::AnmObject** subAnims = GetSubAnimations(ANIM_DISABLE);
            if (anim != NULL) {
                detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
            }
            for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
                if (subAnims[i] != NULL) {
                    detail::PlayOnce(subAnims[i], lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
                }
            }
        } else if (m_State != STATE_LOCKED) {
            StopAllAnimations();
            ApplyAnimationLimit(ANIM_PRESS, true);
        }
        if (m_State != STATE_LOCKED) {
            m_State = STATE_DISABLED;
        }
    }
    m_IsHolding = false;
}

// 0x004FF810
void ButtonEx::PlayAnimation(s32 idx)
{
    lyt::AnmObject* anim = GetAnimation(idx);
    lyt::AnmObject** subAnims = GetSubAnimations(idx);
    if (anim != NULL) {
        detail::PlayOnce(anim, lyt::AnmObject::PLAY_MODE_ONCE);
    } else if (idx == ANIM_SLIDE_OUT) {
        // the press is undone
        lyt::AnmObject* pressAnim = GetAnimation(ANIM_PRESS);
        if (pressAnim != NULL) {
            detail::PlayOnce(pressAnim, lyt::AnmObject::PLAY_MODE_ONCE_REVERSE);
        }
    }
    if (idx != ANIM_SLIDE_OUT && idx != ANIM_DECIDE && idx != ANIM_RELEASE) {
        for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
            if (subAnims[i] != NULL) {
                detail::PlayOnce(subAnims[i], lyt::AnmObject::PLAY_MODE_ONCE);
            }
        }
        return;
    }
    // without sub animations of its own, those of the press are played backwards
    s32 playMode = lyt::AnmObject::PLAY_MODE_ONCE_REVERSE;
    bool hasSubAnims = false;
    for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
        if (subAnims[i] != NULL) {
            hasSubAnims = true;
            break;
        }
    }
    if (hasSubAnims) {
        playMode = lyt::AnmObject::PLAY_MODE_ONCE;
    } else {
        subAnims = GetSubAnimations(ANIM_PRESS);
    }
    for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
        if (subAnims[i] != NULL) {
            detail::PlayOnce(subAnims[i], playMode);
        }
    }
}

// 0x004FF988
void ButtonEx::StopAnimation(s32 idx)
{
    lyt::AnmObject* anim = GetAnimation(idx);
    lyt::AnmObject** subAnims = GetSubAnimations(idx);
    if (anim != NULL) {
        detail::StopIfPlaying(anim);
    }
    for (s32 i = 0; i < SUB_ANIM_NUM; i++) {
        if (subAnims[i] != NULL) {
            detail::StopIfPlaying(subAnims[i]);
        }
    }
}

// 0x004FFA10
ButtonEx::ButtonEx(const Description& desc) : m_Desc(desc), m_IsReset(true)
{
    SetDescription(&m_Desc, s_DefaultSounds);
}

// 0x004FFAEC slot 0x00
// 0x004FFABC (deleting dtor)
ButtonEx::~ButtonEx()
{
    // (only the base is destroyed)
}
} // namespace gui
