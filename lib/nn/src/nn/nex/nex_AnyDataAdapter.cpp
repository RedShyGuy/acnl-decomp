#include "nn/nex/nex_AnyDataAdapter.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Message.h"

namespace nn {
namespace nex {
// 0x009712EC (name is ours)
u32 AnyDataAdapter::s_UseCount;
// 0x009712F0 (name is ours)
qList<Data*>* AnyDataAdapter::s_pPrototypes;
// 0x009712F4 (name is ours)
AnyDataAdapter AnyDataAdapter::s_Instance;
// 0x00AE084C (name is ours)
CriticalSection AnyDataAdapter::s_Lock(0x20000000);
// 0x009830A4 (name is ours)
template <> AnyObjectAdapter<Data, String>* AnyObjectAdapter<nn::nex::Data, nn::nex::String>::s_pInstance = nullptr;

// 0x00370800 slot 0x00 | fefates:bytes
// 0x003707C8 (deleting dtor)
nn::nex::AnyDataAdapter::~AnyDataAdapter()
{
    if (s_pPrototypes != nullptr) {
        UnregisterAllPrototypes();
    }
}

// 0x003706E0 | fefates:callgraph [tier C]
void nn::nex::AnyDataAdapter::Register()
{
    ScopedCS lock(s_Lock);
    if (s_UseCount == 0) {
        s_pPrototypes = new qList<Data*>();
    }
    s_UseCount++;
    s_pInstance = &s_Instance;
}

// 0x00370260 | fefates:callgraph [tier C]
void nn::nex::AnyDataAdapter::Unregister()
{
    ScopedCS lock(s_Lock);
    if (--s_UseCount == 0) {
        delete s_pPrototypes;
        s_pPrototypes = nullptr;
        s_pInstance = nullptr;
    }
}

// 0x00370380 (name is ours)
bool nn::nex::AnyDataAdapter::RegisterPrototype(nn::nex::Data* pPrototype)
{
    bool isRegistered = FindPrototype(pPrototype->GetDataType()) != nullptr;
    if (isRegistered) {
        return false;
    }
    // (the type of a clone must be the own one)
    Data* pClone = pPrototype->Clone();
    bool isValid;
    {
        String cloneType = pClone->GetDataType();
        String type = pPrototype->GetDataType();
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

// 0x00370518 (name is ours)
bool nn::nex::AnyDataAdapter::UnregisterPrototype(const nn::nex::String& type)
{
    for (qList<Data*>::Node* pNode = s_pPrototypes->GetFirst(); !s_pPrototypes->IsEnd(pNode); pNode = pNode->m_pNext) {
        if (String::IsEqual(pNode->m_Value->GetDataType().m_pString, type.m_pString)) {
            if (pNode->m_Value != nullptr) {
                delete pNode->m_Value;
            }
            s_pPrototypes->erase(pNode);
            return true;
        }
    }
    return false;
}

// 0x003705E8 | fefates:bytes [tier B]
void nn::nex::AnyDataAdapter::UnregisterAllPrototypes()
{
    while (!s_pPrototypes->IsEnd(s_pPrototypes->GetFirst())) {
        String type = s_pPrototypes->front()->GetDataType();
        UnregisterPrototype(type);
    }
}

// 0x0072AB70 | fefates:bytes [tier B]
nn::nex::Data* nn::nex::AnyDataAdapter::FindPrototype(const nn::nex::String& type) const
{
    for (qList<Data*>::Node* pNode = s_pPrototypes->GetFirst(); !s_pPrototypes->IsEnd(pNode); pNode = pNode->m_pNext) {
        if (String::IsEqual(pNode->m_Value->GetDataType().m_pString, type.m_pString)) {
            return pNode->m_Value;
        }
    }
    return nullptr;
}

// 0x0072ABF0 slot 0x08 | fefates:bytes
void nn::nex::AnyDataAdapter::StreamIn(nn::nex::Message* pMessage, const nn::nex::Data& data) const
{
    _Type_string::Add(pMessage, data.GetDataType());
    // the data is written into an own message first: its size goes in front of it
    Message message;
    message.m_IsWithoutStructureHeader = pMessage->m_IsWithoutStructureHeader;
    data.StreamIn(&message);
    Buffer* pBuffer = message.GetBuffer();
    u32 size = pBuffer->m_ContentSize;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
    pMessage->AppendRaw(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize);
}

// 0x0072AC9C slot 0x0C | fefates:bytes
void nn::nex::AnyDataAdapter::StreamOut(nn::nex::Message* pMessage, nn::nex::Data** ppData) const
{
    String type;
    _Type_string::Extract(pMessage, &type);
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    Data* pPrototype = FindPrototype(type);
    Data* pData;
    if (pPrototype != nullptr) {
        pData = pPrototype->Clone();
    } else {
        Data* pDynamicPrototype = FindPrototype(String(L"DynamicData"));
        pData = nullptr;
        if (pDynamicPrototype != nullptr) {
            DynamicData* pDynamic = static_cast<DynamicData*>(pDynamicPrototype->Clone());
            pDynamic->m_DataType = type;
            pDynamic->m_IsWithoutStructureHeader = isWithoutHeader;
            pData = pDynamic;
        }
    }
    *ppData = pData;
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
    message.m_IsWithoutStructureHeader = pMessage->m_IsWithoutStructureHeader;
    (*ppData)->StreamOut(&message);
}

} // namespace nex
} // namespace nn
