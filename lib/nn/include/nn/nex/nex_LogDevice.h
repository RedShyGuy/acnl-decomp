#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// Where a Log writes its lines (only the derived classes have a vtable). The name of the function
// is ours.
// RTTI N2nn3nex9LogDeviceE @ 0x008CF75C
class LogDevice : public ::nn::nex::RootObject
{
public:
    LogDevice() {} // (inline)
    virtual ~LogDevice() {}
    virtual void Output(const wchar_t* pLine) = 0;
};
} // namespace nex
} // namespace nn
