#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

#include <cstring>

namespace nn {
namespace nex {
class String;

// An HTTP connection (the platform implementation is CTRHttpConnection). The names of the members,
// of the methods and of the functions without a reference name are ours.
// RTTI N2nn3nex14HttpConnectionE @ 0x008CE2FC
// vtable 0x008FCB10 (vptr 0x008FCB18), offset_to_top 0, 23 entries
class HttpConnection : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    // the method of the request
    enum Method
    {
        METHOD_GET = 0,
        METHOD_HEAD = 1,
        METHOD_POST = 2
    };
    // the size of the boundary of a multipart post
    static const u32 BOUNDARY_SIZE = 41;

    // (inline in the constructor of CTRHttpConnection)
    HttpConnection() : m_Unknown0x04(2), m_ThreadPriority(16), m_IsChunked(false), m_UsesRootCas(true), m_UsesClientCert(true)
    {
        memset(m_Boundary, 0, sizeof(m_Boundary));
    }
    virtual ~HttpConnection(); // 0x00371058 slot 0x00
    // 0x00371040 slot 0x04 (deleting dtor)
    virtual nn::nex::qResult Initialize(const char* pURL, Method method) = 0; // slot 0x08
    virtual nn::nex::qResult AddHeaderField(const char* pLabel, const char* pValue) = 0; // slot 0x0C
    virtual nn::nex::qResult AddPostDataAscii(const char* pLabel, const char* pValue) = 0; // slot 0x10
    // the post data is sent in chunks (multipart) of the size in total
    virtual void SetChunkedPostData(unsigned int size) = 0; // slot 0x14
    virtual bool IsChunked() const; // 0x0072AFA0 slot 0x18
    // the last chunk of the post data was pushed
    virtual void FinishPostData() = 0; // slot 0x1C
    virtual bool IsPostDataFinished() = 0; // slot 0x20
    virtual nn::nex::qResult Start() = 0; // slot 0x24
    virtual nn::nex::qResult Cancel() = 0; // slot 0x28
    // the status code (false while there is none)
    virtual bool GetStatusCode(int* pStatusCode) = 0; // slot 0x2C
    // takes up to maxSize received bytes
    virtual nn::nex::qResult ReadBody(unsigned char* pBuffer, unsigned int* pSize, unsigned int maxSize) = 0; // slot 0x30
    virtual nn::nex::qResult PushPostData(const unsigned char* pData, unsigned int size) = 0; // slot 0x34
    // the room for post data
    virtual unsigned int GetPostDataFreeSize() = 0; // slot 0x38
    virtual nn::nex::qResult GetResult() const = 0; // slot 0x3C
    // the result, a failure while there is no status code
    virtual nn::nex::qResult GetStatus() = 0; // slot 0x40
    // everything was received and taken
    virtual bool IsFinished() = 0; // slot 0x44
    virtual nn::nex::qResult AddPostDataBinary(const char* pLabel, const unsigned char* pData, unsigned int size) = 0; // slot 0x48
    virtual bool GetHeaderField(const char* pLabel, nn::nex::String* pValue) = 0; // slot 0x4C
    virtual void SetUnknown0x10CE(); // 0x0037103C slot 0x50
    virtual bool AddRootCa(const unsigned char* pCertificate, int size) = 0; // slot 0x54
    // the size of the receive buffer
    virtual void SetBufferSize(unsigned int size) = 0; // slot 0x58

    // the boundary of a multipart post (made from the MD5 of the time)
    const char* GetBoundaryString() const; // 0x0072AFA8 | fefates:bytes [tier B]

    u8 m_Unknown0x04;                   // 0x04
    s32 m_ThreadPriority;               // 0x08
    bool m_IsChunked;                   // 0x0C
    bool m_UsesRootCas;                 // 0x0D (the internal root CAs 1 to 5)
    bool m_UsesClientCert;              // 0x0E
    mutable char m_Boundary[BOUNDARY_SIZE]; // 0x0F
};
ASSERT_SIZE(HttpConnection, 0x38);
} // namespace nex
} // namespace nn
