#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class Combiner
{
public:
    class Stage;
    Combiner(); // 0x0034B548 | fefates:bytes-fuzzy [tier B]
    void MakeCommand(unsigned int*) const; // 0x0072893C | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace gr
} // namespace nn
