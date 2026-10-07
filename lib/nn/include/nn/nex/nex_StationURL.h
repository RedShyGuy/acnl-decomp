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
    // (the values are not known)
    enum URLType
    {
    };
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
    u8 GetURLType() const; // 0x00729C20 | mk7dlp:bytes [tier A]
    u8 GetStreamID() const; // 0x00729C3C | mk7dlp:callseq-callee [tier A]
    void ValidateData() const; // 0x00729C58 | mk7dlp:bytes [tier A]
    void GetParamValue(const nn::nex::String&, unsigned) const; // 0x00729C88 | mk7dlp:callseq [tier A]
    void GetPortNumber() const; // 0x00729DF0 | mk7dlp:callseq-callee [tier A]
    const nn::nex::InetAddress* GetInetAddress() const; // 0x00729E30 | mk7dlp:bytes [tier A]
    u32 GetPrincipalID() const; // 0x00729E4C | mk7dlp:callseq-callee [tier A]
    u32 GetConnectionID() const; // 0x00729E5C | mk7dlp:callseq-callee [tier A]
    u8 GetProbeRequestInitiation() const; // 0x0072A040 | fefates:bytes [tier B]
    void GetURL() const; // 0x0072A060 | mk7dlp:bytes [tier A]
    void operator!=(const nn::nex::StationURL&) const; // 0x0072A08C | fefates:bytes [tier B]
    void SetConnectionID(unsigned int id); // 0x00355834 | fefates:callgraph [tier C]
    // (names are ours, after the getters; they call SetParamValue)
    void SetPrincipalID(unsigned int id); // 0x003557E4
    void SetRVConnectionID(unsigned int id); // 0x003558D4
    void SetStreamID(unsigned int id); // 0x003554E4
    void SetStreamType(unsigned int type); // 0x0035577C
    void SetType(unsigned int type); // 0x003563EC
    void SetProbeRequestInitiation(bool isInitiation); // 0x00355978
    // (names are ours; armlink placed them next to SetParamValue, which they call)
    void SetNATMapping(unsigned int mapping); // 0x0035550C
    void SetNATFiltering(unsigned int filtering); // 0x0035585C
    // (name is ours; it calls Copy)
    StationURL& operator=(const nn::nex::StationURL& rhs); // 0x00356864
    // (return types are ours)
    u8 GetNATMapping() const; // 0x00729C6C | fefates:callgraph [tier C]
    u8 GetStreamType() const; // 0x00729E14 | fefates:callgraph [tier C]
    u8 GetNATFiltering() const; // 0x00729E6C | fefates:callgraph [tier C]
    u32 GetRVConnectionID() const; // 0x00729F5C | fefates:callgraph [tier C]
    // the type flags (bit 0: behind a NAT, bit 1: public)
    u32 GetType() const; // 0x0072A07C | fefates:callgraph [tier C]
};
} // namespace nex
} // namespace nn
