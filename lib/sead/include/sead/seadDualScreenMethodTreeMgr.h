#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadMethodTreeNode.h"

namespace sead {
// RTTI N4sead23DualScreenMethodTreeMgrE @ 0x008D1F38
// vtable 0x009067B8 (vptr 0x009067C0), offset_to_top 0, 8 entries
// The method trees of the two screens: one calc tree and a draw tree per screen, each with nodes for
// the system and the application.
class DualScreenMethodTreeMgr : public ::sead::MethodTreeMgr
{
public:
    DualScreenMethodTreeMgr(); // 0x0054CFF8
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E178 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return MethodTreeMgr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E12C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983564 (guard 0x00983560)
        static const RuntimeTypeInfo::Derive<MethodTreeMgr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DualScreenMethodTreeMgr(); // 0x0054D50C slot 0x08
    // 0x0054D4FC slot 0x0C (deleting dtor)
    virtual void attachMethod(s32 methodType, MethodTreeNode* node); // 0x0054CBB8 slot 0x10
    virtual MethodTreeNode* getRootMethodTreeNode(s32 methodType); // 0x0054CDA4 slot 0x14
    virtual void pauseAll(bool pause); // 0x0054CE84 slot 0x18
    virtual void pauseAppCalc(bool pause); // 0x0054CCD4 slot 0x1C

    // the trees (names are ours)
    void calc(); // 0x0054CE68
    void drawTop(); // 0x0054CE7C
    void drawBtm(); // 0x0054CE70

    // the method types (names are ours; "draw" types go to the screen of mSysDrawScreen / mAppDrawScreen)
    enum MethodType
    {
        METHOD_SYS_CALC = 0,
        METHOD_APP_CALC = 1,
        METHOD_SYS_DRAW = 2,
        METHOD_APP_DRAW = 3,
        METHOD_APP_DRAW_FINAL = 4,
        METHOD_TOP_SYS_DRAW = 5,
        METHOD_TOP_APP_DRAW = 6,
        METHOD_TOP_APP_DRAW_FINAL = 7,
        METHOD_BTM_SYS_DRAW = 8,
        METHOD_BTM_APP_DRAW = 9,
        METHOD_BTM_APP_DRAW_FINAL = 10,
    };
    // 0 top, 1 bottom screen
    enum Screen : u8
    {
        SCREEN_TOP = 0,
        SCREEN_BTM = 1,
    };

    MethodTreeNode mRootCalc;        // 0x020
    MethodTreeNode mSysCalc;         // 0x070
    MethodTreeNode mAppCalc;         // 0x0C0
    MethodTreeNode mTopRootDraw;     // 0x110
    MethodTreeNode mTopSysDraw;      // 0x160
    MethodTreeNode mTopAppDraw;      // 0x1B0
    MethodTreeNode mTopAppDrawFinal; // 0x200
    MethodTreeNode mBtmRootDraw;     // 0x250
    MethodTreeNode mBtmSysDraw;      // 0x2A0
    MethodTreeNode mBtmAppDraw;      // 0x2F0
    MethodTreeNode mBtmAppDrawFinal; // 0x340
    Screen mSysDrawScreen;           // 0x390
    Screen mAppDrawScreen;           // 0x391
};
ASSERT_SIZE(DualScreenMethodTreeMgr, 0x394);
} // namespace sead
