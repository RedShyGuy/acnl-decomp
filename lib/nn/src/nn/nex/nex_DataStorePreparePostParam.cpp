#include "nn/nex/nex_DataStorePreparePostParam.h"
#include "nn/nex/nex_DataStoreChangeMetaCompareParam.h"

namespace nn {
namespace nex {
// 0x003B6F58 slot 0x00
// 0x003B6F48 (deleting dtor)
nn::nex::DataStorePreparePostParam::~DataStorePreparePostParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B618C | fefates:bytes [tier B]
void nn::nex::DataStorePreparePostParam::SetRatingSetting(const nn::nex::qMap<signed char, nn::nex::DataStoreRatingInitParam>& settings)
{
    m_RatingInitParams.clear();
    for (qMap<s8, DataStoreRatingInitParam>::iterator it = settings.begin(); it != settings.end(); ++it) {
        DataStoreRatingInitParamWithSlot param;
        param.m_Slot = it->first;
        param.m_Param = it->second;
        m_RatingInitParams.push_back(param);
    }
}

// 0x003B674C (name is ours)
void nn::nex::DataStorePreparePostParam::Reset()
{
    m_Size = 0;
    m_Name = L"";
    m_DataType = 0;
    m_Permission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_DelPermission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_Flag = 0;
    m_Period = DataStoreChangeMetaCompareParam::DEFAULT_PERIOD;
    m_ReferDataId = 0;
    m_Tags = qVector<String>();
    m_MetaBinary.clear();
    m_PersistenceInitParam = DataStorePersistenceInitParam();
    SetRatingSetting(qMap<s8, DataStoreRatingInitParam>());
    m_ExtraData.clear();
}

// 0x003B6C3C
nn::nex::DataStorePreparePostParam::DataStorePreparePostParam(const nn::nex::DataStorePreparePostParam& other) : _DDL_DataStorePreparePostParam(other)
{
    // nothing to do
}

// 0x003B6E50 | fefates:callgraph [tier C]
nn::nex::DataStorePreparePostParam::DataStorePreparePostParam()
{
    Reset();
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00832568
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePreparePostParam, nn::nex::MemAllocator<nn::nex::DataStorePreparePostParam> >::_C_insert_aux(nn::nex::DataStorePreparePostParam* pos, const nn::nex::DataStorePreparePostParam& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std

namespace std {
// 0x0083277C
template <>
DECOMP_NOINLINE vector<nn::nex::DataStorePreparePostParam, nn::nex::MemAllocator<nn::nex::DataStorePreparePostParam> >::~vector()
{
    DestroyInline();
}
} // namespace std
