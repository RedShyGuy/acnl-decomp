#pragma once

#include "decomp.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qResult.h"

namespace __rw {
// (the original calls these instances out of line; defined in nex_StationURL.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::String>, __select1st<std::pair<const nn::nex::String, nn::nex::String>, nn::nex::String>,
                          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::String> > >::iterator
__rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::String>, __select1st<std::pair<const nn::nex::String, nn::nex::String>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::String> > >::erase(iterator first, iterator last);
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, u32>, __select1st<std::pair<const nn::nex::String, u32>, nn::nex::String>,
                          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, u32> > >::iterator
__rb_tree<nn::nex::String, std::pair<const nn::nex::String, u32>, __select1st<std::pair<const nn::nex::String, u32>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, u32> > >::erase(iterator first, iterator last);
} // namespace __rw

namespace std {
template <>
DECOMP_NOINLINE nn::nex::String& map<nn::nex::String, nn::nex::String, less<nn::nex::String>, nn::nex::MemAllocator<pair<const nn::nex::String, nn::nex::String> > >::operator[](const nn::nex::String& key);
template <>
inline u32& map<nn::nex::String, u32, less<nn::nex::String>, nn::nex::MemAllocator<pair<const nn::nex::String, u32> > >::operator[](const nn::nex::String& key)
{
    return IndexInline(key);
}
} // namespace std

namespace nn {
namespace nex {
// (out of line in the original; nex_StationURL.cpp)
template <>
DECOMP_NOINLINE qMap<String, String>::qMap();
template <>
DECOMP_NOINLINE qMap<String, u32>::qMap();

// The address of a station as text, e.g. "prudp:/address=1.2.3.4;port=1234;sid=15#Rsa=..." (the
// parameters behind '#' are the extended ones). The text and the parsed form are kept and made
// from each other on demand: numeric parameters are in m_Params, the others in m_StringParams
// (converted to numbers when they are asked for). The names of the members, of the URL types and
// of the functions marked so are ours.
// RTTI N2nn3nex10StationURLE @ 0x008CDFD0
// vtable 0x008FC1DC (vptr 0x008FC1E4), offset_to_top 0, 2 entries
class StationURL : public ::nn::nex::RootObject
{
public:
    enum URLType : u8
    {
        URL_TYPE_UNKNOWN = 0,
        URL_TYPE_PRUDP = 1,
        URL_TYPE_PRUDPS = 2,
        URL_TYPE_UDP = 3
    };
    // the size of the text that Format makes (characters)
    static const u32 URL_LENGTH = 1024;

    StationURL(); // 0x00356524 | mk7dlp:callseq-callee [tier A]
    StationURL(const wchar_t* pURL); // 0x00356414 | fefates:bytes [tier B]
    StationURL(const nn::nex::StationURL& other); // 0x0035649C | mk7dlp:callseq-callee [tier A]
    virtual ~StationURL(); // 0x003565E8 slot 0x00 | mk7dlp:callseq-callee [tier A]
    // 0x003565B8 slot 0x04 (deleting dtor)
    nn::nex::StationURL& operator=(const nn::nex::String& url); // 0x0035684C | mk7dlp:bytes [tier A]
    // (name is ours; it calls Copy)
    nn::nex::StationURL& operator=(const nn::nex::StationURL& other); // 0x00356864

    void SetURLType(nn::nex::StationURL::URLType type); // 0x00355224 | mk7dlp:bytes [tier A]
    // parses "key=value;key=value" (the text is changed while parsing and restored)
    bool ParseParams(wchar_t* pParams, bool isExtended); // 0x0035524C | fefates:bytes [tier B]
    nn::nex::qResult SetParamValue(const wchar_t* pKey, unsigned int value); // 0x00355534 | fefates:bytes-fuzzy [tier B]
    void SetPortNumber(unsigned short port); // 0x00355754 | fefates:bytes [tier B]
    bool SetInetAddress(const nn::nex::InetAddress* pAddress); // 0x003557A4 | mk7dlp:callseq-callee [tier A]
    // sets "Pl" to 1
    void SetPlatformType(); // 0x00355884 | fefates:bytes [tier B]
    void SetRelayServerAddress(const nn::nex::String& address); // 0x003558FC | fefates:bytes [tier B]
    void Copy(const nn::nex::StationURL& other); // 0x003559A0 | fefates:bytes [tier B]
    // makes the parsed form from the text
    void Parse(); // 0x00355C54 | mk7dlp:callgraph [tier A]
    // makes the text from the parsed form
    void Format(); // 0x00355E38 | mk7dlp:callgraph [tier A]
    DECOMP_NOINLINE void SetURL(const wchar_t* pURL); // 0x00356344 | mk7dlp:callgraph [tier A]
    void SetConnectionID(unsigned int id); // 0x00355834 | fefates:callgraph [tier C]
    // (names are ours, after the getters and the keys; they call SetParamValue)
    void SetStreamID(unsigned int id); // 0x003554E4
    void SetNATMapping(unsigned int mapping); // 0x0035550C
    void SetStreamType(unsigned int type); // 0x0035577C
    void SetPrincipalID(unsigned int id); // 0x003557E4
    void SetUPnPSupport(unsigned int isSupported); // 0x0035580C
    void SetNATFiltering(unsigned int filtering); // 0x0035585C
    void SetNatPMPSupport(unsigned int isSupported); // 0x003558AC
    void SetRVConnectionID(unsigned int id); // 0x003558D4
    void SetProbeRequestInitiation(bool isInitiation); // 0x00355978
    void SetType(unsigned int type); // 0x003563EC

    // (inline in the original, with copies out of line)
    nn::nex::String GetAddress() const; // 0x00729A88 | mk7dlp:callseq-callee [tier A]
    // the scheme of the text ("prudp" etc.)
    nn::nex::qResult GetURLType(wchar_t* pType, unsigned int size) const; // 0x00729B10 | fefates:callgraph [tier C]
    u8 GetURLType() const; // 0x00729C20 | mk7dlp:bytes [tier A]
    u8 GetStreamID() const; // 0x00729C3C | mk7dlp:callseq-callee [tier A]
    // parses the text if it was not parsed yet
    void ValidateData() const; // 0x00729C58 | mk7dlp:bytes [tier A]
    u8 GetNATMapping() const; // 0x00729C6C | fefates:callgraph [tier C]
    // the numeric parameter (defaultValue if there is none; the key is a wchar_t* in this build)
    u32 GetParamValue(const wchar_t* pKey, unsigned int defaultValue) const; // 0x00729C88 | mk7dlp:callseq [tier A]
    u16 GetPortNumber() const; // 0x00729DF0 | mk7dlp:callseq-callee [tier A]
    // (inline where the original inlines GetPortNumber)
    u16 GetPortNumberInline() const
    {
        if (!m_IsParsed) {
            const_cast<StationURL*>(this)->Parse();
        }
        return __builtin_bswap16(m_pAddress->m_SockAddr.m_Port);
    }
    // (inline where the original inlines GetInetAddress)
    const InetAddress* GetInetAddressInline() const
    {
        if (!m_IsParsed) {
            const_cast<StationURL*>(this)->Parse();
        }
        return m_pAddress;
    }
    // (inline where the original inlines SetURLType)
    void SetURLTypeInline(URLType type)
    {
        if (!m_IsParsed) {
            Parse();
        }
        m_URLType = type;
        m_IsFormatted = false;
    }
    // (inline where the original inlines SetPortNumber)
    void SetPortNumberInline(unsigned short port)
    {
        ValidateData();
        m_pAddress->m_SockAddr.m_Port = __builtin_bswap16(port);
        m_IsFormatted = false;
    }
    // (inline) the address of the URL (a dotted address or a host name)
    void SetAddressInline(const wchar_t* pAddress)
    {
        ValidateData();
        m_IsFormatted = false;
        m_pAddress->SetAddress(pAddress);
    }
    u8 GetStreamType() const; // 0x00729E14 | fefates:callgraph [tier C]
    const nn::nex::InetAddress* GetInetAddress() const; // 0x00729E30 | mk7dlp:bytes [tier A]
    u32 GetPrincipalID() const; // 0x00729E4C | mk7dlp:callseq-callee [tier A]
    u32 GetConnectionID() const; // 0x00729E5C | mk7dlp:callseq-callee [tier A]
    u8 GetNATFiltering() const; // 0x00729E6C | fefates:callgraph [tier C]
    nn::nex::String GetRelayAddress() const; // 0x00729E88 (name is ours)
    u32 GetRVConnectionID() const; // 0x00729F5C | fefates:callgraph [tier C]
    nn::nex::String GetRelayServerAddress() const; // 0x00729F6C (name is ours)
    bool GetProbeRequestInitiation() const; // 0x0072A040 | fefates:bytes [tier B]
    const wchar_t* GetURL() const; // 0x0072A060 | mk7dlp:bytes [tier A]
    // the type flags (bit 0: behind a NAT, bit 1: public)
    u32 GetType() const; // 0x0072A07C | fefates:callgraph [tier C]
    bool operator!=(const nn::nex::StationURL& other) const; // 0x0072A08C | fefates:bytes [tier B]

    InetAddress* m_pAddress;                // 0x04
    URLType m_URLType;                      // 0x08
    qMap<String, u32> m_Params;             // 0x0C
    qMap<String, String> m_StringParams;    // 0x28
    qMap<String, String> m_ExtendedParams;  // 0x44
    wchar_t* m_pURL;                        // 0x60 (NewArray)
    bool m_IsParsed;                        // 0x64
    bool m_IsFormatted;                     // 0x65 (m_pURL is up to date)
    bool m_IsValid;                         // 0x66
};
ASSERT_OFFSET(StationURL, m_ExtendedParams, 0x44);
ASSERT_OFFSET(StationURL, m_pURL, 0x60);
ASSERT_SIZE(StationURL, 0x68);

// (out of line in the original; nex_StationURL.cpp)
template <>
DECOMP_NOINLINE qList<StationURL>::qList();
template <>
DECOMP_NOINLINE qList<StationURL>::qList(const qList<StationURL>& other);
} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original; nex_StationURL.cpp)
template <>
u32 map<nn::nex::String, nn::nex::String>::erase(const nn::nex::String& key);
} // namespace std

namespace std {
template <>
DECOMP_NOINLINE list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::~list();

// 0x0082F0F0
template <>
DECOMP_NOINLINE list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >& list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::operator=(const list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >& other);

template <>
DECOMP_NOINLINE void list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::AddBuffer(bool isFirst);

template <>
DECOMP_NOINLINE void list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::push_back(const nn::nex::StationURL& value);
} // namespace std
