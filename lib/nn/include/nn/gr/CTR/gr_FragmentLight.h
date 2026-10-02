#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class FragmentLight
{
public:
    class Source;
    void MakeDisableCommand(unsigned int*, bool); // 0x00349A28 | fefates:bytes [tier B]
    FragmentLight(); // 0x00349AE8 | nintendogs:bytes-fuzzy [tier A]
    void MakeAllCommand(unsigned*, bool) const; // 0x007271F0 | nintendogs:bytes [tier B]
    void MakeLightEnvCommand(unsigned*, bool) const; // 0x0072724C | nintendogs:callgraph [tier A]
    void MakeLutConfigCommand(unsigned*) const; // 0x007274BC | nintendogs:bytes [tier B]
};
} // namespace CTR
} // namespace gr
} // namespace nn
