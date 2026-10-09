// (in applet_Wrapper.cpp in the original)
#include "nn/applet/CTR/applet_SysSleepAcceptedCallbackInfo.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace applet {
namespace CTR {
// 0x0047FD28 | nintendogs:bytes [tier A]
void nn::applet::CTR::SysSleepAcceptedCallbackInfo::Unregister()
{
    nn::os::CriticalSection::ScopedLock lock(detail::s_SleepAcceptedCallbackLock);
    for (SysSleepAcceptedCallbackInfo* pInfo = detail::s_pSleepAcceptedCallbackHead; pInfo != 0; pInfo = pInfo->m_pNext) {
        if (pInfo == this) {
            SysSleepAcceptedCallbackInfo* pPrev = pInfo->m_pPrev;
            SysSleepAcceptedCallbackInfo* pNext = pInfo->m_pNext;
            if (pPrev == 0) {
                detail::s_pSleepAcceptedCallbackHead = pNext;
            } else {
                pPrev->m_pNext = pNext;
            }
            if (pNext == 0) {
                detail::s_pSleepAcceptedCallbackTail = pPrev;
            } else {
                pNext->m_pPrev = pPrev;
            }
            pInfo->m_pPrev = 0;
            pInfo->m_pNext = 0;
            return;
        }
    }
}

// inserts the callback before the first one with a higher priority
// 0x0047FDA8 | nintendogs:bytes [tier A]
void nn::applet::CTR::SysSleepAcceptedCallbackInfo::Register()
{
    nn::os::CriticalSection::ScopedLock lock(detail::s_SleepAcceptedCallbackLock);
    if (m_pPrev != 0 || m_pNext != 0) {
        return;
    }
    SysSleepAcceptedCallbackInfo* pInfo = detail::s_pSleepAcceptedCallbackHead;
    if (pInfo == this) {
        return;
    }
    if (pInfo == 0) {
        detail::s_pSleepAcceptedCallbackHead = this;
        detail::s_pSleepAcceptedCallbackTail = this;
        m_pPrev = 0;
        return;
    }
    for (; pInfo != 0; pInfo = pInfo->m_pNext) {
        if (pInfo->m_Priority > m_Priority) {
            m_pNext = pInfo;
            m_pPrev = pInfo->m_pPrev;
            if (pInfo->m_pPrev == 0) {
                detail::s_pSleepAcceptedCallbackHead = this;
            } else {
                pInfo->m_pPrev->m_pNext = this;
            }
            pInfo->m_pPrev = this;
            return;
        }
    }
    detail::s_pSleepAcceptedCallbackTail->m_pNext = this;
    m_pPrev = detail::s_pSleepAcceptedCallbackTail;
    m_pNext = 0;
    detail::s_pSleepAcceptedCallbackTail = this;
}

} // namespace CTR
} // namespace applet
} // namespace nn
