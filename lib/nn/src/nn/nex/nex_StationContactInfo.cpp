#include "nn/nex/nex_StationContactInfo.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x00390314 | fefates:bytes [tier B]
nn::nex::StationContactInfo::StationContactInfo(const nn::nex::qList<nn::nex::StationURL>& urls)
{
    m_Rvcid = 0;
    for (qList<StationURL>::Node* pNode = urls.GetFirst(); !urls.IsEnd(pNode); pNode = pNode->m_pNext) {
        const StationURL& url = pNode->m_Value;
        if (url.GetParamValue(L"RVCID", 0) != 0) {
            u32 rvcid = url.GetParamValue(L"RVCID", 0);
            if (m_Rvcid != 0 && m_Rvcid != rvcid) {
                continue;
            }
            m_Rvcid = rvcid;
        }
        m_URLs.push_back(url);
    }
}

// 0x003903E0 slot 0x00 | fefates:bytes
// 0x003903B0 (deleting dtor)
nn::nex::StationContactInfo::~StationContactInfo()
{
    // nothing to do
}

// 0x0072C2B8 | fefates:bytes [tier B]
void nn::nex::StationContactInfo::SortAndFilterTarget(nn::nex::StationContactInfo& target) const
{
    bool isSameNAT = false;
    u32 publicCount = 0;
    for (qList<StationURL>::Node* pTarget = target.m_URLs.GetFirst(); !target.m_URLs.IsEnd(pTarget) && !isSameNAT; pTarget = pTarget->m_pNext) {
        if (!(pTarget->m_Value.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
            continue;
        }
        for (qList<StationURL>::Node* pNode = m_URLs.GetFirst(); !m_URLs.IsEnd(pNode) && !isSameNAT; pNode = pNode->m_pNext) {
            if (pNode->m_Value.GetAddress() == pTarget->m_Value.GetAddress()) {
                isSameNAT = true;
            }
        }
        publicCount++;
    }
    if (publicCount == 0) {
        return;
    }
    qList<StationURL> urls;
    if (isSameNAT) {
        for (qList<StationURL>::Node* pNode = target.m_URLs.GetFirst(); !target.m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
            if (!(pNode->m_Value.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC)) {
                urls.PushBackInline(pNode->m_Value);
            }
        }
    }
    for (qList<StationURL>::Node* pNode = target.m_URLs.GetFirst(); !target.m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC) {
            urls.PushBackInline(pNode->m_Value);
        }
    }
    target.m_URLs = urls;
}

} // namespace nex
} // namespace nn
