#include "nn/nex/nex_Platform.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/nex/nex_MD5.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nex {
namespace {
// the start values of the data of the seed
const u64 SEED_DATA[3] = {0x024F8D827839D088ULL, 0x0F831237889EF409ULL, 0xAB37798789797343ULL};
} // namespace

// 0x00ADDE54 (name is ours)
RandomNumberGenerator Platform::s_RandomNumberGenerator;
// 0x0096C913 (name is ours)
bool Platform::s_IsRandomNumberGeneratorSeeded;
// 0x0096CAD0 (name is ours)
u64 Platform::s_TransferableId;
// 0x0096C938 (name is ours)
bool Platform::s_IsTransferableIdRead;

// 0x003D5830 | fefates:callseq [tier C]
u32 nn::nex::Platform::GetRandomSeed()
{
    u64 data[3] = {SEED_DATA[0], SEED_DATA[1], SEED_DATA[2]};
    data[0] ^= (nn::fnd::DateTime::GetNow() - nn::fnd::DateTime::MIN_DATETIME).GetMilliSeconds();
    data[1] ^= nn::svc::GetSystemTick();
    if (!s_IsTransferableIdRead) {
        nn::cfg::CTR::Initialize();
        s_TransferableId = nn::cfg::CTR::GetTransferableId(0);
        nn::cfg::CTR::Finalize();
        s_IsTransferableIdRead = true;
    }
    data[2] ^= s_TransferableId;
    MD5 md5;
    md5.Update(data, sizeof(data));
    const u32* pDigest = reinterpret_cast<const u32*>(md5.GetDigest());
    return pDigest[0] ^ pDigest[1] ^ (pDigest[2] ^ pDigest[3]);
}

// 0x003D59A8 | mk7dlp:callgraph [tier A]
u32 nn::nex::Platform::GetRandomNumber(unsigned max)
{
    return GetRandomNumberGenerator().GetRandomNumber(max);
}

} // namespace nex
} // namespace nn
