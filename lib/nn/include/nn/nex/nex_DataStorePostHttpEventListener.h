#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStoreKeyValue;
class DataStorePostObjectEventListener;

// The listener of the HTTP post of a DataStore object: the header fields, a multipart form with the
// form fields and the data (from memory or from the application) as "file". The member names are
// ours.
// RTTI N2nn3nex30DataStorePostHttpEventListenerE @ 0x008CF190
// vtable 0x008FF384 (vptr 0x008FF38C), offset_to_top 0, 6 entries
class DataStorePostHttpEventListener : public ::nn::nex::HttpEventListener, public ::nn::nex::NonCopyable, public ::nn::nex::RootObject
{
public:
    DataStorePostHttpEventListener(unsigned callContextId, const nn::nex::qVector<nn::nex::DataStoreKeyValue>& headerFields, const nn::nex::qVector<nn::nex::DataStoreKeyValue>& formFields,
                                   const nn::nex::qVector<unsigned char>& rootCa, unsigned threadPriority, unsigned bufferSize, const unsigned char* pData, unsigned dataSize,
                                   nn::nex::DataStorePostObjectEventListener* pListener); // 0x003C23CC | mk7dlp:bytes [tier A]
    virtual ~DataStorePostHttpEventListener(); // 0x003C2450 slot 0x00
    // 0x003C2420 slot 0x04 (deleting dtor)
    virtual void PrepareRequest(nn::nex::HttpConnection* pConnection); // 0x003C1D00 slot 0x08 | fefates:bytes-fuzzy
    virtual unsigned int PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast); // 0x003C20F8 slot 0x0C | fefates:bytes
    virtual void ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode); // 0x00386D6C slot 0x10
    virtual bool ProcessResponse(const unsigned char* pData, unsigned int size); // 0x00386D50 slot 0x14

    u32 m_CallContextId;                                // 0x04
    const qVector<DataStoreKeyValue>* m_pHeaderFields;  // 0x08
    const qVector<DataStoreKeyValue>* m_pFormFields;    // 0x0C
    const qVector<u8>* m_pRootCa;                       // 0x10
    u32 m_ThreadPriority;                               // 0x14
    u32 m_BufferSize;                                   // 0x18
    const u8* m_pData;                                  // 0x1C (null: from the listener)
    u32 m_DataSize;                                     // 0x20
    DataStorePostObjectEventListener* m_pListener;      // 0x24
    u32 m_Offset;                                       // 0x28 (of the data sent so far)
    qVector<u8> m_Header;                               // 0x2C (of the form, up to the data)
    qVector<u8> m_Footer;                               // 0x38 (after the data)
    bool m_IsDataSent;                                  // 0x44
};
ASSERT_OFFSET(DataStorePostHttpEventListener, m_Header, 0x2C);
ASSERT_SIZE(DataStorePostHttpEventListener, 0x48);
} // namespace nex
} // namespace nn
