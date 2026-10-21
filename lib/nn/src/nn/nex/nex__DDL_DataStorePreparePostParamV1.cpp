#include "nn/nex/nex__DDL_DataStorePreparePostParamV1.h"
#include "nn/nex/nex_DataStorePreparePostParamV1.h"

namespace nn {
namespace nex {
// 0x003CAB80 slot 0x00
// 0x003CAB70 (deleting dtor)
nn::nex::_DDL_DataStorePreparePostParamV1::~_DDL_DataStorePreparePostParamV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CA8DC
void nn::nex::_DDL_DataStorePreparePostParamV1::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePreparePostParamV1& data)
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
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
