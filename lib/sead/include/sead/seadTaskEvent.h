#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead9TaskEventE @ 0x008D23D0
// vtable 0x00907120 (vptr 0x00907128), offset_to_top 0, 2 entries
class TaskEvent
{
public:
    // (inline; the type is a number the tasks agree on, name is ours)
    explicit TaskEvent(u8 type) : mType(type) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00750AA4 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00750A58 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983604 (guard 0x00983600)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }

    u8 mType; // 0x4
};
ASSERT_SIZE(TaskEvent, 0x8);
} // namespace sead
