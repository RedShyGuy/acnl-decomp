#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreTouchObjectParam (DataStore). The member names are from the DDL
// declarations in the binary; this build has no RTTI or vtable of it (only the inline Add in
// DataStoreProtocolClient::CallTouchObject).
class _DDL_DataStoreTouchObjectParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreTouchObjectParam() : m_StructureVersion(0), m_DataId(0), m_LockId(0), m_AccessPassword(0) {}
    virtual ~_DDL_DataStoreTouchObjectParam() {}

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreTouchObjectParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LockId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_AccessPassword), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId;          // 0x08
    u32 m_LockId;          // 0x10
    u64 m_AccessPassword;  // 0x18
};
ASSERT_SIZE(_DDL_DataStoreTouchObjectParam, 0x20);
} // namespace nex
} // namespace nn
