#include "pead/peadRandom.h"
#include "pead/peadTickTime.h"

namespace pead {
// 0x0053BEF0 | nintendogs:bytes [tier A]
void pead::Random::init()
{
    init(static_cast<u32>(TickTime().mTick));
}

// 0x0053BF64 (name is ours)
u32 pead::Random::getU32()
{
    u32 t = mX ^ (mX << 11);
    mX = mY;
    mY = mZ;
    mZ = mW;
    mW = (mW ^ (mW >> 19)) ^ (t ^ (t >> 8));
    return mW;
}
} // namespace pead
