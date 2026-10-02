#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10StationURLE @ 0x008CDFD0
// vtable 0x008FC1DC (vptr 0x008FC1E4), offset_to_top 0, 2 entries
class StationURL : public ::nn::nex::RootObject
{
public:
    struct URLType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~StationURL(); // 0x003565E8 slot 0x00 | mk7dlp:callseq-callee
    // 0x003565B8 slot 0x04 | slot vf_0x04 of nn::nex::StationURL (deleting dtor)
    void SetURLType(nn::nex::StationURL::URLType); // 0x00355224 | mk7dlp:bytes [tier A]
    void ParseParams(wchar_t*, bool); // 0x0035524C | fefates:bytes [tier B]
    void SetParamValue(const wchar_t*, unsigned int); // 0x00355534 | fefates:bytes-fuzzy [tier B]
    void SetPortNumber(unsigned short); // 0x00355754 | fefates:bytes [tier B]
    void SetInetAddress(const nn::nex::InetAddress*); // 0x003557A4 | mk7dlp:callseq-callee [tier A]
    void SetPlatformType(); // 0x00355884 | fefates:bytes [tier B]
    void SetRelayServerAddress(const nn::nex::String&); // 0x003558FC | fefates:bytes [tier B]
    void Copy(const nn::nex::StationURL&); // 0x003559A0 | fefates:bytes [tier B]
    void Parse(); // 0x00355C54 | mk7dlp:callgraph [tier A]
    void Format(); // 0x00355E38 | mk7dlp:callgraph [tier A]
    void SetURL(const wchar_t*); // 0x00356344 | mk7dlp:callgraph [tier A]
    StationURL(const wchar_t*); // 0x00356414 | fefates:bytes [tier B]
    StationURL(const nn::nex::StationURL&); // 0x0035649C | mk7dlp:callseq-callee [tier A]
    StationURL(); // 0x00356524 | mk7dlp:callseq-callee [tier A]
    void operator =(const nn::nex::String&); // 0x0035684C | mk7dlp:bytes [tier A]
    void GetAddress() const; // 0x00729A88 | mk7dlp:callseq-callee [tier A]
    void GetURLType() const; // 0x00729C20 | mk7dlp:bytes [tier A]
    void GetStreamID() const; // 0x00729C3C | mk7dlp:callseq-callee [tier A]
    void ValidateData() const; // 0x00729C58 | mk7dlp:bytes [tier A]
    void GetParamValue(const nn::nex::String&, unsigned) const; // 0x00729C88 | mk7dlp:callseq [tier A]
    void GetPortNumber() const; // 0x00729DF0 | mk7dlp:callseq-callee [tier A]
    void GetInetAddress() const; // 0x00729E30 | mk7dlp:bytes [tier A]
    void GetPrincipalID() const; // 0x00729E4C | mk7dlp:callseq-callee [tier A]
    void GetConnectionID() const; // 0x00729E5C | mk7dlp:callseq-callee [tier A]
    void GetProbeRequestInitiation() const; // 0x0072A040 | fefates:bytes [tier B]
    void GetURL() const; // 0x0072A060 | mk7dlp:bytes [tier A]
    void operator!=(const nn::nex::StationURL&) const; // 0x0072A08C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
