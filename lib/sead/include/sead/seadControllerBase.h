#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead14ControllerBaseE @ 0x008D16D8
// vtable 0x00905738 (vptr 0x00905740), offset_to_top 0, 2 entries
class ControllerBase
{
public:
    ControllerBase(); // ctor candidate(s) 0x0054471C (unverified)
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
    void getStickHold_(unsigned, const sead::Vector2<float>&, float, float, int); // 0x00544264 | nintendogs:bytes [tier A]
    void updateDerivativeParams_(unsigned, bool); // 0x00544554 | nintendogs:callseq [tier A]
    ControllerBase(int, int, int, int); // 0x0054471C | nintendogs:callseq [tier A]
};
} // namespace sead
