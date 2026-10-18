#include "nn/nex/nex_DataStorePrepareGetParam.h"

namespace nn {
namespace nex {
// 0x003B2464 slot 0x00
// 0x003B2414 (deleting dtor)
nn::nex::DataStorePrepareGetParam::~DataStorePrepareGetParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B2348 | fefates:callgraph [tier C]
nn::nex::DataStorePrepareGetParam::DataStorePrepareGetParam()
{
    m_DataId = 0;
    m_PersistenceTarget = DataStorePersistenceTarget();
    m_AccessPassword = 0;
    m_LockId = 0;
    m_ExtraData.clear();
}

} // namespace nex
} // namespace nn
