#include "nn/nex/nex_URLProbe.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_OutputFormat.h"

namespace nn {
namespace nex {
namespace {
// (inline; the name is ours) the lines of the trace are indented while it lives
class ScopedTraceIndent
{
public:
    ScopedTraceIndent() { GlobalVariables::s_pInstance->m_TraceLog.m_pFormat->m_Unknown0x04 += 2; }
    ~ScopedTraceIndent()
    {
        OutputFormat* pFormat = GlobalVariables::s_pInstance->m_TraceLog.m_pFormat;
        if (pFormat->m_Unknown0x04 > 2) {
            pFormat->m_Unknown0x04 -= 2;
        } else {
            pFormat->m_Unknown0x04 = 0;
        }
    }
};
} // namespace

// 0x003D6038 (name is ours)
bool nn::nex::URLProbe::ProcessReply(const nn::nex::StationURL& url, const nn::nex::Time& sendTime)
{
    Time time(0);
    {
        ScopedTraceIndent indent;
        m_URL.SetPortNumberInline(url.GetPortNumberInline());
        if (time < sendTime) {
            m_Result = static_cast<s32>(SystemClock::GetTime().m_Value - sendTime.m_Value);
        }
    }
    time = SystemClock::GetTime();
    m_LastReplyTime = time;
    return true;
}

// 0x003D6110 | mk7dlp:bytes-fuzzy [tier A]
nn::nex::URLProbe::URLProbe(const nn::nex::StationURL& url, int timeout, int interval, unsigned int maxSendCount)
    : m_URL(url), m_Deadline(0), m_Result(-1), m_LastReplyTime(0), m_Interval(interval), m_SendCount(0), m_MaxSendCount(maxSendCount)
{
    m_Deadline = SystemClock::GetTime() + timeout;
}

// 0x003D61F0 slot 0x00
// 0x003D61BC (deleting dtor)
nn::nex::URLProbe::~URLProbe()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
