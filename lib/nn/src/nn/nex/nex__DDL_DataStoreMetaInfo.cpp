#include "nn/nex/nex__DDL_DataStoreMetaInfo.h"
#include "nn/nex/nex_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// 0x0039F070
nn::nex::_DDL_DataStoreMetaInfo::_DDL_DataStoreMetaInfo() : m_StructureVersion(0), m_MetaBinary(0, 0), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_Tags(0), m_Ratings(0)
{
}

// 0x0039F164 slot 0x00
// 0x0039F154 (deleting dtor)
nn::nex::_DDL_DataStoreMetaInfo::~_DDL_DataStoreMetaInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0039F1F0
nn::nex::_DDL_DataStoreMetaInfo& nn::nex::_DDL_DataStoreMetaInfo::operator=(const nn::nex::_DDL_DataStoreMetaInfo& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_DataId = other.m_DataId;
    m_OwnerId = other.m_OwnerId;
    m_Size = other.m_Size;
    m_Name = other.m_Name;
    m_DataType = other.m_DataType;
    m_MetaBinary = other.m_MetaBinary;
    m_Permission = other.m_Permission;
    m_DelPermission = other.m_DelPermission;
    m_CreatedTime = other.m_CreatedTime;
    m_UpdatedTime = other.m_UpdatedTime;
    m_Period = other.m_Period;
    m_Status = other.m_Status;
    m_ReferredCnt = other.m_ReferredCnt;
    m_ReferDataId = other.m_ReferDataId;
    m_Flag = other.m_Flag;
    m_ReferredTime = other.m_ReferredTime;
    m_ExpireTime = other.m_ExpireTime;
    m_Tags = other.m_Tags;
    m_Ratings = other.m_Ratings;
    return *this;
}

// 0x0039EDA4
void nn::nex::_DDL_DataStoreMetaInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreMetaInfo* pData)
{
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    pMessage->ExtractPrimitive(&pData->m_DataId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_OwnerId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Size);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_Name);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_DataType);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qBuffer::Extract(pMessage, &pData->m_MetaBinary);
    if (pMessage->m_IsError) {
        return;
    }
    DDLClassMetaType<DataStorePermission>::Extract(pMessage, &pData->m_Permission);
    if (pMessage->m_IsError) {
        return;
    }
    DDLClassMetaType<DataStorePermission>::Extract(pMessage, &pData->m_DelPermission);
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_CreatedTime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_UpdatedTime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Period);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Status);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_ReferredCnt);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_ReferDataId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Flag);
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_ReferredTime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_ExpireTime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_Tags);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<DDLClassMetaType<DataStoreRatingInfoWithSlot>>::Extract(pMessage, &pData->m_Ratings);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
