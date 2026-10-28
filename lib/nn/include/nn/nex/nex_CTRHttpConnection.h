#pragma once

#include "decomp.h"
#include "nn/http/http_Connection.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_qReservedQueue.h"
#include "nn/nex/nex_qVector.h"
#include "nn/os/os_Thread.h"

namespace nn {
namespace nex {
// The HTTP connection over nn::http: a thread sends the post data and receives the body into a
// ring buffer. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex17CTRHttpConnectionE @ 0x008CE668
// vtable 0x008FD32C (vptr 0x008FD334), offset_to_top 0, 23 entries
class CTRHttpConnection : public ::nn::nex::HttpConnection
{
public:
    // the stack of the thread
    static const u32 STACK_SIZE = 0x1050 - 0x50;

    CTRHttpConnection(); // 0x003858AC
    virtual ~CTRHttpConnection(); // 0x003859E4 slot 0x00 | fefates:callseq
    // 0x003859B4 slot 0x04 (deleting dtor)
    virtual nn::nex::qResult Initialize(const char* pURL, Method method); // 0x0038483C slot 0x08
    virtual nn::nex::qResult AddHeaderField(const char* pLabel, const char* pValue); // 0x00384DFC slot 0x0C
    virtual nn::nex::qResult AddPostDataAscii(const char* pLabel, const char* pValue); // 0x00384964 slot 0x10
    virtual void SetChunkedPostData(unsigned int size); // 0x00384E58 slot 0x14
    virtual void FinishPostData(); // 0x0038502C slot 0x1C
    virtual bool IsPostDataFinished(); // 0x0072C0E8 slot 0x20
    virtual nn::nex::qResult Start(); // 0x003849C0 slot 0x24 | mk7dlp:callseq
    virtual nn::nex::qResult Cancel(); // 0x00385804 slot 0x28
    virtual bool GetStatusCode(int* pStatusCode); // 0x0072BD78 slot 0x2C
    virtual nn::nex::qResult ReadBody(unsigned char* pBuffer, unsigned int* pSize, unsigned int maxSize); // 0x003856B4 slot 0x30
    virtual nn::nex::qResult PushPostData(const unsigned char* pData, unsigned int size); // 0x00384EA4 slot 0x34
    virtual unsigned int GetPostDataFreeSize(); // 0x0072BE0C slot 0x38
    virtual nn::nex::qResult GetResult() const; // 0x0072C1BC slot 0x3C
    virtual nn::nex::qResult GetStatus(); // 0x0072C12C slot 0x40
    virtual bool IsFinished(); // 0x0072BD04 slot 0x44
    virtual nn::nex::qResult AddPostDataBinary(const char* pLabel, const unsigned char* pData, unsigned int size); // 0x00384900 slot 0x48
    virtual bool GetHeaderField(const char* pLabel, nn::nex::String* pValue); // 0x0072BE78 slot 0x4C
    virtual void SetUnknown0x10CE(); // 0x00385070 slot 0x50
    virtual bool AddRootCa(const unsigned char* pCertificate, int size); // 0x00385894 slot 0x54 | mk7dlp:bytes
    virtual void SetBufferSize(unsigned int size); // 0x003850B4 slot 0x58

    // the result of nn::http as a result of nex
    static DECOMP_NOINLINE nn::nex::qResult ConvertResult(nn::Result result); // 0x00384F94
    // the thread (only a nop in the original that falls into Run)
    // (the parameter is the connection; uptr: the thread shares the TypeInfo of the uptr threads)
    static void ThreadProc(uptr arg); // 0x003850C0
    void Run(); // 0x003850C4

    nn::http::Connection m_Connection;    // 0x0038
    nn::os::Thread m_Thread;              // 0x0048
    u8 m_Stack[STACK_SIZE];               // 0x0050
    qResult m_Result;                     // 0x1050
    ForcedCriticalSection m_Lock;         // 0x105C
    bool m_IsDone;                        // 0x1084
    bool m_IsCanceled;                    // 0x1085
    bool m_IsPostDataFinished;            // 0x1086
    bool m_IsPostDataSent;                // 0x1087
    bool m_HasStatusCode;                 // 0x1088
    s32 m_StatusCode;                     // 0x108C
    char m_ContentLength[32];             // 0x1090
    u32 m_BufferSize;                     // 0x10B0
    qReservedQueue<u8>* m_pQueue;         // 0x10B4 (the post data, then the received body)
    qVector<u8> m_Data;                   // 0x10B8 (the data to send / just received)
    u32 m_PostDataSize;                   // 0x10C4
    u32 m_ThreadId;                       // 0x10C8
    bool m_IsThreadStarted;               // 0x10CC
    bool m_IsFinalized;                   // 0x10CD
    s8 m_Unknown0x10CE;                   // 0x10CE
};
ASSERT_OFFSET(CTRHttpConnection, m_Thread, 0x48);
ASSERT_OFFSET(CTRHttpConnection, m_Result, 0x1050);
ASSERT_OFFSET(CTRHttpConnection, m_Lock, 0x105C);
ASSERT_OFFSET(CTRHttpConnection, m_IsDone, 0x1084);
ASSERT_OFFSET(CTRHttpConnection, m_ContentLength, 0x1090);
ASSERT_OFFSET(CTRHttpConnection, m_Data, 0x10B8);
ASSERT_SIZE(CTRHttpConnection, 0x10D0);
} // namespace nex
} // namespace nn
