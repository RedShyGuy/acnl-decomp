#include "sead/seadDrawLockContext.h"
#include "sead/seadThreadMgr.h"

namespace sead {
// 0x001208F4 (name is ours)
sead::DrawLockContext::DrawLockContext() : hostio::Node(), field_0x04(0), mCriticalSection(), mOwner(nullptr), mLockCount(0)
{
}

// 0x00133148 (name is ours; symbols.json: Graphics::lockDrawContext)
void sead::DrawLockContext::lock()
{
    Thread* current = ThreadMgr::s_pInstance->getCurrentThread();
    if (mOwner == current && mLockCount > 0) {
        mLockCount++;
        return;
    }
    mCriticalSection.lock();
    mOwner = current;
    mLockCount = 1;
}

// 0x001331B8 (name is ours; symbols.json: Graphics::unlockDrawContext)
void sead::DrawLockContext::unlock()
{
    ThreadMgr::s_pInstance->getCurrentThread();
    if (--mLockCount == 0) {
        mOwner = nullptr;
        mCriticalSection.unlock();
    }
}
} // namespace sead
