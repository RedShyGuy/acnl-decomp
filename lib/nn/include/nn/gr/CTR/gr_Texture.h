#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class Texture
{
public:
    class UnitBase;
    class Unit0;
    void MakeDisableCommand(unsigned int*, bool); // 0x0034B1BC | fefates:bytes [tier B]
    Texture(); // 0x0034B248 | fefates:bytes [tier B]
    void MakeCommand(unsigned*, bool) const; // 0x007281E0 | nintendogs:bytes-fuzzy [tier A]
    void MakeFuncCommand(unsigned int*, bool) const; // 0x00728220 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace gr
} // namespace nn
