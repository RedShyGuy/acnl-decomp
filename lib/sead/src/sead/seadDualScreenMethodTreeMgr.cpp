#include "sead/seadDualScreenMethodTreeMgr.h"

namespace sead {
// 0x0054CBB8 slot 0x10 (name after the reference symbols)
void sead::DualScreenMethodTreeMgr::attachMethod(s32 methodType, MethodTreeNode* node)
{
    switch (methodType) {
    case METHOD_SYS_CALC:
        mSysCalc.pushBackChild(node);
        break;
    case METHOD_APP_CALC:
        mAppCalc.pushBackChild(node);
        break;
    case METHOD_SYS_DRAW:
        if (mSysDrawScreen == SCREEN_TOP) {
            mTopSysDraw.pushFrontChild(node);
        } else if (mSysDrawScreen == SCREEN_BTM) {
            mBtmSysDraw.pushFrontChild(node);
        }
        break;
    case METHOD_APP_DRAW:
        if (mAppDrawScreen == SCREEN_TOP) {
            mTopAppDraw.pushFrontChild(node);
        } else if (mAppDrawScreen == SCREEN_BTM) {
            mBtmAppDraw.pushFrontChild(node);
        }
        break;
    case METHOD_APP_DRAW_FINAL:
        if (mAppDrawScreen == SCREEN_TOP) {
            mTopAppDrawFinal.pushFrontChild(node);
        } else if (mAppDrawScreen == SCREEN_BTM) {
            mBtmAppDrawFinal.pushFrontChild(node);
        }
        break;
    case METHOD_TOP_SYS_DRAW:
        mTopSysDraw.pushBackChild(node);
        break;
    case METHOD_TOP_APP_DRAW:
        mTopAppDraw.pushBackChild(node);
        break;
    case METHOD_TOP_APP_DRAW_FINAL:
        mTopAppDrawFinal.pushBackChild(node);
        break;
    case METHOD_BTM_SYS_DRAW:
        mBtmSysDraw.pushBackChild(node);
        break;
    case METHOD_BTM_APP_DRAW:
        mBtmAppDraw.pushBackChild(node);
        break;
    case METHOD_BTM_APP_DRAW_FINAL:
        mBtmAppDrawFinal.pushBackChild(node);
        break;
    }
}

// 0x0054CCD4 slot 0x1C (name is ours)
void sead::DualScreenMethodTreeMgr::pauseAppCalc(bool pause)
{
    mAppCalc.pause(pause);
}

// 0x0054CDA4 slot 0x14 (name is ours)
sead::MethodTreeNode* sead::DualScreenMethodTreeMgr::getRootMethodTreeNode(s32 methodType)
{
    switch (methodType) {
    case METHOD_SYS_CALC:
        return &mSysCalc;
    case METHOD_APP_CALC:
        return &mAppCalc;
    case METHOD_SYS_DRAW:
        if (mSysDrawScreen == SCREEN_TOP) {
            return &mTopSysDraw;
        }
        if (mSysDrawScreen == SCREEN_BTM) {
            return &mBtmSysDraw;
        }
        return nullptr;
    case METHOD_APP_DRAW:
        if (mAppDrawScreen == SCREEN_TOP) {
            return &mTopAppDraw;
        }
        if (mAppDrawScreen == SCREEN_BTM) {
            return &mBtmAppDraw;
        }
        return nullptr;
    case METHOD_APP_DRAW_FINAL:
        if (mAppDrawScreen == SCREEN_TOP) {
            return &mTopAppDrawFinal;
        }
        if (mAppDrawScreen == SCREEN_BTM) {
            return &mBtmAppDrawFinal;
        }
        return nullptr;
    case METHOD_TOP_SYS_DRAW:
        return &mTopSysDraw;
    case METHOD_TOP_APP_DRAW:
        return &mTopAppDraw;
    case METHOD_TOP_APP_DRAW_FINAL:
        return &mTopAppDrawFinal;
    case METHOD_BTM_SYS_DRAW:
        return &mBtmSysDraw;
    case METHOD_BTM_APP_DRAW:
        return &mBtmAppDraw;
    case METHOD_BTM_APP_DRAW_FINAL:
        return &mBtmAppDrawFinal;
    }
    return nullptr;
}

// 0x0054CE68 (name is ours)
void sead::DualScreenMethodTreeMgr::calc()
{
    mRootCalc.call();
}

// 0x0054CE70 (name is ours)
void sead::DualScreenMethodTreeMgr::drawBtm()
{
    mBtmRootDraw.call();
}

// 0x0054CE7C (name is ours)
void sead::DualScreenMethodTreeMgr::drawTop()
{
    mTopRootDraw.call();
}

// 0x0054CE84 slot 0x18 (name after the reference symbols)
void sead::DualScreenMethodTreeMgr::pauseAll(bool pause)
{
    mRootCalc.pause(pause);
    mTopRootDraw.pause(pause);
    mBtmRootDraw.pause(pause);
}

// 0x0054CFF8 (name is ours)
sead::DualScreenMethodTreeMgr::DualScreenMethodTreeMgr()
    : MethodTreeMgr(), mRootCalc(&mCriticalSection), mSysCalc(&mCriticalSection), mAppCalc(&mCriticalSection),
      mTopRootDraw(&mCriticalSection), mTopSysDraw(&mCriticalSection), mTopAppDraw(&mCriticalSection),
      mTopAppDrawFinal(&mCriticalSection), mBtmRootDraw(&mCriticalSection), mBtmSysDraw(&mCriticalSection),
      mBtmAppDraw(&mCriticalSection), mBtmAppDrawFinal(&mCriticalSection), mSysDrawScreen(SCREEN_BTM),
      mAppDrawScreen(SCREEN_TOP)
{
    mRootCalc.mINamableName = "sead::RootCalc";
    mSysCalc.mINamableName = "sead::SysCalc";
    mAppCalc.mINamableName = "sead::AppCalc";
    mTopRootDraw.mINamableName = "sead::TopRootDraw";
    mTopSysDraw.mINamableName = "sead::TopSysDraw";
    mTopAppDraw.mINamableName = "sead::TopAppDraw";
    mTopAppDrawFinal.mINamableName = "sead::TopAppDrawFinal";
    mBtmRootDraw.mINamableName = "sead::BtmRootDraw";
    mBtmSysDraw.mINamableName = "sead::BtmSysDraw";
    mBtmAppDraw.mINamableName = "sead::BtmAppDraw";
    mBtmAppDrawFinal.mINamableName = "sead::BtmAppDrawFinal";
    mRootCalc.pushBackChild(&mSysCalc);
    mRootCalc.pushBackChild(&mAppCalc);
    mTopRootDraw.pushBackChild(&mTopAppDraw);
    mTopRootDraw.pushBackChild(&mTopAppDrawFinal);
    mTopRootDraw.pushBackChild(&mTopSysDraw);
    mBtmRootDraw.pushBackChild(&mBtmAppDraw);
    mBtmRootDraw.pushBackChild(&mBtmAppDrawFinal);
    mBtmRootDraw.pushBackChild(&mBtmSysDraw);
    mSysCalc.pause(false);
    mAppCalc.pause(false);
    mTopSysDraw.pause(false);
    mTopAppDraw.pause(false);
    mTopAppDrawFinal.pause(false);
    mBtmSysDraw.pause(false);
    mBtmAppDraw.pause(false);
    mBtmAppDrawFinal.pause(false);
    // the roots wait for the framework
    mRootCalc.pause(true);
    mTopRootDraw.pause(true);
    mBtmRootDraw.pause(true);
}

// 0x0054D50C slot 0x08
// 0x0054D4FC (deleting dtor)
sead::DualScreenMethodTreeMgr::~DualScreenMethodTreeMgr()
{
    // (only the members are destroyed)
}
} // namespace sead

// the type information of the classes below MethodTreeMgr
// 0x00828D28 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::MethodTreeMgr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
