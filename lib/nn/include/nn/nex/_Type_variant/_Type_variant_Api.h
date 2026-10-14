#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Message;
class Variant;

// The DDL type of Variant: the type byte and the value of the type.
class _Type_variant
{
public:
    typedef Variant Type;

    static void Add(nn::nex::Message* pMessage, const nn::nex::Variant& variant); // 0x0036FC84 | fefates:bytes-fuzzy [tier B]
    // false if the string of a string variant cannot be read (a short value is read as 0)
    static bool Extract(nn::nex::Message* pMessage, nn::nex::Variant* pVariant); // 0x0036FDD8 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
