#pragma once

#include "decomp.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace http {
// RTTI N2nn4http10ConnectionE @ 0x008D03A4
// vtable 0x009021EC (vptr 0x009021F4), offset_to_top 0, 2 entries
class Connection : public ::nn::util::ADLFireWall::NonCopyable<nn::http::Connection>
{
public:
    virtual ~Connection(); // 0x0046F6B8 slot 0x00 | fefates:bytes-fuzzy
    virtual void vf_0x04(); // 0x0046F658 slot 0x04 | virtual slot, introduced by nn::http::Connection
    void Initialize(const char*, nn::http::RequestMethod, bool); // 0x0046F1A8 | mk7dlp:callseq [tier A]
    void SetLazyPostDataSetting(nn::http::PostDataType, unsigned int); // 0x0046F4A0 | fefates:bytes [tier B]
    void Cancel(); // 0x0046F554 | fefates:bytes [tier B]
    void Finalize(); // 0x0046F590 | fefates:bytes [tier B]
    void SetRootCa(const unsigned char*, unsigned); // 0x0046F5EC | mk7dlp:callgraph [tier A]
    Connection(); // 0x0046F63C | mk7dlp:bytes [tier A]
    void GetProgress(unsigned int*, unsigned int*) const; // 0x00737168 | fefates:bytes [tier B]
    void GetSslError(int*) const; // 0x007371C4 | fefates:bytes [tier B]
    void GetStatusCode(int*) const; // 0x00737218 | fefates:bytes [tier B]
    void GetHeaderField(const char*, char*, unsigned int, unsigned int*) const; // 0x00737250 | fefates:bytes [tier B]
};
} // namespace http
} // namespace nn
