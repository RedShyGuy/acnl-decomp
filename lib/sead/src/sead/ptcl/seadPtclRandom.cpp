#include "sead/ptcl/seadPtclRandom.h"
#include "sead/seadGlobalRandom.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadNew.h"
#include "sead/seadRandom.h"

namespace sead {
namespace ptcl {
// 0x0097F85C
Vector3<f32>* PtclRandom::s_pVec3Tbl = nullptr;
// 0x0097F860
Vector3<f32>* PtclRandom::s_pNormalizedVec3Tbl = nullptr;

// 0x0054EF78 | mk7dlp:bytes
void sead::ptcl::PtclRandom::CreateRandomTbl(Heap* heap)
{
    Random random;
    random.init(12345679);
    s_pVec3Tbl = new (heap, 4) Vector3<f32>[512];
    s_pNormalizedVec3Tbl = new (heap, 4) Vector3<f32>[512];
    for (s32 i = 0; i < 512; i++) {
        s_pVec3Tbl[i].x = random.getF32() * 2.0f - 1.0f;
        s_pVec3Tbl[i].y = random.getF32() * 2.0f - 1.0f;
        s_pVec3Tbl[i].z = random.getF32() * 2.0f - 1.0f;
        s_pNormalizedVec3Tbl[i].x = random.getF32() * 2.0f - 1.0f;
        s_pNormalizedVec3Tbl[i].y = random.getF32() * 2.0f - 1.0f;
        s_pNormalizedVec3Tbl[i].z = random.getF32() * 2.0f - 1.0f;
        Vector3CalcCtr<f32>::normalize(toVEC3(s_pNormalizedVec3Tbl[i]));
    }
}

// 0x0054F118 | mk7dlp:bytes [tier B]
sead::ptcl::PtclRandom::PtclRandom()
{
    Random* random = GlobalRandom::s_pInstance;
    mVec3TblIndex = random->getU32();
    mNormalizedVec3TblIndex = random->getU32();
    mSeed = random->getU32();
}
} // namespace ptcl
} // namespace sead
