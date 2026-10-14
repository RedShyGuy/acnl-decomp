#include "nn/nex/nex__DDL_DataStorePrepareGetParam.h"
#include "nn/nex/nex_DataStorePrepareGetParam.h"

namespace nn {
namespace nex {
// 0x003C1A48 slot 0x00
// 0x003C19F8 (deleting dtor)
nn::nex::_DDL_DataStorePrepareGetParam::~_DDL_DataStorePrepareGetParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C180C
void nn::nex::_DDL_DataStorePrepareGetParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePrepareGetParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LockId), sizeof(u32));
    DDLClassMetaType<DataStorePersistenceTarget>::Add(pMessage, data.m_PersistenceTarget);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_AccessPassword), sizeof(u64));
    _Type_qvector<_Type_string>::Add(pMessage, data.m_ExtraData);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
