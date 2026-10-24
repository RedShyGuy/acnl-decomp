#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"
#include "nw/snd/internal/snd_BankFile_FileHeader.h"
#include "nw/snd/internal/snd_BankFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// Reads the regions of a bank file (the class and function names are from the reference symbols,
// the members are ours).
class BankFileReader
{
public:
    BankFileReader(); // 0x004C81D8 (name after the other constructor)
    BankFileReader(const void* pBankFile); // 0x0013BB38 | nintendogs:bytes [tier A]

    void Initialize(const void* pBankFile); // 0x004C8164 | fefates:bytes [tier B]
    void Finalize(); // 0x004C81B8 | fefates:bytes [tier B]
    // the region of a program number, key and velocity; false if there is none or it has no wave
    bool ReadVelocityRegionInfo(VelocityRegionInfo* pInfo, int programNo, int key, int velocity) const; // 0x0073F6E0 | nintendogs:bytes [tier A]
    // the waves of the bank (NULL: not initialized)
    const BinaryTable<WaveId>* GetWaveIdTable() const; // 0x0013D38C | nintendogs:bytes [tier A]

    const BankFile::FileHeader* m_pHeader;          // 0x0
    const BankFile::InfoBlockBody* m_pInfoBlockBody; // 0x4
    bool m_IsInitialized;                           // 0x8
};
ASSERT_SIZE(BankFileReader, 0xC);
} // namespace internal
} // namespace snd
} // namespace nw
