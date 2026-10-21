#include "nn/nex/nex__DDL_DataStorePersistenceInitParam.h"
#include "nn/nex/nex_DataStorePersistenceInitParam.h"

namespace nn {
namespace nex {
// 0x003CB928 slot 0x00
// 0x003CB924 (deleting dtor)
nn::nex::_DDL_DataStorePersistenceInitParam::~_DDL_DataStorePersistenceInitParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CB92C
nn::nex::_DDL_DataStorePersistenceInitParam& nn::nex::_DDL_DataStorePersistenceInitParam::operator=(const nn::nex::_DDL_DataStorePersistenceInitParam& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_PersistenceSlotId = other.m_PersistenceSlotId;
    m_DeleteLastObject = other.m_DeleteLastObject;
    return *this;
}

} // namespace nex
} // namespace nn
