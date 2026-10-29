#pragma once

#include "decomp.h"
#include "sead/seadDelegateBase.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadNew.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Delegate1<@anon::InsectBbsNotice, script::Phrase*>  typeinfo 0x008D22C0  vtable 0x00906F68
//   sead::Delegate1<@anon::TakumiImportPhrase, script::Phrase*>  typeinfo 0x008D22B4  vtable 0x00906F58
//   sead::Delegate1<DemoDollSetNpcCafeHold, HumanModel*>  typeinfo 0x008D2290  vtable 0x00906F28
//   sead::Delegate1<EvBbsNotice, script::Phrase*>  typeinfo 0x008D2278  vtable 0x00906F08
//   sead::Delegate1<SetCommonStrItemNameForMail, script::Phrase*>  typeinfo 0x008D229C  vtable 0x00906F38
//   sead::Delegate1<SetCommonStrTownNameForMail, script::Phrase*>  typeinfo 0x008D22A8  vtable 0x00906F48
//   sead::Delegate1<StrcBbsNotice, script::Phrase*>  typeinfo 0x008D2284  vtable 0x00906F18
//   sead::Delegate1<sead::FaderTaskBase, sead::TaskBase*>  typeinfo 0x008D22F0  vtable 0x00906FA8
//   sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*>  typeinfo 0x008D22E4  vtable 0x00906F98
//   sead::Delegate1<svnpc::Animal, script::Phrase*>  typeinfo 0x008D22CC  vtable 0x00906F78
//   sead::Delegate1<svqst::TimeCapsule, script::Phrase*>  typeinfo 0x008D22D8  vtable 0x00906F88
//
// Calls a member function of an object; clone copies the delegate onto a heap.
template <typename T0, typename T1>
class Delegate1 : public DelegateBase<T0, void (T0::*)(T1), IDelegate1<T1>>
{
public:
    typedef DelegateBase<T0, void (T0::*)(T1), IDelegate1<T1>> Base;

    Delegate1() : Base() {}
    Delegate1(T0* object, typename Base::MethodType method) : Base(object, method) {}

    // (DECOMP_EMIT_COPY: GCC drops the explicit instances with a class of an unnamed namespace of the game)
    DECOMP_EMIT_COPY virtual void invoke(T1 arg)
    {
        if (this->mObject != nullptr && this->mMethod != nullptr) {
            (this->mObject->*this->mMethod)(arg);
        }
    }
    DECOMP_EMIT_COPY virtual IDelegate1<T1>* clone(Heap* heap) const { return new (heap, 4) Delegate1(*this); }
};
} // namespace sead
