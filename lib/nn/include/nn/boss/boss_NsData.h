#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"

namespace nn {
namespace fnd {
class DateTime;
}
namespace boss {
// RTTI N2nn4boss6NsDataE @ 0x008D0394
// One downloaded data item (by its serial id), read in pieces; the member names are ours.
class NsData
{
public:
    NsData(); // 0x0046CED8 | nintendogs:bytes [tier A]
    virtual ~NsData();

    nn::Result Initialize(unsigned serialId); // 0x0046C854 | nintendogs:bytes [tier A]
    nn::Result GetReadFlag(bool* pFlag); // 0x0046C874 | nintendogs:bytes [tier A]
    nn::Result SetReadFlag(bool flag); // 0x0046C920 | nintendogs:bytes [tier A]
    nn::Result GetHeaderInfo(nn::boss::HeaderInfoType type, void* pValue, unsigned size); // 0x0046C9B8 | nintendogs:bytes [tier A]
    nn::Result GetLastUpdated(nn::fnd::DateTime* pDateTime); // 0x0046CA84 | nintendogs:bytes [tier B]
    nn::Result Delete(); // 0x0046CBA8 (name is ours)
    // the bytes read (from the current position on), or -1 (no size) / -2 (read failed) /
    // -3 (no session) / -4 (the data changed in between)
    s32 ReadData(unsigned char* pBuffer, unsigned size); // 0x0046CC34 | nintendogs:bytes [tier A]

    u32 m_Unknown04;     // 0x04
    bool m_IsPrivileged; // 0x08, of another program (m_ProgramId)
    u32 m_SerialId;      // 0x0C
    s32 m_Size;          // 0x10, 0 until the first ReadData
    u32 m_Version;       // 0x14, of the first ReadData
    u64 m_ProgramId;     // 0x18
    s64 m_ReadOffset;    // 0x20
};
ASSERT_SIZE(NsData, 0x28);
} // namespace boss
} // namespace nn
