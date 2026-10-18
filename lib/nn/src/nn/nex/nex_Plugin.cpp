#include "nn/nex/nex_Plugin.h"

namespace nn {
namespace nex {
// 0x003D05E8 slot 0x28 (name is ours)
bool nn::nex::Plugin::IsCompatible()
{
    return true;
}

// 0x003D05F0 slot 0x2C (name is ours)
void nn::nex::Plugin::Hook0x2C()
{
    // nothing to do
}

// 0x003D05F4 slot 0x30 (name is ours)
void nn::nex::Plugin::Hook0x30()
{
    // nothing to do
}

// 0x003D05F8 slot 0x34 (name is ours)
void nn::nex::Plugin::Hook0x34()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
