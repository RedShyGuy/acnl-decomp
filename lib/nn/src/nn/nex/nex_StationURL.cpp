#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_StringConversion.h"
#include "nn/socket/socket_Api.h"
#include "nn/socket/socket_Types.h"
#include <cstring>
#include <cwchar>

namespace nn {
namespace nex {
namespace {
const int RESULT_SUCCESS = qResult::SUCCESS;
// the result codes of GetURLType (Core module)
const int RESULT_NO_URL = static_cast<int>(0x8001000C);
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_BUFFER_TOO_SMALL = static_cast<int>(0x8001000F);
// (Transport module) the scheme of the text is invalid
const int RESULT_INVALID_URL = static_cast<int>(0x80050003);
// the sizes of the text forms of the address
const u32 ADDRESS_LENGTH = 20;
const u32 ADDRESS_WIDE_LENGTH = 128;
// the room that Format needs for an address
const u32 ADDRESS_ROOM = 32;
const u32 URL_TYPE_LENGTH = 64;
} // namespace

// 0x00355224 | mk7dlp:bytes [tier A]
void nn::nex::StationURL::SetURLType(nn::nex::StationURL::URLType type)
{
    ValidateData();
    m_URLType = type;
    m_IsFormatted = false;
}

// 0x0035524C | fefates:bytes [tier B]
bool nn::nex::StationURL::ParseParams(wchar_t* pParams, bool isExtended)
{
    bool isLast = false;
    while (true) {
        wchar_t* pEnd = wcsstr(pParams, L";");
        if (pEnd == nullptr) {
            isLast = true;
            pEnd = pParams + wcslen(pParams);
            if (pParams == pEnd) {
                return true;
            }
        }
        if (pParams == pEnd) {
            return false;
        }
        wchar_t* pKey = pParams;
        *pEnd = L'\0';
        bool isValid;
        wchar_t* pEqual = wcsstr(pParams, L"=");
        if (pEqual == nullptr || pKey == pEqual || pEqual[1] == L'\0' || wcsstr(pEqual + 1, L"=") != nullptr) {
            isValid = false;
        } else {
            isValid = true;
            *pEqual = L'\0';
            wchar_t* pValue = pEqual + 1;
            if (isExtended) {
                String key(pKey);
                String value(pValue);
                ValidateData();
                m_ExtendedParams[key] = value;
                m_IsFormatted = false;
            } else if (wcscmp(pKey, L"address") == 0) {
                ValidateData();
                m_IsFormatted = false;
                isValid = m_pAddress->SetAddress(pValue);
            } else if (wcscmp(pKey, L"port") == 0) {
                u16 port = wcstoul(pValue, nullptr, 10);
                ValidateData();
                m_pAddress->m_SockAddr.m_Port = __builtin_bswap16(port);
                m_IsFormatted = false;
            } else {
                String key(pKey);
                String value(pValue);
                ValidateData();
                m_StringParams[key] = value;
                m_IsFormatted = false;
            }
            *pEqual = L'=';
        }
        if (!isLast) {
            *pEnd = L';';
        }
        if (!isValid) {
            return false;
        }
        if (isLast) {
            return true;
        }
        pParams = pEnd + 1;
    }
}

// 0x003554E4 (name is ours)
void nn::nex::StationURL::SetStreamID(unsigned int id)
{
    SetParamValue(L"sid", id);
}

// 0x0035550C (name is ours)
void nn::nex::StationURL::SetNATMapping(unsigned int mapping)
{
    SetParamValue(L"natm", mapping);
}

// 0x00355534 | fefates:bytes-fuzzy [tier B]
nn::nex::qResult nn::nex::StationURL::SetParamValue(const wchar_t* pKey, unsigned int value)
{
    ValidateData();
    m_StringParams.erase(String(pKey));
    m_Params[String(pKey)] = value;
    m_IsFormatted = false;
    return qResult(RESULT_SUCCESS);
}

// 0x00355754 | fefates:bytes [tier B]
void nn::nex::StationURL::SetPortNumber(unsigned short port)
{
    ValidateData();
    m_pAddress->m_SockAddr.m_Port = __builtin_bswap16(port);
    m_IsFormatted = false;
}

// 0x0035577C (name is ours)
void nn::nex::StationURL::SetStreamType(unsigned int type)
{
    SetParamValue(L"stream", type);
}

// 0x003557A4 | mk7dlp:callseq-callee [tier A]
bool nn::nex::StationURL::SetInetAddress(const nn::nex::InetAddress* pAddress)
{
    ValidateData();
    m_pAddress->m_SockAddr = pAddress->m_SockAddr;
    m_IsFormatted = false;
    return true;
}

// 0x003557E4 (name is ours)
void nn::nex::StationURL::SetPrincipalID(unsigned int id)
{
    SetParamValue(L"PID", id);
}

// 0x0035580C (name is ours)
void nn::nex::StationURL::SetUPnPSupport(unsigned int isSupported)
{
    SetParamValue(L"upnp", isSupported);
}

// 0x00355834 | fefates:callgraph [tier C]
void nn::nex::StationURL::SetConnectionID(unsigned int id)
{
    SetParamValue(L"CID", id);
}

// 0x0035585C (name is ours)
void nn::nex::StationURL::SetNATFiltering(unsigned int filtering)
{
    SetParamValue(L"natf", filtering);
}

// 0x00355884 | fefates:bytes [tier B]
void nn::nex::StationURL::SetPlatformType()
{
    SetParamValue(L"Pl", 1);
}

// 0x003558AC (name is ours)
void nn::nex::StationURL::SetNatPMPSupport(unsigned int isSupported)
{
    SetParamValue(L"pmp", isSupported);
}

// 0x003558D4 (name is ours)
void nn::nex::StationURL::SetRVConnectionID(unsigned int id)
{
    SetParamValue(L"RVCID", id);
}

// 0x003558FC | fefates:bytes [tier B]
void nn::nex::StationURL::SetRelayServerAddress(const nn::nex::String& address)
{
    m_StringParams.erase(String(L"Rsa"));
    m_StringParams[String(L"Rsa")] = address;
}

// 0x00355978 (name is ours)
void nn::nex::StationURL::SetProbeRequestInitiation(bool isInitiation)
{
    SetParamValue(L"probeinit", isInitiation);
}

// 0x003559A0 | fefates:bytes [tier B]
void nn::nex::StationURL::Copy(const nn::nex::StationURL& other)
{
    if (other.m_IsFormatted) {
        SetURL(other.m_pURL);
        m_IsFormatted = true;
    } else {
        SetURL(nullptr);
    }
    if (other.m_IsParsed) {
        m_pAddress->m_SockAddr = other.m_pAddress->m_SockAddr;
        m_URLType = other.m_URLType;
        m_Params = other.m_Params;
        m_StringParams = other.m_StringParams;
        m_ExtendedParams = other.m_ExtendedParams;
        m_IsParsed = true;
    }
    m_IsValid = other.m_IsValid;
}

// 0x00355C54 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::Parse()
{
    m_IsParsed = true;
    *reinterpret_cast<u32*>(&m_pAddress->m_SockAddr) = 0;
    m_pAddress->m_SockAddr.m_Address = 0;
    m_pAddress->m_SockAddr.m_Family = nn::socket::AF_INET;
    m_Params.clear();
    m_StringParams.clear();
    m_ExtendedParams.clear();
    wchar_t type[URL_TYPE_LENGTH];
    qResult result = GetURLType(type, URL_TYPE_LENGTH);
    URLType urlType = URL_TYPE_UNKNOWN;
    if (result.m_Code >= 0) {
        if (wcscmp(type, L"prudp") == 0) {
            urlType = URL_TYPE_PRUDP;
        } else if (wcscmp(type, L"prudps") == 0) {
            urlType = URL_TYPE_PRUDPS;
        } else if (wcscmp(type, L"udp") == 0) {
            urlType = URL_TYPE_UDP;
        }
    }
    m_URLType = urlType;
    if (urlType == URL_TYPE_UNKNOWN) {
        return;
    }
    wchar_t* pParams = wcsstr(m_pURL, L":/") + wcslen(L":/");
    wchar_t* pHash = wcsstr(pParams, L"#");
    if (pHash != nullptr) {
        if (wcsstr(pHash + 1, L"#") != nullptr) {
            return;
        }
        *pHash = L'\0';
        bool isValid = ParseParams(pParams, false);
        *pHash = L'#';
        if (!isValid || !ParseParams(pHash + 1, true)) {
            return;
        }
    } else if (!ParseParams(pParams, false)) {
        return;
    }
    m_IsValid = true;
    m_IsFormatted = true;
}

// 0x00355E38 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::Format()
{
    wchar_t buffer[URL_LENGTH];
    int length = 0;
    switch (m_URLType) {
    case URL_TYPE_PRUDP:
        length = swprintf(buffer, URL_LENGTH, L"prudp:/");
        break;
    case URL_TYPE_PRUDPS:
        length = swprintf(buffer, URL_LENGTH, L"prudps:/");
        break;
    case URL_TYPE_UDP:
        length = swprintf(buffer, URL_LENGTH, L"udp:/");
        break;
    default:
        break;
    }
    bool hasParams = false;
    if (m_pAddress->GetAddress() != 0) {
        int end = length + swprintf(buffer + length, URL_LENGTH, L"%ls%ls", L"address", L"=");
        u32 room = URL_LENGTH - end;
        wchar_t* pAddress = buffer + end;
        if (room >= ADDRESS_ROOM) {
            char text[ADDRESS_LENGTH];
            nn::socket::InetNtoP(nn::socket::AF_INET, &m_pAddress->m_SockAddr.m_Address, text, ADDRESS_LENGTH);
            StringConversion::Char8_2T(text, pAddress, room);
        }
        length = wcslen(pAddress) + end;
        hasParams = true;
    }
    if (m_pAddress->GetPortNumber() != 0) {
        if (hasParams) {
            length += swprintf(buffer + length, URL_LENGTH, L";");
        }
        length += swprintf(buffer + length, URL_LENGTH, L"%ls%ls%u", L"port", L"=", m_pAddress->GetPortNumber());
        hasParams = true;
    }
    // the string and the numeric parameters in the order of their keys
    qMap<String, String>::iterator itString = m_StringParams.begin();
    qMap<String, u32>::iterator itNumber = m_Params.begin();
    while (itString != m_StringParams.end() || itNumber != m_Params.end()) {
        if (hasParams) {
            length += swprintf(buffer + length, URL_LENGTH, L";");
        }
        if (itString != m_StringParams.end() && (itNumber == m_Params.end() || itString->first < itNumber->first)) {
            const wchar_t* pKey = itString->first.m_pString;
            const wchar_t* pValue = itString->second.m_pString;
            ++itString;
            if (pValue != nullptr) {
                length += swprintf(buffer + length, URL_LENGTH, L"%ls%ls%ls", pKey, L"=", pValue);
            } else {
                length += swprintf(buffer + length, URL_LENGTH, L"%ls%ls%u", pKey, L"=", 0u);
            }
        } else {
            const wchar_t* pKey = itNumber->first.m_pString;
            u32 value = itNumber->second;
            ++itNumber;
            length += swprintf(buffer + length, URL_LENGTH, L"%ls%ls%u", pKey, L"=", value);
        }
        hasParams = true;
    }
    if (m_ExtendedParams.size() != 0) {
        length += swprintf(buffer + length, URL_LENGTH, L"#");
        qMap<String, String>::iterator it = m_ExtendedParams.begin();
        while (it != m_ExtendedParams.end()) {
            length += swprintf(buffer + length, URL_LENGTH, L"%ls%ls%ls", it->first.m_pString, L"=", it->second.m_pString);
            ++it;
            if (it != m_ExtendedParams.end()) {
                length += swprintf(buffer + length, URL_LENGTH, L";");
            }
        }
    }
    SetURL(buffer);
    m_IsValid = true;
    m_IsParsed = true;
}

// 0x00356344 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::SetURL(const wchar_t* pURL)
{
    if (m_pURL != nullptr) {
        DeleteArray(m_pURL);
        m_pURL = nullptr;
        m_IsFormatted = false;
    }
    if (pURL != nullptr) {
        u32 length = wcslen(pURL) + 1;
        m_pURL = NewArray<wchar_t>(length);
        wcsncpy(m_pURL, pURL, length);
        m_pURL[length - 1] = L'\0';
        m_IsFormatted = true;
        m_IsParsed = false;
        m_IsValid = false;
    }
}

// 0x003563EC (name is ours)
void nn::nex::StationURL::SetType(unsigned int type)
{
    SetParamValue(L"type", type);
}

// 0x00356414 | fefates:bytes [tier B]
nn::nex::StationURL::StationURL(const wchar_t* pURL) : m_pURL(nullptr), m_IsParsed(false), m_IsFormatted(true), m_IsValid(false)
{
    m_pAddress = new InetAddress();
    SetURL(pURL);
}

// 0x0035649C | mk7dlp:callseq-callee [tier A]
nn::nex::StationURL::StationURL(const nn::nex::StationURL& other) : m_pURL(nullptr), m_IsParsed(true), m_IsFormatted(false), m_IsValid(false)
{
    m_pAddress = new InetAddress();
    Copy(other);
}

// 0x00356524 | mk7dlp:callseq-callee [tier A]
nn::nex::StationURL::StationURL() : m_pURL(nullptr), m_IsParsed(true), m_IsFormatted(false), m_IsValid(true)
{
    m_pAddress = new InetAddress();
    SetURLType(URL_TYPE_PRUDP);
}

// 0x003565E8 | mk7dlp:callseq-callee [tier A]
// 0x003565B8 (deleting dtor)
nn::nex::StationURL::~StationURL()
{
    SetURL(nullptr);
    delete m_pAddress;
}

// 0x0035684C | mk7dlp:bytes [tier A]
nn::nex::StationURL& nn::nex::StationURL::operator=(const nn::nex::String& url)
{
    SetURL(url.m_pString);
    return *this;
}

// 0x00356864 (name is ours)
nn::nex::StationURL& nn::nex::StationURL::operator=(const nn::nex::StationURL& other)
{
    Copy(other);
    return *this;
}

// 0x00729A88 | mk7dlp:callseq-callee [tier A]
nn::nex::String nn::nex::StationURL::GetAddress() const
{
    wchar_t address[ADDRESS_WIDE_LENGTH];
    address[0] = L'\0';
    ValidateData();
    if (m_pAddress->GetAddress() != 0) {
        char text[ADDRESS_LENGTH];
        nn::socket::InetNtoP(nn::socket::AF_INET, &m_pAddress->m_SockAddr.m_Address, text, ADDRESS_LENGTH);
        StringConversion::Char8_2T(text, address, ADDRESS_WIDE_LENGTH);
    }
    return String(address);
}

// 0x00729B10 | fefates:callgraph [tier C]
nn::nex::qResult nn::nex::StationURL::GetURLType(wchar_t* pType, unsigned int size) const
{
    if (m_pURL == nullptr) {
        return qResult(RESULT_NO_URL);
    }
    if (pType == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    const wchar_t* pSeparator = wcsstr(m_pURL, L":/");
    if (pSeparator == nullptr || pSeparator == m_pURL) {
        return qResult(RESULT_INVALID_URL);
    }
    u32 length = pSeparator - m_pURL;
    for (u32 i = 0; i < length; i++) {
        wchar_t c = m_pURL[i];
        if (!((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || (c >= L'0' && c <= L'9') || c == L'-' || c == L'_')) {
            return qResult(RESULT_INVALID_URL);
        }
    }
    if (length + 1 > size) {
        return qResult(RESULT_BUFFER_TOO_SMALL);
    }
    memcpy(pType, m_pURL, length * sizeof(wchar_t));
    pType[length] = L'\0';
    return qResult(RESULT_SUCCESS);
}

// 0x00729C20 | mk7dlp:bytes [tier A]
u8 nn::nex::StationURL::GetURLType() const
{
    ValidateData();
    return m_URLType;
}

// 0x00729C3C | mk7dlp:callseq-callee [tier A]
u8 nn::nex::StationURL::GetStreamID() const
{
    return GetParamValue(L"sid", 0);
}

// 0x00729C58 | mk7dlp:bytes [tier A]
void nn::nex::StationURL::ValidateData() const
{
    if (!m_IsParsed) {
        const_cast<StationURL*>(this)->Parse();
    }
}

// 0x00729C6C | fefates:callgraph [tier C]
u8 nn::nex::StationURL::GetNATMapping() const
{
    return GetParamValue(L"natm", 0);
}

// 0x00729C88 | mk7dlp:callseq [tier A]
u32 nn::nex::StationURL::GetParamValue(const wchar_t* pKey, unsigned int defaultValue) const
{
    ValidateData();
    qMap<String, u32>::iterator it = m_Params.find(String(pKey));
    if (it != m_Params.end()) {
        return it->second;
    }
    qMap<String, String>::iterator itString = m_StringParams.find(String(pKey));
    if (itString == m_StringParams.end()) {
        return defaultValue;
    }
    u32 value = wcstoul(itString->second.m_pString, nullptr, 10);
    // (the converted value is kept as a numeric parameter)
    const_cast<StationURL*>(this)->SetParamValue(pKey, value);
    return value;
}

// 0x00729DF0 | mk7dlp:callseq-callee [tier A]
u16 nn::nex::StationURL::GetPortNumber() const
{
    ValidateData();
    return __builtin_bswap16(m_pAddress->m_SockAddr.m_Port);
}

// 0x00729E14 | fefates:callgraph [tier C]
u8 nn::nex::StationURL::GetStreamType() const
{
    return GetParamValue(L"stream", 0);
}

// 0x00729E30 | mk7dlp:bytes [tier A]
const nn::nex::InetAddress* nn::nex::StationURL::GetInetAddress() const
{
    ValidateData();
    return m_pAddress;
}

// 0x00729E4C | mk7dlp:callseq-callee [tier A]
u32 nn::nex::StationURL::GetPrincipalID() const
{
    return GetParamValue(L"PID", 0);
}

// 0x00729E5C | mk7dlp:callseq-callee [tier A]
u32 nn::nex::StationURL::GetConnectionID() const
{
    return GetParamValue(L"CID", 0);
}

// 0x00729E6C | fefates:callgraph [tier C]
u8 nn::nex::StationURL::GetNATFiltering() const
{
    return GetParamValue(L"natf", 0);
}

// 0x00729E88 (name is ours)
nn::nex::String nn::nex::StationURL::GetRelayAddress() const
{
    qMap<String, String>::iterator it = m_StringParams.find(String(L"Ra"));
    if (it != m_StringParams.end()) {
        return String(it->second.m_pString);
    }
    return String(L"");
}

// 0x00729F5C | fefates:callgraph [tier C]
u32 nn::nex::StationURL::GetRVConnectionID() const
{
    return GetParamValue(L"RVCID", 0);
}

// 0x00729F6C (name is ours)
nn::nex::String nn::nex::StationURL::GetRelayServerAddress() const
{
    qMap<String, String>::iterator it = m_StringParams.find(String(L"Rsa"));
    if (it != m_StringParams.end()) {
        return String(it->second.m_pString);
    }
    return String(L"");
}

// 0x0072A040 | fefates:bytes [tier B]
bool nn::nex::StationURL::GetProbeRequestInitiation() const
{
    return GetParamValue(L"probeinit", 0) != 0;
}

// 0x0072A060 | mk7dlp:bytes [tier A]
const wchar_t* nn::nex::StationURL::GetURL() const
{
    if (!m_IsFormatted) {
        const_cast<StationURL*>(this)->Format();
    }
    return m_pURL;
}

// 0x0072A07C | fefates:callgraph [tier C]
u32 nn::nex::StationURL::GetType() const
{
    return GetParamValue(L"type", 0);
}

// 0x0072A08C | fefates:bytes [tier B]
bool nn::nex::StationURL::operator!=(const nn::nex::StationURL& other) const
{
    const wchar_t* pOther = other.GetURL();
    return wcscmp(GetURL(), pOther) != 0;
}

// 0x003CE134 (name is ours)
template <>
DECOMP_NOINLINE qMap<String, String>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

// 0x003CE1D0 (name is ours)
template <>
DECOMP_NOINLINE qMap<String, u32>::qMap()
{
    // nothing to do: the tree is constructed by the compiler
}

// 0x003CF340 | mk7dlp:bytes [tier A]
template <>
DECOMP_NOINLINE qList<StationURL>::qList()
{
    // nothing to do: std::list makes the head node
}

// 0x003CF25C
template <>
DECOMP_NOINLINE qList<StationURL>::qList(const qList<StationURL>& other) : base_type(base_type::NoInit())
{
    InitializeInline();
    for (Node* pNode = other.GetFirst(); !other.IsEnd(pNode); pNode = pNode->m_pNext) {
        insert(m_pHead, pNode->m_Value);
    }
}

// 0x003CF394 (name is ours; it calls the destructor of std::list)
template qList<StationURL>::~qList();

} // namespace nex
} // namespace nn

namespace __rw {
// 0x007FE8D8 | mk7dlp:callgraph [tier A]
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::String>, __select1st<std::pair<const nn::nex::String, nn::nex::String>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::String> > >::iterator
__rb_tree<nn::nex::String, std::pair<const nn::nex::String, nn::nex::String>, __select1st<std::pair<const nn::nex::String, nn::nex::String>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, nn::nex::String> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}

// (armlink folded this instance with the equal code of the DOHandle map that symbols.json names)
// 0x007FF59C
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::String, std::pair<const nn::nex::String, u32>, __select1st<std::pair<const nn::nex::String, u32>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, u32> > >::iterator
__rb_tree<nn::nex::String, std::pair<const nn::nex::String, u32>, __select1st<std::pair<const nn::nex::String, u32>, nn::nex::String>,
          std::less<nn::nex::String>, nn::nex::MemAllocator<std::pair<const nn::nex::String, u32> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
} // namespace __rw

namespace std {
// 0x0082E600 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE nn::nex::String& map<nn::nex::String, nn::nex::String, less<nn::nex::String>, nn::nex::MemAllocator<pair<const nn::nex::String, nn::nex::String> > >::operator[](const nn::nex::String& key)
{
    return IndexInline(key);
}
} // namespace std

namespace std {
// 0x0082E4D4 (name is ours)
template <>
u32 map<nn::nex::String, nn::nex::String>::erase(const nn::nex::String& key)
{
    return EraseInline(key);
}
} // namespace std

namespace std {
// 0x0082F020 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE DECOMP_NOIPA list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::~list()
{
    DestroyInline();
}

// 0x0082EEB8 | mk7dlp:callgraph [tier A]
template <>
DECOMP_NOINLINE void list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::AddBuffer(bool isFirst)
{
    AddBufferInline(isFirst);
}

// 0x0082EFA4 | fefates:callgraph [tier B]
template <>
DECOMP_NOINLINE void list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::push_back(const nn::nex::StationURL& value)
{
    PushBackInline(value);
}

// 0x0082F0F0
template <>
DECOMP_NOINLINE list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >& list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >::operator=(const list<nn::nex::StationURL, nn::nex::MemAllocator<nn::nex::StationURL> >& other)
{
    if (this != &other) {
        Node* pNode = m_pHead->m_pNext;
        Node* pOther = other.m_pHead->m_pNext;
        for (; pNode != m_pHead && pOther != other.m_pHead; pNode = pNode->m_pNext, pOther = pOther->m_pNext) {
            pNode->m_Value.Copy(pOther->m_Value);
        }
        if (pOther == other.m_pHead) {
            while (pNode != m_pHead) {
                pNode = erase(pNode);
            }
        } else {
            for (; pOther != other.m_pHead; pOther = pOther->m_pNext) {
                push_back(pOther->m_Value);
            }
        }
    }
    return *this;
}
} // namespace std
