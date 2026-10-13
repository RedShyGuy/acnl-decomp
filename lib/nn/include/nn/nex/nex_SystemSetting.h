#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// A named setting of nex with a number (e.g. "CallContextRegisterCheckExpiredCalls": the period of
// the check in ms). All settings are in a list. The names of the members are ours.
// RTTI N2nn3nex13SystemSettingE @ 0x008CE2A8
// vtable 0x008FC9E0 (vptr 0x008FC9E8), offset_to_top 0, 2 entries
class SystemSetting : public ::nn::nex::RootObject
{
public:
    typedef qChain<SystemSetting*, DefaultChainPolicy<SystemSetting*> > List;

    SystemSetting(const wchar_t* pName, unsigned int value); // 0x0036EAE8 | mk7dlp:bytes [tier A]
    virtual ~SystemSetting(); // 0x0036EB88 slot 0x00 | mk7dlp:bytes [tier A]
    // 0x0036EB58 slot 0x04 (deleting dtor)
    // the list of all settings
    static List& GetList(); // 0x0036EA74 | mk7dlp:callgraph [tier A]

    SystemSetting* m_pChainNext; // 0x04
    SystemSetting* m_pChainPrev; // 0x08
    const wchar_t* m_pName;      // 0x0C
    u32 m_Value;                 // 0x10
    bool m_Unknown0x14;          // 0x14
};
ASSERT_SIZE(SystemSetting, 0x18);
} // namespace nex
} // namespace nn
