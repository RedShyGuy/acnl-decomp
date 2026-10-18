#include "nn/nex/nex_DataStoreChangeMetaCompareParam.h"

namespace nn {
namespace nex {
// 0x003C5B44 slot 0x00
// 0x003C5B34 (deleting dtor)
nn::nex::DataStoreChangeMetaCompareParam::~DataStoreChangeMetaCompareParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C5974
nn::nex::DataStoreChangeMetaCompareParam::DataStoreChangeMetaCompareParam()
{
    m_ComparisonFlag = 0;
    m_Name = L"";
    m_Permission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_DelPermission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_Period = DEFAULT_PERIOD;
    m_Tags = qVector<String>();
    m_MetaBinary.clear();
    m_ReferredCnt = 0;
    m_DataType = 0;
    m_Status = 0;
}

} // namespace nex
} // namespace nn
