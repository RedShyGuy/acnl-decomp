#include "nn/nex/nex_URLProbeList.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x00361678 | fefates:bytes [tier B]
nn::nex::URLProbe* nn::nex::URLProbeList::FindProbe(const nn::nex::StationURL& url)
{
    for (Node* pNode = GetFirst(); !IsEnd(pNode); pNode = pNode->m_pNext) {
        bool isFound = false;
        if (pNode->m_Value.m_URL.GetAddress() == url.GetAddress()) {
            isFound = pNode->m_Value.m_URL.GetParamValue(L"RVCID", 0) == url.GetParamValue(L"RVCID", 0);
        }
        if (isFound) {
            return &pNode->m_Value;
        }
    }
    return nullptr;
}

// 0x003617A0 slot 0x00 | fefates:bytes
// 0x00361770 (deleting dtor)
nn::nex::URLProbeList::~URLProbeList()
{
    // nothing to do: the list is destroyed by the base
}

// 0x008302D8
template <>
DECOMP_NOINLINE void qList<URLProbe>::AddBuffer(bool isFirst)
{
    AddBufferInline(isFirst);
}

// 0x00830350
template <>
DECOMP_NOINLINE void qList<URLProbe>::push_back(const URLProbe& value)
{
    PushBackInline(value);
}

} // namespace nex
} // namespace nn
