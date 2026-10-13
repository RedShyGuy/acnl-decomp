#include "nn/nex/nex_SpinTest.h"
#include "nn/os/os_Api.h"

namespace nn {
namespace nex {
// 0x003D5E20 | fefates:bytes-fuzzy [tier B]
bool nn::nex::SpinTest::SpinOnce(wchar_t* pFile, unsigned int line, wchar_t* pCondition)
{
    if (m_SleepMilliseconds != 0) {
        OsSleepThread(static_cast<u64>(m_SleepMilliseconds) * 1000000);
    } else {
        OsYieldThread();
    }
    Time now = SystemClock::GetTime();
    m_Elapsed = static_cast<u32>(now.m_Value) - static_cast<u32>(m_StartTime.m_Value);
    if (m_Elapsed > m_Timeout && m_Timeout != static_cast<u32>(-1)) {
        m_IsTimedOut = true;
        switch (m_Mode) {
        case MODE_CONTINUE:
            break;
        case MODE_STOP:
            return false;
        }
    }
    return true;
}

// 0x003D5EAC | fefates:bytes [tier B]
nn::nex::SpinTest::SpinTest(unsigned int sleepMilliseconds, unsigned int timeout)
{
    m_SleepMilliseconds = sleepMilliseconds;
    m_Timeout = timeout;
    m_StartTime = SystemClock::GetTime();
    m_Elapsed = 0;
    m_Unknown0x20 = 1000;
    m_Mode = MODE_CONTINUE;
    m_Unknown0x28 = 0;
    m_Unknown0x2C = 8;
    m_IsTimedOut = false;
}

// 0x003D5F48
// 0x003D5F24 (deleting dtor)
nn::nex::SpinTest::~SpinTest()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
