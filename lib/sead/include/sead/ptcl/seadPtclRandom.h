#pragma once

#include "decomp.h"
#include "sead/seadVector3.h"

namespace sead {
class Heap;

namespace ptcl {
// The random numbers of a particle: indices into the two tables of random vectors and a seed (the class
// and CreateRandomTbl are from the reference symbols, the rest is ours).
class PtclRandom
{
public:
    // from the global random generator
    PtclRandom(); // 0x0054F118 | mk7dlp:bytes [tier B]

    // the 512 random vectors in [-1, 1) and the 512 random directions
    static void CreateRandomTbl(Heap* heap); // 0x0054EF78

    static Vector3<f32>* s_pVec3Tbl;
    static Vector3<f32>* s_pNormalizedVec3Tbl;

    // the indices and the seed from seed (inline; name is ours)
    void init(u32 seed)
    {
        mVec3TblIndex = seed;
        mNormalizedVec3TblIndex = seed >> 16;
        mSeed = seed;
    }
    // the seed, then the next one (a linear congruential generator; inline, names are ours)
    u32 getU32()
    {
        u32 x = mSeed;
        mSeed = x * 0x41C64E6D + 12345;
        return x;
    }
    // in [0, 1)
    f32 getF32() { return getU32() * (1.0f / 4294967296.0f); }
    // in [0, range) (range < 0: (range, 0])
    s32 getS32Range(s32 range) { return static_cast<s32>((static_cast<s64>(getU32()) * range) >> 32); }
    // the next vectors of the tables
    const Vector3<f32>& getVec3() { return s_pVec3Tbl[mVec3TblIndex++ & 0x1FF]; }
    const Vector3<f32>& getNormalizedVec3() { return s_pNormalizedVec3Tbl[mNormalizedVec3TblIndex++ & 0x1FF]; }

    u16 mVec3TblIndex;           // 0x0
    u16 mNormalizedVec3TblIndex; // 0x2
    u32 mSeed;                   // 0x4
};
ASSERT_SIZE(PtclRandom, 0x8);
} // namespace ptcl
} // namespace sead
