#pragma once

#include "decomp.h"
#include "sead/seadVector2.h"
#include "sead/seadBoundBox2.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead14ControllerBaseE @ 0x008D16D8
// vtable 0x00905738 (vptr 0x00905740), offset_to_top 0, 2 entries
// The state of a controller: the pad bits (hold/trigger/release/repeat), two sticks, two analog
// triggers and a pointer. The names of the functions marked with reference symbols are from them, the
// members and the other names are ours.
class ControllerBase
{
public:
    ControllerBase(s32 padBitMax, s32 leftStickCrossStartBit, s32 rightStickCrossStartBit, s32 touchKeyBit); // 0x0054471C | nintendogs:callseq [tier A]
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BD64 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BD18 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983464 (guard 0x00983460)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }

    // the bits of mPointerFlag
    enum PointerFlag
    {
        POINTER_ON = 1,
        POINTER_ON_NOW = 2,              // turned on in this frame
        POINTER_OFF_NOW = 4,             // turned off in this frame
        POINTER_SET_INVALID_ON_OFF = 8,  // the pointer goes to s_InvalidPointer when it is off
    };

    // the bits of a stick that are held: start + 0 up, + 1 down, + 2 left, + 3 right (by the angle of the
    // stick, after the hold/release thresholds)
    u32 getStickHold_(u32 prevHold, const Vector2<f32>& stick, f32 holdThreshold, f32 releaseThreshold,
                      s32 startBit); // 0x00544264 | nintendogs:bytes [tier A]
    // the bits of the sticks, trigger/release/repeat and the pointer flags from the previous state
    void updateDerivativeParams_(u32 prevHold, bool prevPointerOn); // 0x00544554 | nintendogs:callseq [tier A]
    // the pointer at pos (relative to mPointerBound if it is set; outside it the pointer is off) and
    // the touch key bit
    void setPointerWithBound_(bool isOn, bool isTouchKeyHold, const Vector2<f32>& pos); // 0x00544444
    // the repeat of the pad bits of mask
    void setPadRepeat(u32 mask, u8 delayFrame, u8 pulseFrame); // 0x00544224
    // nothing held, no pointer, the sticks and triggers at rest
    bool isIdleBase_() const; // 0x005440BC
    // clears all state
    void setIdleBase_(); // 0x00544160

    // the pointer while it is off
    static const Vector2<f32> s_InvalidPointer;
    static const Vector2<s32> s_InvalidPointerS32;

    u32 mPadTrig;                     // 0x004
    u32 mPadRelease;                  // 0x008
    u32 mPadRepeat;                   // 0x00C
    u32 mPointerFlag;                 // 0x010
    Vector2<s32> mPointerS32;         // 0x014
    BoundBox2<f32> mPointerBound;     // 0x01C, the area of the pointer (none while min > max)
    u32 mPadHoldCounts[32];           // 0x02C, frames each bit is held
    u8 mPadRepeatDelays[32];          // 0x0AC
    u8 mPadRepeatPulses[32];          // 0x0CC
    f32 mLeftStickHoldThreshold;      // 0x0EC
    f32 mRightStickHoldThreshold;     // 0x0F0
    f32 mLeftStickReleaseThreshold;   // 0x0F4
    f32 mRightStickReleaseThreshold;  // 0x0F8
    s32 mPadBitMax;                   // 0x0FC, at most 32
    s32 mLeftStickCrossStartBit;      // 0x100, -1: no bits
    s32 mRightStickCrossStartBit;     // 0x104
    s32 mTouchKeyBit;                 // 0x108, the pad bit of the touch (-1: none)
    s32 mIdleFrame;                   // 0x10C
    u32 mPadHold;                     // 0x110
    Vector2<f32> mPointer;            // 0x114
    Vector2<f32> mLeftStick;          // 0x11C
    Vector2<f32> mRightStick;         // 0x124
    f32 mLeftAnalogTrigger;           // 0x12C
    f32 mRightAnalogTrigger;          // 0x130
};
ASSERT_SIZE(ControllerBase, 0x134);
} // namespace sead
