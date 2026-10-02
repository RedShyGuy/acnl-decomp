#include "nn/nex/nex_Variant.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::Variant::Variant()
{
}

// 0x003D3824 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::String&)
{
}

// 0x003D3844 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::DateTime&)
{
}

// 0x003D3860 | fefates:bytes [tier B]
nn::nex::Variant::Variant(const nn::nex::Variant&)
{
}

// 0x003D38E0 | fefates:bytes [tier B]
nn::nex::Variant::~Variant()
{
}

// 0x003D3AE4 | fefates:bytes-fuzzy [tier B]
void nn::nex::Variant::operator=(const nn::nex::Variant&)
{
}

// 0x0072E508 | fefates:bytes [tier B]
void nn::nex::Variant::GetBoolValue() const
{
}

// 0x0072E540 | fefates:bytes [tier B]
void nn::nex::Variant::GetDoubleValue() const
{
}

// 0x0072E560 | fefates:bytes [tier B]
void nn::nex::Variant::GetStringValue() const
{
}

// 0x0072E5CC | fefates:bytes [tier B]
void nn::nex::Variant::GetDateTimeValue() const
{
}

} // namespace nex
} // namespace nn
