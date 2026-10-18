#include "nn/nex/nex_DataStorePostHttpEventListener.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_DataStorePostObjectEventListener.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_StringStream.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the call context of the listener
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}

// (inline) the text goes to the end of the buffer (as UTF-8)
inline void AppendText(qVector<u8>* pBuffer, const StringStream& stream)
{
    String text(stream.m_pBuffer);
    char* pText;
    text.CreateCopy(&pText);
    pBuffer->insert(pBuffer->m_pEnd, pText, pText + strlen(pText));
    String::ReleaseCopy(pText);
}

// (inline) moves up to size bytes from the front of the buffer
inline u32 TakeFront(qVector<u8>* pBuffer, u8* pDestination, u32 size)
{
    u32 available = pBuffer->size();
    u32 count = size < available ? size : available;
    u8* pFirst = pBuffer->m_pBegin;
    u8* pLast = pFirst + count;
    for (u8* p = pFirst; p != pLast; p++) {
        *pDestination++ = *p;
    }
    if (pBuffer->m_pBegin != pBuffer->m_pEnd) {
        u8* pEnd = pBuffer->m_pEnd;
        u8* pTo = pBuffer->m_pBegin;
        for (u8* p = pLast; p != pEnd; p++) {
            *pTo++ = *p;
        }
        pBuffer->m_pEnd -= pLast - pBuffer->m_pBegin;
    }
    return count;
}
} // namespace

// 0x003C23CC | mk7dlp:bytes [tier A]
nn::nex::DataStorePostHttpEventListener::DataStorePostHttpEventListener(unsigned callContextId, const nn::nex::qVector<nn::nex::DataStoreKeyValue>& headerFields,
                                                                       const nn::nex::qVector<nn::nex::DataStoreKeyValue>& formFields, const nn::nex::qVector<unsigned char>& rootCa,
                                                                       unsigned threadPriority, unsigned bufferSize, const unsigned char* pData, unsigned dataSize,
                                                                       nn::nex::DataStorePostObjectEventListener* pListener)
    : m_CallContextId(callContextId), m_pHeaderFields(&headerFields), m_pFormFields(&formFields), m_pRootCa(&rootCa), m_ThreadPriority(threadPriority), m_BufferSize(bufferSize),
      m_pData(pData), m_DataSize(dataSize), m_pListener(pListener), m_Offset(0), m_IsDataSent(false)
{
    // nothing to do
}

// 0x003C2450 slot 0x00
// 0x003C2420 (deleting dtor)
nn::nex::DataStorePostHttpEventListener::~DataStorePostHttpEventListener()
{
    // nothing to do: the members are destroyed by the compiler
}

// 0x003C1D00 slot 0x08 | fefates:bytes-fuzzy
void nn::nex::DataStorePostHttpEventListener::PrepareRequest(nn::nex::HttpConnection* pConnection)
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        return;
    }
    pConnection->m_ThreadPriority = m_ThreadPriority;
    pConnection->SetBufferSize(m_BufferSize);
    for (const DataStoreKeyValue* p = m_pHeaderFields->begin(); p != m_pHeaderFields->end(); p++) {
        char* pLabel;
        char* pValue;
        p->m_Key.CreateCopy(&pLabel);
        p->m_Value.CreateCopy(&pValue);
        pConnection->AddHeaderField(pLabel, pValue);
        String::ReleaseCopy(pLabel);
        String::ReleaseCopy(pValue);
    }
    if (m_pRootCa->begin() != m_pRootCa->end()) {
        pConnection->AddRootCa(m_pRootCa->begin(), m_pRootCa->size());
    }
    // the form: the fields, then the data as "file"
    for (const DataStoreKeyValue* p = m_pFormFields->begin(); p != m_pFormFields->end(); p++) {
        String boundary(pConnection->GetBoundaryString());
        StringStream stream;
        stream << L"--" << boundary << L"\r\n" << L"Content-Disposition: form-data; name=\"" << p->m_Key << L"\"" << L"\r\n" << L"\r\n" << p->m_Value << L"\r\n";
        AppendText(&m_Header, stream);
    }
    {
        String boundary(pConnection->GetBoundaryString());
        StringStream stream;
        stream << L"--" << boundary << L"\r\n" << L"Content-Disposition: form-data; name=\"file\"" << L"\r\n" << L"\r\n";
        AppendText(&m_Header, stream);
    }
    {
        String boundary(pConnection->GetBoundaryString());
        StringStream stream;
        stream << L"\r\n" << L"--" << boundary << L"--" << L"\r\n";
        AppendText(&m_Footer, stream);
    }
    pConnection->SetChunkedPostData(m_Header.size() + m_Footer.size() + m_DataSize);
}

// 0x003C20F8 slot 0x0C | fefates:bytes
unsigned int nn::nex::DataStorePostHttpEventListener::PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast)
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        return 0;
    }
    if (m_IsDataSent) {
        // the footer
        u32 count = TakeFront(&m_Footer, pBuffer, size);
        if (m_Footer.begin() == m_Footer.end()) {
            if (m_Offset != m_DataSize) {
                goto cancel;
            }
            *pIsLast = true;
        }
        return count;
    }
    if (m_Header.begin() != m_Header.end()) {
        return TakeFront(&m_Header, pBuffer, size);
    }
    {
        // the data
        u32 count;
        if (m_pData != nullptr) {
            u32 rest = m_DataSize - m_Offset;
            count = rest > size ? size : rest;
            memcpy(pBuffer, m_pData + m_Offset, count);
            if (m_Offset + count == m_DataSize) {
                m_IsDataSent = true;
            }
        } else {
            count = m_pListener->GetData(pBuffer, size, &m_IsDataSent);
        }
        m_Offset += count;
        if (m_Offset <= m_DataSize) {
            return count;
        }
    }
cancel:
    pContext->Cancel(CallContext::STATE_CANCELLED);
    return 0;
}

// 0x00386D6C slot 0x10
void nn::nex::DataStorePostHttpEventListener::ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode)
{
    // nothing to do
}

// 0x00386D50 slot 0x14
bool nn::nex::DataStorePostHttpEventListener::ProcessResponse(const unsigned char* pData, unsigned int size)
{
    return true;
}

} // namespace nex
} // namespace nn
