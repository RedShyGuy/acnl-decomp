#include "nn/nex/nex__DDL_ResultRange.h"
#include "nn/nex/nex_ResultRange.h"

namespace nn {
namespace nex {
// 0x00384100 slot 0x00
// 0x003840FC (deleting dtor)
nn::nex::_DDL_ResultRange::~_DDL_ResultRange()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00384104
nn::nex::_DDL_ResultRange& nn::nex::_DDL_ResultRange::operator=(const nn::nex::_DDL_ResultRange& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_uiOffset = other.m_uiOffset;
    m_uiSize = other.m_uiSize;
    return *this;
}

// 0x00384024
void nn::nex::_DDL_ResultRange::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_ResultRange& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiOffset), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiSize), sizeof(u32));
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
