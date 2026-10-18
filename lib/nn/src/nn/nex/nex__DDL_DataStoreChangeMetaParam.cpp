#include "nn/nex/nex__DDL_DataStoreChangeMetaParam.h"
#include "nn/nex/nex_DataStoreChangeMetaParam.h"

namespace nn {
namespace nex {
// 0x003C15BC
nn::nex::_DDL_DataStoreChangeMetaParam& nn::nex::_DDL_DataStoreChangeMetaParam::operator=(const nn::nex::_DDL_DataStoreChangeMetaParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_DataId = other.m_DataId;
    m_ModifiesFlag = other.m_ModifiesFlag;
    m_Name = other.m_Name;
    m_Permission = other.m_Permission;
    m_DelPermission = other.m_DelPermission;
    m_Period = other.m_Period;
    m_MetaBinary = other.m_MetaBinary;
    m_Tags = other.m_Tags;
    m_UpdatePassword = other.m_UpdatePassword;
    m_ReferredCnt = other.m_ReferredCnt;
    m_DataType = other.m_DataType;
    m_Status = other.m_Status;
    m_CompareParam = other.m_CompareParam;
    m_PersistenceTarget = other.m_PersistenceTarget;
    return *this;
}

// 0x003C1530 slot 0x00
// 0x003C1520 (deleting dtor)
nn::nex::_DDL_DataStoreChangeMetaParam::~_DDL_DataStoreChangeMetaParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
