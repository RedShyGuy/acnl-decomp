#include "nn/nex/nex_DataStoreSearchParam.h"
#include "nn/nex/nex_DataStoreConstants.h"

namespace nn {
namespace nex {
// 0x003967B0 slot 0x00
// 0x003967A0 (deleting dtor)
nn::nex::DataStoreSearchParam::~DataStoreSearchParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00396384 (name is ours)
void nn::nex::DataStoreSearchParam::SetDataTypes(const nn::nex::qVector<unsigned short>& dataTypes)
{
    m_DataTypes = dataTypes;
}

// 0x00396564
nn::nex::DataStoreSearchParam::DataStoreSearchParam()
{
    m_SearchTarget = 1;
    m_OwnerIds = qVector<u32>();
    m_OwnerType = 0;
    m_DestinationIds = qVector<u32>();
    m_DataType = 0xFFFF;
    SetDataTypes(qVector<u16>());
    m_CreatedAfter = g_DataStoreMaxTime;
    m_CreatedBefore = g_DataStoreMaxTime;
    m_UpdatedAfter = g_DataStoreMaxTime;
    m_UpdatedBefore = g_DataStoreMaxTime;
    m_ReferDataId = 0;
    m_ResultOrderColumn = 0;
    m_ResultOrder = 0;
    m_ResultRange = ResultRange(0, ResultRange::DEFAULT_SIZE);
    m_ResultOption = 0;
    m_Tags = qVector<String>();
    m_MinimalRatingFrequency = 0;
    m_UseCache = false;
    m_TotalCountEnabled = false;
}

// 0x003B8754
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreSearchParam>::Add(Message* pMessage, const nn::nex::DataStoreSearchParam& value)
{
    DataStoreSearchParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn
