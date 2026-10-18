#include "nn/nex/nex_StreamTable.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"

#include <cwchar>

namespace nn {
namespace nex {
// 0x0035BA80 | mk7dlp:bytes-fuzzy [tier A]
nn::nex::ConnectionOrientedStream* nn::nex::StreamTable::FindStream(wchar_t* pType)
{
    for (qList<ConnectionOrientedStream*>::Node* pNode = m_Streams.GetFirst(); !m_Streams.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (wcscmp(pNode->m_Value->GetURLType(), pType) == 0) {
            return pNode->m_Value;
        }
    }
    return nullptr;
}

// 0x0035BAE0 | mk7dlp:bytes [tier A]
nn::nex::StreamTable::StreamTable()
{
    // nothing to do
}

// 0x0035BB98 slot 0x00 | fefates:bytes
// 0x0035BB68 (deleting dtor)
nn::nex::StreamTable::~StreamTable()
{
    // nothing to do: the list is destroyed by the compiler
}

} // namespace nex
} // namespace nn
