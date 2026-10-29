#pragma once

#include "decomp.h"
#include "sead/seadTaskConstructArg.h"

namespace sead {
class TaskBase;

// What makes a task: a factory function, or a string or a number that the functions set by the
// program turn into a task (the class and create are from the reference symbols, the rest is ours).
class TaskClassID
{
public:
    enum Type : u8
    {
        TYPE_INVALID = 0,
        TYPE_STRING = 1,
        TYPE_FACTORY = 2,
        TYPE_INT = 3,
    };

    typedef TaskBase* (*FactoryFunc)(const TaskConstructArg& arg);
    typedef TaskBase* (*StringFactoryFunc)(const char* name, const TaskConstructArg& arg);
    typedef TaskBase* (*IntFactoryFunc)(s32 id, const TaskConstructArg& arg);

    TaskClassID() : mType(TYPE_INVALID), mFactory(nullptr) {}
    TaskClassID(FactoryFunc factory) : mType(TYPE_FACTORY), mFactory(factory) {}

    // the task (NULL if it cannot be made)
    DECOMP_NOINLINE TaskBase* create(const TaskConstructArg& arg) const // 0x0074A86C | nintendogs:bytes [tier A]
    {
        switch (mType) {
        case TYPE_STRING:
            if (s_StringFactory != nullptr) {
                return s_StringFactory(mString, arg);
            }
            return nullptr;
        case TYPE_FACTORY:
            return mFactory(arg);
        case TYPE_INT:
            if (s_IntFactory != nullptr) {
                return s_IntFactory(mInt, arg);
            }
            return nullptr;
        default:
            return nullptr;
        }
    }

    static StringFactoryFunc s_StringFactory;
    static IntFactoryFunc s_IntFactory;

    Type mType; // 0x0
    union
    {
        const char* mString;
        FactoryFunc mFactory;
        s32 mInt;
    }; // 0x4
};
ASSERT_SIZE(TaskClassID, 0x8);
} // namespace sead
