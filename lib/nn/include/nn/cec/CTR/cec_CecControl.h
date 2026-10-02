#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
class CecControl
{
public:
    void StartScanning(bool); // 0x001407C0 | nintendogs:bytes [tier A]
    void Initialize(); // 0x0034D358 | fefates:bytes [tier B]
    void StopScanning(bool, bool); // 0x0034D3B8 | nintendogs:callgraph [tier A]
    void Suspend(); // 0x0034D574 | nintendogs:bytes [tier A]
    void Finalize(); // 0x0034D604 | nintendogs:callseq [tier A]
};
} // namespace CTR
} // namespace cec
} // namespace nn
