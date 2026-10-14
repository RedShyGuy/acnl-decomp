#include "nn/nex/nex_AnyGatheringAdapter.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_DynamicGathering.h"
#include "nn/nex/nex_Message.h"

namespace nn {
namespace nex {
// 0x00976CB4 (name is ours)
qList<Gathering*>* AnyGatheringAdapter::s_pPrototypes;
// 0x00976CB8 (name is ours)
AnyGatheringAdapter AnyGatheringAdapter::s_Instance;
// 0x00AE6B18 (name is ours)
CriticalSection AnyGatheringAdapter::s_Lock(0x20000000);
// 0x009830A8 (name is ours)
template <> AnyObjectAdapter<Gathering, String>* AnyObjectAdapter<nn::nex::Gathering, nn::nex::String>::s_pInstance = nullptr;

// 0x00391560 slot 0x00
// 0x0039155C (deleting dtor)
nn::nex::AnyGatheringAdapter::~AnyGatheringAdapter()
{
    // nothing to do: the prototypes go with the last use
}

// 0x00390FC0 | fefates:callseq [tier C]
u32 nn::nex::AnyGatheringAdapter::RegisterUse()
{
    ScopedCS lock(s_Lock);
    if (s_Instance.m_UseCount == 0) {
        s_pPrototypes = new qList<Gathering*>();
    }
    s_pInstance = &s_Instance;
    return ++s_Instance.m_UseCount;
}

// 0x003910AC | fefates:callgraph [tier C]
u32 nn::nex::AnyGatheringAdapter::UnregisterUse()
{
    ScopedCS lock(s_Lock);
    if (--s_Instance.m_UseCount == 0) {
        s_Instance.UnregisterAllPrototypes();
        s_pInstance = nullptr;
        delete s_pPrototypes;
        s_pPrototypes = nullptr;
    }
    return s_Instance.m_UseCount;
}

// 0x003911E0 | fefates:callseq [tier C]
bool nn::nex::AnyGatheringAdapter::RegisterPrototype(nn::nex::Gathering* pPrototype)
{
    ScopedCS lock(s_Lock);
    bool isRegistered = FindPrototype(pPrototype->GetGatheringType()) != nullptr;
    if (isRegistered) {
        return false;
    }
    // (the type of a clone must be the own one)
    Gathering* pClone = pPrototype->Clone();
    bool isValid;
    {
        String cloneType = pClone->GetGatheringType();
        String type = pPrototype->GetGatheringType();
        isValid = String::IsEqual(cloneType.m_pString, type.m_pString);
    }
    if (pClone != nullptr) {
        delete pClone;
    }
    if (isValid) {
        s_pPrototypes->push_back(pPrototype);
    }
    return isValid;
}

// 0x003913C4 (name is ours)
bool nn::nex::AnyGatheringAdapter::UnregisterPrototype(const nn::nex::String& type)
{
    ScopedCS lock(s_Lock);
    for (qList<Gathering*>::Node* pNode = s_pPrototypes->GetFirst(); !s_pPrototypes->IsEnd(pNode); pNode = pNode->m_pNext) {
        if (String::IsEqual(pNode->m_Value->GetGatheringType().m_pString, type.m_pString)) {
            if (pNode->m_Value != nullptr) {
                delete pNode->m_Value;
            }
            s_pPrototypes->erase(pNode);
            return true;
        }
    }
    return false;
}

// 0x003914CC (name is ours)
void nn::nex::AnyGatheringAdapter::UnregisterAllPrototypes()
{
    ScopedCS lock(s_Lock);
    while (!s_pPrototypes->IsEnd(s_pPrototypes->GetFirst())) {
        String type = s_pPrototypes->front()->GetGatheringType();
        UnregisterPrototype(type);
    }
}

// 0x0072C7D4 | fefates:callgraph [tier C]
nn::nex::Gathering* nn::nex::AnyGatheringAdapter::FindPrototype(const nn::nex::String& type) const
{
    ScopedCS lock(s_Lock);
    for (qList<Gathering*>::Node* pNode = s_pPrototypes->GetFirst(); !s_pPrototypes->IsEnd(pNode); pNode = pNode->m_pNext) {
        if (String::IsEqual(pNode->m_Value->GetGatheringType().m_pString, type.m_pString)) {
            return pNode->m_Value;
        }
    }
    return nullptr;
}

// 0x0072C890 slot 0x08 | fefates:bytes
void nn::nex::AnyGatheringAdapter::StreamIn(nn::nex::Message* pMessage, const nn::nex::Gathering& gathering) const
{
    String type = gathering.GetGatheringType();
    if (String::IsEqual(type.m_pString, L"PersistentGathering")) {
        type = L"Community";
    }
    _Type_string::Add(pMessage, type);
    // the gathering is written into an own message first: its size goes in front of it
    Message message;
    gathering.StreamIn(&message);
    Buffer* pBuffer = message.GetBuffer();
    u32 size = pBuffer->m_ContentSize;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
    pMessage->AppendRaw(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize);
}

// 0x0072C998 slot 0x0C | fefates:bytes
void nn::nex::AnyGatheringAdapter::StreamOut(nn::nex::Message* pMessage, nn::nex::Gathering** ppGathering) const
{
    String type;
    _Type_string::Extract(pMessage, &type);
    if (String::IsEqual(type.m_pString, L"Community")) {
        type = L"PersistentGathering";
    }
    Gathering* pPrototype = FindPrototype(type);
    Gathering* pGathering;
    if (pPrototype != nullptr) {
        pGathering = pPrototype->Clone();
    } else {
        Gathering* pDynamicPrototype = FindPrototype(String(L"DynamicGathering"));
        pGathering = nullptr;
        if (pDynamicPrototype != nullptr) {
            DynamicGathering* pDynamic = static_cast<DynamicGathering*>(pDynamicPrototype->Clone());
            pDynamic->m_GatheringType = type;
            pGathering = pDynamic;
        }
    }
    *ppGathering = pGathering;
    // the data of the size in front of it is read from an own message
    Buffer buffer;
    u32 size = 0;
    if (pMessage->ExtractPrimitive(&size)) {
        if (size > pMessage->m_pBuffer->m_ContentSize - pMessage->m_Position) {
            pMessage->m_IsError = true;
        } else {
            Buffer* pSource = pMessage->GetBuffer();
            buffer.AppendData(pSource->m_pData + pSource->m_HeaderSize + pMessage->m_Position, size, static_cast<unsigned>(-1));
            u32 end = pMessage->m_Position + size;
            if (pMessage->m_pBuffer->m_ContentSize >= end) {
                pMessage->m_Position = end;
            }
        }
    }
    Message message(&buffer);
    (*ppGathering)->StreamOut(&message);
}

} // namespace nex
} // namespace nn
