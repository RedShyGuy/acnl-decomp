#include "nn/nex/nex_DataStorePermission.h"

namespace nn {
namespace nex {
// 0x00392DE0 | fefates:bytes [tier B]
nn::nex::DataStorePermission::DataStorePermission(nn::nex::DataStoreConstants::Permission permission, const nn::nex::qVector<unsigned int>& recipientIds)
{
    m_Permission = permission;
    m_RecipientIds = recipientIds;
}

// 0x00392E50
nn::nex::DataStorePermission::DataStorePermission(const nn::nex::DataStorePermission& other) : _DDL_DataStorePermission(other)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00392F3C slot 0x00
// 0x00392F14 (deleting dtor)
nn::nex::DataStorePermission::~DataStorePermission()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D70EC
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStorePermission>::Add(Message* pMessage, const nn::nex::DataStorePermission& value)
{
    DataStorePermission::Add(pMessage, value);
}

// 0x007D71FC
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStorePermission>::Extract(Message* pMessage, nn::nex::DataStorePermission* pValue)
{
    DataStorePermission::Extract(pMessage, pValue);
}

} // namespace nex
} // namespace nn
