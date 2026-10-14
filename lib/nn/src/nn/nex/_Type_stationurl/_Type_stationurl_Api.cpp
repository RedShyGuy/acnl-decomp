#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
// 0x003847A0 | fefates:bytes [tier B]
void nn::nex::_Type_stationurl::Add(nn::nex::Message* pMessage, const nn::nex::StationURL& url)
{
    String string(url.GetURL());
    _Type_string::Add(pMessage, string);
}

// 0x003847DC | mk7dlp:bytes [tier A]
bool nn::nex::_Type_stationurl::Extract(nn::nex::Message* pMessage, nn::nex::StationURL* pURL)
{
    String string;
    if (!_Type_string::Extract(pMessage, &string)) {
        return false;
    }
    pURL->SetURL(string.m_pString);
    return true;
}

} // namespace nex
} // namespace nn
