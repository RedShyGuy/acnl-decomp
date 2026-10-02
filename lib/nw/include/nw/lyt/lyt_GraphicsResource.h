#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
class GraphicsResource
{
public:
    void FinishSetup(); // 0x004B5708 | nintendogs:callseq [tier A]
    void SetResource(int, void*, unsigned, bool); // 0x004B57E8 | nintendogs:bytes-fuzzy [tier A]
    void ResetGlState(); // 0x004B5A1C | libgarden [tier A]
    void GetResourcePath(int); // 0x004B5A28 | nintendogs:bytes [tier A]
    void SetProjectionMtx(nn::math::MTX44 const&); // 0x004B5B10 | libgarden [tier A]
    void ResetGlProgramState(); // 0x004B5B84 | libgarden [tier A]
    void InitVBO(); // 0x004B5B9C | nintendogs:bytes [tier A]
    void FinalizeGraphics(); // 0x004BEE70 | libgarden [tier A]
};
} // namespace lyt
} // namespace nw
