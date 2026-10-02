#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Variant
{
public:
    Variant(); // TODO: default ctor added so derived stubs compile - may not exist
    Variant(const nn::nex::String&); // 0x003D3824 | fefates:bytes [tier B]
    Variant(const nn::nex::DateTime&); // 0x003D3844 | fefates:bytes [tier B]
    Variant(const nn::nex::Variant&); // 0x003D3860 | fefates:bytes [tier B]
    ~Variant(); // 0x003D38E0 | fefates:bytes [tier B]
    void operator=(const nn::nex::Variant&); // 0x003D3AE4 | fefates:bytes-fuzzy [tier B]
    void GetBoolValue() const; // 0x0072E508 | fefates:bytes [tier B]
    void GetDoubleValue() const; // 0x0072E540 | fefates:bytes [tier B]
    void GetStringValue() const; // 0x0072E560 | fefates:bytes [tier B]
    void GetDateTimeValue() const; // 0x0072E5CC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
