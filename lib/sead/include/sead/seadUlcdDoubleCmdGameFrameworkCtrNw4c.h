#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDoubleCmdGameFrameworkCtrNw4c.h"

namespace sead {
// RTTI N4sead33UlcdDoubleCmdGameFrameworkCtrNw4cE @ 0x008D1FD0
// vtable 0x0090697C (vptr 0x00906984), offset_to_top 0, 38 entries
// The framework with the stereoscopic top screen: a second top image for the right eye with its own
// display buffers. Members and unnamed slots are ours.
class UlcdDoubleCmdGameFrameworkCtrNw4c : public ::sead::DoubleCmdGameFrameworkCtrNw4c
{
public:
    explicit UlcdDoubleCmdGameFrameworkCtrNw4c(const CreateArg& arg); // 0x0011EC54
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E614 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DoubleCmdGameFrameworkCtrNw4c::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E5B0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098359C (guard 0x00983598)
        static const RuntimeTypeInfo::Derive<DoubleCmdGameFrameworkCtrNw4c> s_TypeInfo;
        return &s_TypeInfo;
    }
    // (the original falls into the destructor of DoubleCmdGameFrameworkCtrNw4c with a nop)
    virtual ~UlcdDoubleCmdGameFrameworkCtrNw4c(); // 0x0054DF5C slot 0x08
    // 0x0054E1B4 slot 0x0C (deleting dtor)
    virtual FrameBuffer* getMethodFrameBuffer(s32 methodType) const; // 0x0074E5FC slot 0x18
    virtual MethodTreeMgr* createMethodTreeMgr_(Heap* heap); // 0x00549000 slot 0x30
    virtual void procDraw_(); // 0x0054E098 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
    virtual void swapBuffer_(); // 0x0054DF94 slot 0x78 | nintendogs:callseq
    virtual void vf_0x88(); // 0x0054E094 slot 0x88
    virtual void presentTopLeft_(); // 0x0054E050 slot 0x90
    virtual void presentTopRight_(); // 0x0054E05C slot 0x94

    // the 3D mode of the top screen
    void setUlcdEnable(bool enable); // 0x0011EC38 | nintendogs:bytes [tier A]

    GLuint mRightDisplayBuffers[3]; // 0x15C
};
} // namespace sead
