#include "nn/nex/nex__DDL_DataStoreRatingInitParam.h"
#include "nn/nex/nex_DataStoreRatingInitParam.h"

namespace nn {
namespace nex {
// 0x003C1CB0 slot 0x00
// 0x003C1CAC (deleting dtor)
nn::nex::_DDL_DataStoreRatingInitParam::~_DDL_DataStoreRatingInitParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C1CB4
nn::nex::_DDL_DataStoreRatingInitParam& nn::nex::_DDL_DataStoreRatingInitParam::operator=(const nn::nex::_DDL_DataStoreRatingInitParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Flag = other.m_Flag;
    m_InternalFlag = other.m_InternalFlag;
    m_LockType = other.m_LockType;
    m_InitialValue = other.m_InitialValue;
    m_RangeMin = other.m_RangeMin;
    m_RangeMax = other.m_RangeMax;
    m_PeriodHour = other.m_PeriodHour;
    m_PeriodDuration = other.m_PeriodDuration;
    return *this;
}

// 0x003C1B74
void nn::nex::_DDL_DataStoreRatingInitParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingInitParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Flag), sizeof(u8));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_InternalFlag), sizeof(u8));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LockType), sizeof(u8));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_InitialValue), sizeof(s64));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_RangeMin), sizeof(s32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_RangeMax), sizeof(s32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PeriodHour), sizeof(s8));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PeriodDuration), sizeof(s16));
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
