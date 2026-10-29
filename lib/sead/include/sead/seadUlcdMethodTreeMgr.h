#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"

namespace sead {
// RTTI N4sead17UlcdMethodTreeMgrE @ 0x008D1C20
// vtable 0x00906080 (vptr 0x00906088), offset_to_top 0, 8 entries
// The method trees with a draw tree for the right image of the stereoscopic top screen.
class UlcdMethodTreeMgr : public ::sead::DualScreenMethodTreeMgr
{
public:
    UlcdMethodTreeMgr(); // 0x00549018
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C9BC (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DualScreenMethodTreeMgr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C970 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834CC (guard 0x009834C8)
        static const RuntimeTypeInfo::Derive<DualScreenMethodTreeMgr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~UlcdMethodTreeMgr(); // 0x005492A0 slot 0x08
    // 0x0054921C slot 0x0C (deleting dtor)
    // 11..13: the top screen types 5..7 (the left image), 14..16: the right image
    virtual void attachMethod(s32 methodType, MethodTreeNode* node); // 0x0054CB44 slot 0x10
    virtual MethodTreeNode* getRootMethodTreeNode(s32 methodType); // 0x0054CD3C slot 0x14
    virtual void pauseAll(bool pause); // 0x00548F8C slot 0x18

    // the trees of the two images of the top screen (names are ours; the left one is drawTop: the
    // original falls into it with a nop)
    void drawTopLeft(); // 0x0054CE78
    void drawTopRight(); // 0x00545294

    // the method types of the image for the right eye (names are ours)
    enum UlcdMethodType
    {
        METHOD_LEFT_SYS_DRAW = 11,
        METHOD_LEFT_APP_DRAW = 12,
        METHOD_LEFT_APP_DRAW_FINAL = 13,
        METHOD_RIGHT_SYS_DRAW = 14,
        METHOD_RIGHT_APP_DRAW = 15,
        METHOD_RIGHT_APP_DRAW_FINAL = 16,
    };

    MethodTreeNode mRightRootDraw;     // 0x394
    MethodTreeNode mRightSysDraw;      // 0x3E4
    MethodTreeNode mRightAppDraw;      // 0x434
    MethodTreeNode mRightAppDrawFinal; // 0x484
};
ASSERT_SIZE(UlcdMethodTreeMgr, 0x4D4);
} // namespace sead
