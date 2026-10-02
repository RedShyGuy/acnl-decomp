#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class FrameHeap
{
public:
    void SaveState(); // 0x001327AC | nintendogs:bytes [tier A]
    void Create(void*, unsigned); // 0x00138540 | nintendogs:bytes [tier A]
    FrameHeap(); // 0x001385C4 | nintendogs:bytes [tier A]
    void LoadState(int); // 0x0013F3FC | nintendogs:bytes-fuzzy [tier B]
    void NewSection(); // 0x0013F4D8 | nintendogs:bytes [tier A]
    void ClearSection(); // 0x00141964 | nintendogs:bytes [tier A]
    void ProcessCallback(int); // 0x001419FC | nintendogs:callgraph [tier A]
    void Clear(); // 0x00141AFC | nintendogs:bytes-fuzzy [tier A]
    void Destroy(); // 0x00141B20 | nintendogs:bytes [tier A]
    void Alloc(unsigned, void(*)(void*, unsigned long, void*), void*); // 0x004D4B3C | nintendogs:bytes [tier A]
    ~FrameHeap(); // 0x004D4BAC | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
