#include "nn/nex/nex_DataStorePreparePostParamV1.h"
#include "nn/nex/nex_DataStoreChangeMetaCompareParam.h"

namespace nn {
namespace nex {
// 0x003BCF1C slot 0x00
// 0x003BCF0C (deleting dtor)
nn::nex::DataStorePreparePostParamV1::~DataStorePreparePostParamV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BCD14
nn::nex::DataStorePreparePostParamV1::DataStorePreparePostParamV1()
{
    m_Size = 0;
    m_Name = "";
    m_DataType = 0;
    m_Permission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_DelPermission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_Flag = 0;
    m_Period = DataStoreChangeMetaCompareParam::DEFAULT_PERIOD;
    m_ReferDataId = 0;
    m_Tags = qVector<String>();
    m_MetaBinary.clear();
}

} // namespace nex
} // namespace nn
