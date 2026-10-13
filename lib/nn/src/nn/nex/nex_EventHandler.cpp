#include "nn/nex/nex_EventHandler.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_Time.h"
#include "nn/os/os_Api.h"
#include <cstring>

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Platform/Core/EventHandler.cpp";
// the time between two polls of WaitForBoolEvent (1 ms)
const s64 POLL_INTERVAL_NS = 1000000;
const u16 NO_EVENT = 0xFFFF;
} // namespace

// 0x0096CA7C (name is ours)
u32 EventHandler::s_WaitStartIndex;

// 0x0013CF0C | fefates:callseq-callee [tier C]
unsigned short nn::nex::EventHandler::GetEventIndex(nn::nex::Event* pEvent) const
{
    ScopedCS lock(m_Lock);
    u16 index;
    for (index = 0; index < m_Size; index++) {
        if (m_ppEvents[index] == pEvent) {
            break;
        }
    }
    if (index == m_Size) {
        return 0;
    }
    return index;
}

// 0x0035C774 | fefates:callseq-callee [tier C]
nn::nex::Event* nn::nex::EventHandler::CreateEventObject(unsigned int unknown0x08, unsigned int unknown0x0C)
{
    ScopedCS lock(m_Lock);
    int index;
    for (index = 0; index < m_Size; index++) {
        if (m_ppEvents[index] == nullptr) {
            break;
        }
    }
    m_ppEvents[index] = new Event(this, unknown0x08, unknown0x0C);
    m_EventCount++;
    m_pFlags->m_pFlags[GetEventIndex(m_ppEvents[index])].m_IsSet = false;
    return m_ppEvents[index];
}

// 0x0035C88C | fefates:callseq [tier C]
nn::nex::EventHandler::EventHandler(unsigned short size) : m_Lock(0x20000000)
{
    m_pFlags = new (MemoryManager::Allocate(sizeof(Flags))) Flags;
    m_Size = size;
    m_pFlags->m_pFlags = NewArray<Flag>(size);
    m_ppEvents = NewArray<Event*>(m_Size);
    memset(m_ppEvents, 0, m_Size * sizeof(Event*));
    m_EventCount = 0;
}

// 0x0035C998 | fefates:callseq [tier C]
// 0x0035C968 (deleting dtor)
nn::nex::EventHandler::~EventHandler()
{
    DeleteArray(m_ppEvents);
    DeleteArray(m_pFlags->m_pFlags);
    MemoryManager::FreeInline(m_pFlags);
}

// 0x0072A390 | mk7dlp:callseq [tier C]
bool nn::nex::EventHandler::WaitForBoolEvent(unsigned int timeout, nn::nex::Event** ppEvent, bool isTimeoutExpected) const
{
    Time start = SystemClock::GetTime();
    u16 index;
    while (true) {
        index = NO_EVENT;
        {
            ScopedCS lock(m_Lock);
            for (u32 i = 0; i < m_EventCount; i++) {
                u32 slot = (s_WaitStartIndex + i) % m_EventCount;
                if (m_pFlags->m_pFlags[GetEventIndex(m_ppEvents[slot])].m_IsSet) {
                    index = slot;
                    break;
                }
            }
            if (index == NO_EVENT) {
                s_WaitStartIndex++;
            }
        }
        if (index != NO_EVENT) {
            break;
        }
        if (timeout != static_cast<unsigned int>(-1)) {
            Time now = SystemClock::GetTime();
            if (static_cast<s64>(now.m_Value - start.m_Value) >= static_cast<s64>(timeout)) {
                if (!isTimeoutExpected) {
                    SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 311, SystemError::ERROR_TIMEOUT, 0);
                }
                return false;
            }
        }
        OsSleepThread(POLL_INTERVAL_NS);
    }
    m_ppEvents[index]->Reset();
    *ppEvent = m_ppEvents[index];
    return true;
}

} // namespace nex
} // namespace nn
