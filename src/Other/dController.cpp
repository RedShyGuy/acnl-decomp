#include "Other/dController.h"

#include "applet/dErrEulaMgr.h"
#include "sead/seadController.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadCtrController.h"

// (inline)
inline sead::Controller* Controller::GetController()
{
    return sead::ControllerMgr::s_pInstance->getController(0);
}

// 0x00304A14 | libgarden [tier A]
bool Controller::IsHold(u32 mask)
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    return (GetController()->mPadHold & mask) != 0;
}

// 0x005222F4
void Controller::SetupCirclePad()
{
    sead::CtrController* controller = sead::DynamicCast<sead::CtrController>(GetController());
    if (controller != NULL) {
        controller->setNormalizeStickScaleSettings(1.0f, 141);
    }
}

// 0x005223A0 (name after libgarden)
bool Controller::IsRelease(u32 mask)
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    return (GetController()->mPadRelease & mask) != 0;
}

// 0x005223E8 | libgarden [tier A]
bool Controller::IsTrig(u32 mask)
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    return (GetController()->mPadTrig & mask) != 0;
}

// 0x00522430
bool Controller::IsTrigAIfAllowed()
{
    if (!Func5C37C4()) {
        return false;
    }
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    return (GetController()->mPadTrig & A) != 0;
}

// 0x00522480
void Controller::Initialize()
{
    // empty (in the original too)
}

// 0x00522484
void Controller::Finalize()
{
    // empty (in the original too)
}

// 0x00522488
const sead::Vector2<f32>& Controller::GetPointer()
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        // 0x00954B24 (guard 0x00954B14)
        static const sead::Vector2<f32> s_BlockedPointer(-3.4028235e38f, -3.4028235e38f);
        return s_BlockedPointer;
    }
    sead::Controller* controller = GetController();
    if ((controller->mPointerFlag & sead::ControllerBase::POINTER_ON) != 0) {
        return controller->mPointer;
    }
    return sead::ControllerBase::s_InvalidPointer;
}

// 0x00522520 | libgarden [tier A]
const sead::Vector2<f32>& Controller::GetCirclePad()
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        // 0x00954B1C (guard 0x00954B10)
        static const sead::Vector2<f32> s_BlockedCirclePad(0.0f, 0.0f);
        return s_BlockedCirclePad;
    }
    return GetController()->mLeftStick;
}

// 0x005225A4
const sead::Vector2<s32>& Controller::GetPointerS32()
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        // 0x00954B2C (guard 0x00954B18)
        static const sead::Vector2<s32> s_BlockedPointerS32(-0x7FFFFFFF - 1, -0x7FFFFFFF - 1);
        return s_BlockedPointerS32;
    }
    sead::Controller* controller = GetController();
    if ((controller->mPointerFlag & sead::ControllerBase::POINTER_ON) != 0) {
        return controller->mPointerS32;
    }
    return sead::ControllerBase::s_InvalidPointerS32;
}

// 0x00522638 (name after libgarden)
bool Controller::IsRepeat(u32 mask)
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    sead::Controller* controller = GetController();
    return ((controller->mPadTrig | controller->mPadRepeat) & mask) != 0;
}
