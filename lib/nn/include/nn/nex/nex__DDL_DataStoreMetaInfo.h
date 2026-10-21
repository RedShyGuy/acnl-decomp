#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_DataStoreRatingInfoWithSlot.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreMetaInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex22_DDL_DataStoreMetaInfoE @ 0x008CEC98
// vtable 0x008FE20C (vptr 0x008FE214), offset_to_top 0, 2 entries
class _DDL_DataStoreMetaInfo : public ::nn::nex::RootObject
{
public:
    _DDL_DataStoreMetaInfo(); // 0x0039F070
    virtual ~_DDL_DataStoreMetaInfo(); // 0x0039F164 slot 0x00
    // 0x0039F154 slot 0x04 (deleting dtor)
    _DDL_DataStoreMetaInfo& operator=(const _DDL_DataStoreMetaInfo& other); // 0x0039F1F0

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreMetaInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Size), sizeof(u32));
        _Type_string::Add(pMessage, data.m_Name);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
        _Type_qBuffer::Add(pMessage, data.m_MetaBinary);
        DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_Permission);
        DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_DelPermission);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_CreatedTime), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_UpdatedTime), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Period), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Status), sizeof(u8));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferredCnt), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferDataId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Flag), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferredTime), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ExpireTime), sizeof(u64));
        _Type_qvector<_Type_string>::Add(pMessage, data.m_Tags);
        _Type_qvector<DDLClassMetaType<DataStoreRatingInfoWithSlot>>::Add(pMessage, data.m_Ratings);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreMetaInfo* pData); // 0x0039EDA4

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    u32 m_OwnerId; // 0x10
    u32 m_Size; // 0x14
    String m_Name; // 0x18
    u16 m_DataType; // 0x20
    qBuffer m_MetaBinary; // 0x24
    DataStorePermission m_Permission; // 0x44
    DataStorePermission m_DelPermission; // 0x58
    DateTime m_CreatedTime; // 0x70
    DateTime m_UpdatedTime; // 0x78
    u16 m_Period; // 0x80
    u8 m_Status; // 0x82
    u32 m_ReferredCnt; // 0x84
    u32 m_ReferDataId; // 0x88
    u32 m_Flag; // 0x8C
    DateTime m_ReferredTime; // 0x90
    DateTime m_ExpireTime; // 0x98
    qVector<String> m_Tags; // 0xA0
    qVector<DataStoreRatingInfoWithSlot> m_Ratings; // 0xAC
};
ASSERT_OFFSET(_DDL_DataStoreMetaInfo, m_Ratings, 0xAC);
ASSERT_SIZE(_DDL_DataStoreMetaInfo, 0xB8);
} // namespace nex
} // namespace nn
