#include "nn/nex/nex__DDL_DataStorePermission.h"
#include "nn/nex/nex_DataStorePermission.h"

namespace nn {
namespace nex {
// 0x003B57A8 slot 0x00
// 0x003B5780 (deleting dtor)
nn::nex::_DDL_DataStorePermission::~_DDL_DataStorePermission()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B57CC
nn::nex::_DDL_DataStorePermission& nn::nex::_DDL_DataStorePermission::operator=(const nn::nex::_DDL_DataStorePermission& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Permission = other.m_Permission;
    m_RecipientIds = other.m_RecipientIds;
    return *this;
}

} // namespace nex
} // namespace nn
