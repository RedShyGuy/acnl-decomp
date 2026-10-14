#include "nn/nex/nex__DDL_DataStorePersistenceTarget.h"
#include "nn/nex/nex_DataStorePersistenceTarget.h"

namespace nn {
namespace nex {
// 0x003C6DE4 slot 0x00
// 0x003C6DE0 (deleting dtor)
nn::nex::_DDL_DataStorePersistenceTarget::~_DDL_DataStorePersistenceTarget()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C6DE8
nn::nex::_DDL_DataStorePersistenceTarget& nn::nex::_DDL_DataStorePersistenceTarget::operator=(const nn::nex::_DDL_DataStorePersistenceTarget& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_OwnerId = other.m_OwnerId;
    m_PersistenceSlotId = other.m_PersistenceSlotId;
    return *this;
}

} // namespace nex
} // namespace nn
