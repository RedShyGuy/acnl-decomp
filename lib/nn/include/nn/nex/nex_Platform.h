#pragma once

#include "decomp.h"
#include "nn/nex/nex_RandomNumberGenerator.h"

namespace nn {
namespace nex {
// The services of the platform for nex. The names of the members and of the inline functions are
// ours.
class Platform
{
public:
    // a random number below max (from the generator of nex)
    static u32 GetRandomNumber(unsigned max); // 0x003D59A8 | mk7dlp:callgraph [tier A]
    // a seed from the time, the system tick and the transferable id of the console (MD5)
    static u32 GetRandomSeed(); // 0x003D5830 | fefates:callseq [tier C]

    // (inline) the generator of nex, seeded on the first use
    static RandomNumberGenerator& GetRandomNumberGenerator()
    {
        if (!s_IsRandomNumberGeneratorSeeded) {
            s_RandomNumberGenerator.SetSeed(GetRandomSeed());
            s_IsRandomNumberGeneratorSeeded = true;
        }
        return s_RandomNumberGenerator;
    }

    static RandomNumberGenerator s_RandomNumberGenerator;
    static bool s_IsRandomNumberGeneratorSeeded;
    // the transferable id of the console (read once)
    static u64 s_TransferableId;
    static bool s_IsTransferableIdRead;
};
} // namespace nex
} // namespace nn
