#include "sead/seadControllerBase.h"
#include "sead/seadMathCalcCommon.h"

#include <cfloat>
#include <climits>
#include <cmath>
#include <cstring>

namespace sead {
// (the static initializer sets both; GCC initializes them constantly)
// 0x0079CA24 (static initializer)
// 0x009762E4
const Vector2<f32> ControllerBase::s_InvalidPointer(-FLT_MAX, -FLT_MAX);
// 0x009762EC
const Vector2<s32> ControllerBase::s_InvalidPointerS32(INT_MIN, INT_MIN);

// 0x005440BC (name is ours)
bool sead::ControllerBase::isIdleBase_() const
{
    return mPadHold == 0 && (mPointerFlag & POINTER_ON) == 0 && mLeftStick.x == Vector2<f32>::zero.x &&
           mLeftStick.y == Vector2<f32>::zero.y && mRightStick.x == Vector2<f32>::zero.x &&
           mRightStick.y == Vector2<f32>::zero.y && mLeftAnalogTrigger == 0.0f && mRightAnalogTrigger == 0.0f;
}

// 0x00544160 (name is ours)
void sead::ControllerBase::setIdleBase_()
{
    mPadHold = 0;
    mPadTrig = 0;
    mPadRelease = 0;
    mPadRepeat = 0;
    mPointerFlag = 0;
    for (s32 i = 0; i < mPadBitMax; i++) {
        mPadHoldCounts[i] = 0;
    }
    mPointer = s_InvalidPointer;
    mPointerS32 = s_InvalidPointerS32;
    mLeftStick.x = 0.0f;
    mLeftStick.y = 0.0f;
    mRightStick.x = 0.0f;
    mRightStick.y = 0.0f;
    mLeftAnalogTrigger = 0.0f;
    mRightAnalogTrigger = 0.0f;
}

// 0x00544224 (name is ours)
void sead::ControllerBase::setPadRepeat(u32 mask, u8 delayFrame, u8 pulseFrame)
{
    for (s32 i = 0; i < mPadBitMax; i++) {
        if (mask & (1 << i)) {
            mPadRepeatDelays[i] = delayFrame;
            mPadRepeatPulses[i] = pulseFrame;
        }
    }
}

// 0x00544264 | nintendogs:bytes [tier A]
u32 sead::ControllerBase::getStickHold_(u32 prevHold, const Vector2<f32>& stick, f32 holdThreshold,
                                        f32 releaseThreshold, s32 startBit)
{
    f32 length = sqrtf(stick.x * stick.x + stick.y * stick.y);
    if (length < releaseThreshold) {
        return 0;
    }
    u32 up = 1 << startBit;
    u32 down = 1 << (startBit + 1);
    u32 left = 1 << (startBit + 2);
    u32 right = 1 << (startBit + 3);
    if (holdThreshold > length && (prevHold & (up | down | left | right)) == 0) {
        return 0;
    }
    // eight directions of 45 degrees, right at 0
    u32 angle = MathCommon<f32>::atan2Idx(stick.y, stick.x);
    if (angle < 0x10000000) {
        return right;
    }
    if (angle < 0x30000000) {
        return right | up;
    }
    if (angle < 0x50000000) {
        return up;
    }
    if (angle < 0x70000000) {
        return left | up;
    }
    if (angle < 0x90000000) {
        return left;
    }
    if (angle < 0xB0000000) {
        return left | down;
    }
    if (angle < 0xD0000000) {
        return down;
    }
    if (angle < 0xF0000000) {
        return right | down;
    }
    return right;
}

// 0x00544444 (name is ours)
void sead::ControllerBase::setPointerWithBound_(bool isOn, bool isTouchKeyHold, const Vector2<f32>& pos)
{
    if (isOn) {
        if (mPointerBound.mMin.x > mPointerBound.mMax.x || mPointerBound.mMin.y > mPointerBound.mMax.y) {
            mPointer = pos;
        } else if (mPointerBound.mMin.x <= pos.x && pos.x <= mPointerBound.mMax.x &&
                   mPointerBound.mMin.y <= pos.y && pos.y <= mPointerBound.mMax.y) {
            mPointer.x = pos.x - mPointerBound.mMin.x;
            mPointer.y = pos.y - mPointerBound.mMin.y;
        } else {
            isOn = false;
        }
    }
    if (isOn) {
        mPointerFlag |= POINTER_ON;
    } else {
        mPointerFlag &= ~POINTER_ON;
    }
    if (mTouchKeyBit >= 0) {
        if (isOn && isTouchKeyHold) {
            mPadHold |= 1 << mTouchKeyBit;
        } else {
            mPadHold &= ~(1 << mTouchKeyBit);
        }
    }
    if (mPointerFlag & POINTER_SET_INVALID_ON_OFF) {
        if ((mPointerFlag & POINTER_ON) == 0) {
            mPointer = s_InvalidPointer;
        }
        mPointerFlag &= ~POINTER_SET_INVALID_ON_OFF;
    }
}

// 0x00544554 | nintendogs:callseq [tier A]
void sead::ControllerBase::updateDerivativeParams_(u32 prevHold, bool prevPointerOn)
{
    u32 stickHold = 0;
    if (mLeftStickCrossStartBit >= 0) {
        stickHold = getStickHold_(prevHold, mLeftStick, mLeftStickHoldThreshold, mLeftStickReleaseThreshold,
                                  mLeftStickCrossStartBit);
    }
    if (mRightStickCrossStartBit >= 0) {
        stickHold |= getStickHold_(prevHold, mRightStick, mRightStickHoldThreshold, mRightStickReleaseThreshold,
                                   mRightStickCrossStartBit);
    }
    u32 stickMask = 0;
    if (mLeftStickCrossStartBit >= 0) {
        stickMask = 1 << mLeftStickCrossStartBit | 1 << (mLeftStickCrossStartBit + 1) |
                    1 << (mLeftStickCrossStartBit + 2) | 1 << (mLeftStickCrossStartBit + 3);
    }
    if (mRightStickCrossStartBit >= 0) {
        stickMask |= 1 << mRightStickCrossStartBit | 1 << (mRightStickCrossStartBit + 1) |
                     1 << (mRightStickCrossStartBit + 2) | 1 << (mRightStickCrossStartBit + 3);
    }
    mPadHold = (mPadHold & ~stickMask) | stickHold;
    mPadTrig = mPadHold & ~prevHold;
    mPadRelease = prevHold & ~mPadHold;
    mPadRepeat = 0;
    for (s32 i = 0; i < mPadBitMax; i++) {
        u32 bit = 1 << i;
        if ((mPadHold & bit) == 0) {
            mPadHoldCounts[i] = 0;
            continue;
        }
        if (mPadRepeatPulses[i] != 0) {
            u32 delay = mPadRepeatDelays[i];
            if (mPadHoldCounts[i] == delay) {
                mPadRepeat |= bit;
            } else if (delay < mPadHoldCounts[i] && (mPadHoldCounts[i] - delay) % mPadRepeatPulses[i] == 0) {
                mPadRepeat |= bit;
            }
        }
        mPadHoldCounts[i]++;
    }
    if (!prevPointerOn && (mPointerFlag & POINTER_ON) != 0) {
        mPointerFlag |= POINTER_ON_NOW;
    } else {
        mPointerFlag &= ~POINTER_ON_NOW;
    }
    if (prevPointerOn && (mPointerFlag & POINTER_ON) == 0) {
        mPointerFlag |= POINTER_OFF_NOW;
    } else {
        mPointerFlag &= ~POINTER_OFF_NOW;
    }
    mPointerS32.x = static_cast<s32>(mPointer.x);
    mPointerS32.y = static_cast<s32>(mPointer.y);
}

// 0x0054471C | nintendogs:callseq [tier A]
sead::ControllerBase::ControllerBase(s32 padBitMax, s32 leftStickCrossStartBit, s32 rightStickCrossStartBit,
                                     s32 touchKeyBit)
    : mPadTrig(0), mPadRelease(0), mPadRepeat(0), mPointerFlag(0), mPointerS32(s_InvalidPointerS32),
      mPointerBound(), mLeftStickHoldThreshold(0.5f), mRightStickHoldThreshold(0.5f),
      mLeftStickReleaseThreshold(0.25f), mRightStickReleaseThreshold(0.25f), mPadBitMax(padBitMax),
      mLeftStickCrossStartBit(leftStickCrossStartBit), mRightStickCrossStartBit(rightStickCrossStartBit),
      mTouchKeyBit(touchKeyBit), mIdleFrame(0), mPadHold(0), mPointer(s_InvalidPointer), mLeftStick(0.0f, 0.0f),
      mRightStick(0.0f, 0.0f), mLeftAnalogTrigger(0.0f), mRightAnalogTrigger(0.0f)
{
    if (padBitMax > 32) {
        mPadBitMax = 32;
    }
    std::memset(mPadRepeatDelays, 30, sizeof(mPadRepeatDelays));
    std::memset(mPadRepeatPulses, 1, sizeof(mPadRepeatPulses));
    std::memset(mPadHoldCounts, 0, sizeof(mPadHoldCounts));
}
} // namespace sead
