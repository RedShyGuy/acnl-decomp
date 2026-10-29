#include "sead/seadUlcdMethodTreeMgr.h"

namespace sead {
// 0x00548F8C slot 0x18 (name after the reference symbols)
void sead::UlcdMethodTreeMgr::pauseAll(bool pause)
{
    DualScreenMethodTreeMgr::pauseAll(pause);
    mRightRootDraw.pause(pause);
}

// 0x00549018 (name is ours)
sead::UlcdMethodTreeMgr::UlcdMethodTreeMgr()
    : DualScreenMethodTreeMgr(), mRightRootDraw(&mCriticalSection), mRightSysDraw(&mCriticalSection),
      mRightAppDraw(&mCriticalSection), mRightAppDrawFinal(&mCriticalSection)
{
    mRightRootDraw.mINamableName = "sead::RightRootDraw";
    mRightSysDraw.mINamableName = "sead::RightSysDraw";
    mRightAppDraw.mINamableName = "sead::RightAppDraw";
    mRightAppDrawFinal.mINamableName = "sead::RightAppDrawFinal";
    mRightRootDraw.pushBackChild(&mRightAppDraw);
    mRightRootDraw.pushBackChild(&mRightAppDrawFinal);
    mRightRootDraw.pushBackChild(&mRightSysDraw);
    mRightSysDraw.pause(false);
    mRightAppDraw.pause(false);
    mRightAppDrawFinal.pause(false);
    mRightRootDraw.pause(true);
}

// 0x005492A0 slot 0x08
// 0x0054921C (deleting dtor)
sead::UlcdMethodTreeMgr::~UlcdMethodTreeMgr()
{
    // (only the members are destroyed)
}

// 0x0054CB44 slot 0x10 (name after the reference symbols)
void sead::UlcdMethodTreeMgr::attachMethod(s32 methodType, MethodTreeNode* node)
{
    switch (methodType) {
    case METHOD_LEFT_SYS_DRAW:
        DualScreenMethodTreeMgr::attachMethod(METHOD_TOP_SYS_DRAW, node);
        break;
    case METHOD_LEFT_APP_DRAW:
        DualScreenMethodTreeMgr::attachMethod(METHOD_TOP_APP_DRAW, node);
        break;
    case METHOD_LEFT_APP_DRAW_FINAL:
        DualScreenMethodTreeMgr::attachMethod(METHOD_TOP_APP_DRAW_FINAL, node);
        break;
    case METHOD_RIGHT_SYS_DRAW:
        mRightSysDraw.pushBackChild(node);
        break;
    case METHOD_RIGHT_APP_DRAW:
        mRightAppDraw.pushBackChild(node);
        break;
    case METHOD_RIGHT_APP_DRAW_FINAL:
        mRightAppDrawFinal.pushBackChild(node);
        break;
    default:
        DualScreenMethodTreeMgr::attachMethod(methodType, node);
        break;
    }
}

// 0x0054CD3C slot 0x14 (name is ours)
sead::MethodTreeNode* sead::UlcdMethodTreeMgr::getRootMethodTreeNode(s32 methodType)
{
    switch (methodType) {
    case METHOD_LEFT_SYS_DRAW:
        return DualScreenMethodTreeMgr::getRootMethodTreeNode(METHOD_TOP_SYS_DRAW);
    case METHOD_LEFT_APP_DRAW:
        return DualScreenMethodTreeMgr::getRootMethodTreeNode(METHOD_TOP_APP_DRAW);
    case METHOD_LEFT_APP_DRAW_FINAL:
        return DualScreenMethodTreeMgr::getRootMethodTreeNode(METHOD_TOP_APP_DRAW_FINAL);
    case METHOD_RIGHT_SYS_DRAW:
        return &mRightSysDraw;
    case METHOD_RIGHT_APP_DRAW:
        return &mRightAppDraw;
    case METHOD_RIGHT_APP_DRAW_FINAL:
        return &mRightAppDrawFinal;
    default:
        return DualScreenMethodTreeMgr::getRootMethodTreeNode(methodType);
    }
}
// 0x0054CE78 (name is ours)
void sead::UlcdMethodTreeMgr::drawTopLeft()
{
    drawTop();
}

// 0x00545294 (name is ours)
void sead::UlcdMethodTreeMgr::drawTopRight()
{
    mRightRootDraw.call();
}
} // namespace sead

// the type information of the classes below DualScreenMethodTreeMgr
// 0x008293F0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DualScreenMethodTreeMgr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
