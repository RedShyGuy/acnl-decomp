#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11NonCopyableE @ 0x008CE048
// The base of the classes that cannot be copied (the copy constructor and the assignment are private
// and not defined).
class NonCopyable
{
public:
    // (inline)
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable& other);
    NonCopyable& operator=(const NonCopyable& other);
};
} // namespace nex
} // namespace nn
