#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/http/http_ConnectionIpc.h"
#include "nn/http/http_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace http {
// RTTI N2nn4http10ConnectionE @ 0x008D03A4
// vtable 0x009021EC (vptr 0x009021F4), offset_to_top 0, 2 entries
// A request of http:C on its own session. The functions without symbols are named after the
// commands they send (names are ours); member names are ours.
class Connection : public ::nn::util::ADLFireWall::NonCopyable<nn::http::Connection>
{
public:
    Connection(); // 0x0046F63C | mk7dlp:bytes [tier A]
    // 0x0046F6B8 slot 0x00 (deleting dtor 0x0046F658)
    virtual ~Connection();

    nn::Result Initialize(const char* url, RequestMethod method, bool useDefaultProxy); // 0x0046F1A8 | mk7dlp:callseq [tier A]
    nn::Result Finalize(); // 0x0046F590 | fefates:bytes [tier B]
    nn::Result StartConnectionAsync(); // 0x0046F2D0 (name after the command)
    nn::Result SetInternalClientCert(u32 certId); // 0x0046F2F0 (name after the command)
    nn::Result AddHeaderField(const char* label, const char* value); // 0x0046F314 (name after the command)
    nn::Result SendPostDataRaw(const u8* data, size_t size); // 0x0046F384 (name after the command)
    nn::Result AddPostDataAscii(const char* label, const char* value); // 0x0046F3C4 (name after the command)
    nn::Result AddPostDataBinary(const char* label, const u8* data, size_t size); // 0x0046F434 (name after the command)
    nn::Result SetLazyPostDataSetting(PostDataType type, size_t size); // 0x0046F4A0 | fefates:bytes [tier B]
    nn::Result NotifyFinishSendPostData(); // 0x0046F4F4 (name after the command)
    nn::Result ReadBody(u8* pBuffer, size_t size); // 0x0046F514 (name after the command)
    nn::Result Cancel(); // 0x0046F554 | fefates:bytes [tier B]
    nn::Result SetRootCa(const u8* cert, size_t size); // 0x0046F5EC | mk7dlp:callgraph [tier A]
    nn::Result SetInternalRootCa(u32 certId); // 0x0046F618 (name after the command)
    nn::Result GetProgress(u32* pReceived, u32* pTotal) const; // 0x00737168 | fefates:bytes [tier B]
    nn::Result GetSslError(int* pError) const; // 0x007371C4 | fefates:bytes [tier B]
    nn::Result GetStatusCode(int* pStatusCode) const; // 0x00737218 | fefates:bytes [tier B]
    nn::Result GetHeaderField(const char* label, char* value, size_t valueSize, size_t* pValueSize) const; // 0x00737250 | fefates:bytes [tier B]

private:
    int m_Handle;               // 0x04, of the connection (> 0 while open)
    nn::Handle m_Session;       // 0x08
    mutable ConnectionIpc m_Ipc; // 0x0C, on m_Session
};
ASSERT_SIZE(Connection, 0x10);
} // namespace http
} // namespace nn
