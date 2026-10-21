#include "nn/nex/nex__DDL_DataStoreChangeMetaCompareParam.h"
#include "nn/nex/nex_DataStoreChangeMetaCompareParam.h"

namespace nn {
namespace nex {
// 0x003CC0D4 slot 0x00
// 0x003CC048 (deleting dtor)
nn::nex::_DDL_DataStoreChangeMetaCompareParam::~_DDL_DataStoreChangeMetaCompareParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CC15C
nn::nex::_DDL_DataStoreChangeMetaCompareParam& nn::nex::_DDL_DataStoreChangeMetaCompareParam::operator=(const nn::nex::_DDL_DataStoreChangeMetaCompareParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_ComparisonFlag = other.m_ComparisonFlag;
    m_Name = other.m_Name;
    m_Permission = other.m_Permission;
    m_DelPermission = other.m_DelPermission;
    m_Period = other.m_Period;
    m_MetaBinary = other.m_MetaBinary;
    m_Tags = other.m_Tags;
    m_ReferredCnt = other.m_ReferredCnt;
    m_DataType = other.m_DataType;
    m_Status = other.m_Status;
    return *this;
}

// 0x003CBEC4
void nn::nex::_DDL_DataStoreChangeMetaCompareParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreChangeMetaCompareParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ComparisonFlag), sizeof(u32));
    _Type_string::Add(pMessage, data.m_Name);
    DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_Permission);
    DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_DelPermission);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Period), sizeof(u16));
    _Type_qBuffer::Add(pMessage, data.m_MetaBinary);
    _Type_qvector<_Type_string>::Add(pMessage, data.m_Tags);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferredCnt), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Status), sizeof(u8));
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
