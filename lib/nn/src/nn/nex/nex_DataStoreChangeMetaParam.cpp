#include "nn/nex/nex_DataStoreChangeMetaParam.h"

namespace nn {
namespace nex {
// 0x003B22E0 slot 0x00
// 0x003B22D0 (deleting dtor)
nn::nex::DataStoreChangeMetaParam::~DataStoreChangeMetaParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B2084
nn::nex::DataStoreChangeMetaParam::DataStoreChangeMetaParam()
{
    m_DataId = 0;
    m_ModifiesFlag = 0;
    m_Name = L"";
    m_Permission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_DelPermission = DataStorePermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>());
    m_Period = DataStoreChangeMetaCompareParam::DEFAULT_PERIOD;
    m_Tags = qVector<String>();
    m_MetaBinary.clear();
    m_UpdatePassword = 0;
    m_ReferredCnt = 0;
    m_DataType = 0;
    m_Status = 0;
    m_CompareParam = DataStoreChangeMetaCompareParam();
    m_PersistenceTarget = DataStorePersistenceTarget();
}

// 0x003C12A0
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreChangeMetaParam>::Add(Message* pMessage, const nn::nex::DataStoreChangeMetaParam& value)
{
    DataStoreChangeMetaParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00831E80
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreChangeMetaParam, nn::nex::MemAllocator<nn::nex::DataStoreChangeMetaParam> >::_C_insert_aux(nn::nex::DataStoreChangeMetaParam* pos, const nn::nex::DataStoreChangeMetaParam& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
