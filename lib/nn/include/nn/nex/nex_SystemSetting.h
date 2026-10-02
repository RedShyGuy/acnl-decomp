#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13SystemSettingE @ 0x008CE2A8
// vtable 0x008FC9E0 (vptr 0x008FC9E8), offset_to_top 0, 2 entries
class SystemSetting : public ::nn::nex::RootObject
{
public:
    SystemSetting(); // ctor candidate(s) 0x0036EAE8 (unverified)
    virtual ~SystemSetting(); // 0x0036EB88 slot 0x00 | mk7dlp:bytes
    // 0x0036EB58 slot 0x04 | slot vf_0x04 of nn::nex::SystemSetting (deleting dtor)
    void GetList(); // 0x0036EA74 | mk7dlp:callgraph [tier A]
    SystemSetting(const wchar_t*, unsigned); // 0x0036EAE8 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
