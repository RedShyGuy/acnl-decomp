#include "nn/nex/nex__DDL_DataStorePrepareUpdateParam.h"
#include "nn/nex/nex_DataStorePrepareUpdateParam.h"

namespace nn {
namespace nex {
// 0x003CADB8 slot 0x00
// 0x003CAD68 (deleting dtor)
nn::nex::_DDL_DataStorePrepareUpdateParam::~_DDL_DataStorePrepareUpdateParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CAC3C
void nn::nex::_DDL_DataStorePrepareUpdateParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePrepareUpdateParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Size), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_UpdatePassword), sizeof(u64));
    _Type_qvector<_Type_string>::Add(pMessage, data.m_ExtraData);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
