#pragma once

// nn::Handle - a kernel object handle (thread, event, session, ...).
// The class name is from the binary (mangled signatures like nn::svc::CloseHandle(nn::Handle)).
// It is passed in a register, so it is a 4 byte class. The member name is ours.
//
// Only "types.h" may be included here: include/forward.h includes this header.

#include "types.h"

namespace nn {

class Handle
{
public:
    Handle() : mHandle(0) {}
    explicit Handle(bit32 handle) : mHandle(handle) {}

    bool IsValid() const { return mHandle != 0; }
    bit32 GetPrintableBits() const { return mHandle; }

    bool operator==(const Handle& rhs) const { return mHandle == rhs.mHandle; }
    bool operator!=(const Handle& rhs) const { return mHandle != rhs.mHandle; }

private:
    bit32 mHandle;
};

// Pseudo handles understood by the kernel (3dbrew: "Handles")
static const bit32 PSEUDO_HANDLE_CURRENT_THREAD = 0xFFFF8000;
static const bit32 PSEUDO_HANDLE_CURRENT_PROCESS = 0xFFFF8001;

} // namespace nn
