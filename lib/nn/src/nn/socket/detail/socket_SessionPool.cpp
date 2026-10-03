#include "nn/socket/detail/socket_SessionPool.h"
#include "nn/srv/srv_Api.h"

namespace nn {
namespace socket {
namespace detail {

// 0x00485888 | fefates:bytes [tier B]
void nn::socket::detail::SessionPool::SemiFinalize()
{
    nn::os::CriticalSection::ScopedLock lock(mLock);
    mIsFinalizing = true;
    mEvent.Pulse();
    while (!mIdleSessions.IsEmpty()) {
        SessionItem* item = mIdleSessions.PopFront();
        item->mSession.Close();
        mIdleCount--;
    }
    while (!mFreeItems.IsEmpty()) {
        mFreeCount--;
        SessionItem* item = mFreeItems.PopFront();
        item->mSession.Close();
    }
}

// 0x00485994 | fefates:bytes [tier B]
nn::Result nn::socket::detail::SessionPool::AddNewSession()
{
    SessionItem* item = mFreeItems.GetFront();
    if (item == 0) {
        return nn::Result(RESULT_NO_FREE_SESSION);
    }
    nn::Result result = nn::srv::GetServiceHandle(&item->mSession, mName, mNameLength);
    if (result.IsFailure()) {
        return result;
    }
    result = InitializeSession(&item->mSession);
    if (result.IsFailure()) {
        item->mSession.Close();
        return result;
    }
    mFreeItems.PopFront();
    mFreeCount--;
    mIdleSessions.PushBack(item);
    mIdleCount++;
    return nn::Result();
}

} // namespace detail
} // namespace socket
} // namespace nn
