#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_AuthenticationInfo.h"

namespace nn {
namespace nex {
// (0x00357A5C, named AuthenticationInfo::AuthenticationInfo() [ctor?] in symbols.json, is the
// target of a branch inside a login job that constructs one inline)
// RTTI N2nn3nex18AuthenticationInfoE @ 0x008CE6E8
// vtable 0x008FD468 (vptr 0x008FD470), offset_to_top 0, 8 entries
class AuthenticationInfo : public ::nn::nex::_DDL_AuthenticationInfo
{
public:
    // (inline; the names of the constants are ours)
    AuthenticationInfo()
    {
        m_Token = L"";
        m_ServerVersion = DEFAULT_SERVER_VERSION;
        m_Version = DEFAULT_VERSION;
        m_TokenType = 0;
    }
    virtual ~AuthenticationInfo(); // 0x00387420 slot 0x00
    // 0x003873FC slot 0x04 (deleting dtor)

    static const u32 DEFAULT_VERSION = 3;
    static const u32 DEFAULT_SERVER_VERSION = 2006;
};
ASSERT_SIZE(AuthenticationInfo, 0x1C);
} // namespace nex
} // namespace nn
