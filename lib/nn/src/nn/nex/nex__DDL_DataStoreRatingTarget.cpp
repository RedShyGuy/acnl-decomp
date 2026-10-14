#include "nn/nex/nex__DDL_DataStoreRatingTarget.h"
#include "nn/nex/nex_DataStoreRatingTarget.h"

namespace nn {
namespace nex {
// 0x003BBCEC
void nn::nex::_DDL_DataStoreRatingTarget::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingTarget& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Slot), sizeof(s8));
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
