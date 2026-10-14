#include "nn/nex/nex__DDL_DataStorePreparePostParam.h"
#include "nn/nex/nex_DataStorePreparePostParam.h"

namespace nn {
namespace nex {
// 0x003C46F0 slot 0x00
// 0x003C46E0 (deleting dtor)
nn::nex::_DDL_DataStorePreparePostParam::~_DDL_DataStorePreparePostParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C47E0
nn::nex::_DDL_DataStorePreparePostParam& nn::nex::_DDL_DataStorePreparePostParam::operator=(const nn::nex::_DDL_DataStorePreparePostParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Size = other.m_Size;
    m_Name = other.m_Name;
    m_DataType = other.m_DataType;
    m_MetaBinary = other.m_MetaBinary;
    m_Permission = other.m_Permission;
    m_DelPermission = other.m_DelPermission;
    m_Flag = other.m_Flag;
    m_Period = other.m_Period;
    m_ReferDataId = other.m_ReferDataId;
    m_Tags = other.m_Tags;
    m_RatingInitParams = other.m_RatingInitParams;
    m_PersistenceInitParam = other.m_PersistenceInitParam;
    m_ExtraData = other.m_ExtraData;
    return *this;
}

// 0x003C433C
void nn::nex::_DDL_DataStorePreparePostParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePreparePostParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Size), sizeof(u32));
    _Type_string::Add(pMessage, data.m_Name);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
    _Type_qBuffer::Add(pMessage, data.m_MetaBinary);
    DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_Permission);
    DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_DelPermission);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Flag), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Period), sizeof(u16));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferDataId), sizeof(u32));
    _Type_qvector<_Type_string>::Add(pMessage, data.m_Tags);
    _Type_qvector<DDLClassMetaType<DataStoreRatingInitParamWithSlot>>::Add(pMessage, data.m_RatingInitParams);
    DDLClassMetaType<DataStorePersistenceInitParam>::Add(pMessage, data.m_PersistenceInitParam);
    _Type_qvector<_Type_string>::Add(pMessage, data.m_ExtraData);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
