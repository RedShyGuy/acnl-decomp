#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace fnd {
class IAllocator;
} // namespace fnd

namespace cec {
namespace CTR {
// Starts and stops StreetPass: opens the session of cecd and suspends / resumes the cec daemon of
// ndm while a message box is open. All functions are static.
class CecControl
{
public:
    static nn::Result Initialize(nn::fnd::IAllocator& allocator); // 0x0034D348 | nintendogs:callseq [tier C]
    static nn::Result Initialize(); // 0x0034D358 | fefates:bytes [tier B]
    static nn::Result StartScanning(bool start); // 0x001407C0 | nintendogs:bytes [tier A]
    static nn::Result StopScanning(bool stop, bool noWait); // 0x0034D3B8 | nintendogs:callgraph [tier A]
    static nn::Result Suspend(); // 0x0034D574 | nintendogs:bytes [tier A]
    static nn::Result Finalize(); // 0x0034D604 | nintendogs:callseq [tier A]
};
} // namespace CTR
} // namespace cec
} // namespace nn
