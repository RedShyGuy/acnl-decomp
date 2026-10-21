#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// A probe of a station URL by the NAT traversal: the URL, its deadline and the round trip time of
// the reply. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex8URLProbeE
class URLProbe : public ::nn::nex::RootObject
{
public:
    // timeout: ms until the deadline; interval: ms between the probes after a reply; maxSendCount:
    // the probes without a reply
    URLProbe(const nn::nex::StationURL& url, int timeout, int interval, unsigned int maxSendCount); // 0x003D6110 | mk7dlp:bytes-fuzzy [tier A]
    virtual ~URLProbe(); // 0x003D61F0 slot 0x00
    // 0x003D61BC slot 0x04 (deleting dtor)
    // the reply to the probe sent at sendTime (0: not known) came from the port of the URL
    bool ProcessReply(const nn::nex::StationURL& url, const nn::nex::Time& sendTime); // 0x003D6038

    StationURL m_URL;     // 0x04
    Time m_Deadline;      // 0x70
    s32 m_Result;         // 0x78 (the round trip time in ms; -1: no reply yet)
    Time m_LastReplyTime; // 0x80 (0: none)
    s32 m_Interval;       // 0x88
    u32 m_SendCount;      // 0x8C
    u32 m_MaxSendCount;   // 0x90
};
ASSERT_OFFSET(URLProbe, m_Deadline, 0x70);
ASSERT_OFFSET(URLProbe, m_Result, 0x78);
ASSERT_SIZE(URLProbe, 0x98);
} // namespace nex
} // namespace nn
