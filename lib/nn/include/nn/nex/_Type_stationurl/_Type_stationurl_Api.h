#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Message;
class StationURL;

// The DDL type of StationURL: its URL as a string.
class _Type_stationurl
{
public:
    typedef nn::nex::StationURL Type;

    static void Add(nn::nex::Message* pMessage, const nn::nex::StationURL& url); // 0x003847A0 | fefates:bytes [tier B]
    static bool Extract(nn::nex::Message* pMessage, nn::nex::StationURL* pURL); // 0x003847DC | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
