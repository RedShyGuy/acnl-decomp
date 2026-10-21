#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemSetting.h"

namespace nn {
namespace nex {
class Message;
class String;

// The DDL type of strings: a u16 size and the characters (UTF-16, or UTF-8 by the setting
// ConnectionEncoding). The names of the statics are ours.
class _Type_string
{
public:
    typedef nn::nex::String Type;

    // the encodings of the setting (names are ours)
    static const u32 ENCODING_UTF16 = 0;
    static const u32 ENCODING_UTF8 = 1;

    static void Add(nn::nex::Message* pMessage, const nn::nex::String& string); // 0x003618AC | mk7dlp:bytes [tier A]
    // false if the message is too short for the string
    static bool Extract(nn::nex::Message* pMessage, nn::nex::String* pString); // 0x003619AC | mk7dlp:callgraph [tier A]

    static SystemSetting s_ConnectionEncoding;
    // (1: UTF-16 strings are turned into UTF-8 ones)
    static u32 s_ExtractMode;
};
} // namespace nex
} // namespace nn
