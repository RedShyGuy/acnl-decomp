#include "sead/seadRandom.h"
#include "sead/seadTickTime.h"

namespace sead {
// 0x00139138 (name after pead::Random::init)
void sead::Random::init()
{
    init(static_cast<u32>(TickTime().mTick));
}

// 0x0055D5A4 | nintendogs:bytes [tier A]
void sead::Random::init(u32 seed)
{
    mX = (seed ^ (seed >> 30)) * 0x6C078965 + 1;
    mY = (mX ^ (mX >> 30)) * 0x6C078965 + 2;
    mZ = (mY ^ (mY >> 30)) * 0x6C078965 + 3;
    mW = (mZ ^ (mZ >> 30)) * 0x6C078965 + 4;
}

// 0x0055D5FC (name is ours)
void sead::Random::init(u32 x, u32 y, u32 z, u32 w)
{
    if ((x | y | z | w) == 0) {
        // the state of init(0)
        mX = 1;
        mY = 0x6C078967;
        mZ = 0x714ACB41;
        mW = 0x48077044;
    } else {
        mX = x;
        mY = y;
        mZ = z;
        mW = w;
    }
}

// 0x0055D640 | mk7dlp:callgraph [tier C]
u32 sead::Random::getU32()
{
    u32 t = mX ^ (mX << 11);
    mX = mY;
    mY = mZ;
    mZ = mW;
    mW = (mW ^ (mW >> 19)) ^ (t ^ (t >> 8));
    return mW;
}

} // namespace sead
