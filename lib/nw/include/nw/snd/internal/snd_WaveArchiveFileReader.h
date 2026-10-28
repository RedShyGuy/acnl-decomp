#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveArchiveFile.h"
#include "nw/snd/internal/snd_WaveArchiveFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// Gives the wave files of a wave archive: in the file block, or for an archive whose waves are
// loaded one by one, from the table in the file block that holds their addresses (the class and
// function names are from the reference symbols, the members are ours).
class WaveArchiveFileReader
{
public:
    WaveArchiveFileReader(); // 0x004C98D8 (name after the other constructor)
    WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual); // 0x0013F344 | fefates:bytes [tier B]

    void Initialize(const void* pWaveArchiveFile, bool isIndividual); // 0x004C980C | fefates:bytes [tier B]
    void Finalize(); // 0x004C98B4 | fefates:bytes [tier B]

    // the wave file (NULL: beyond the archive or not loaded)
    const void* GetWaveFile(u32 waveIndex) const; // 0x0013FE8C | nintendogs:callseq-callee [tier A]
    // the loaded wave file set in the table; the old one (NULL: no table)
    const void* SetWaveFile(u32 waveIndex, const void* pWaveFile); // 0x0013F310 | nintendogs:callseq-callee [tier A]
    bool HasIndividualLoadTable() const; // 0x0013FEE4 | fefates:bytes [tier B]
    // all entries of the table NULL
    void InitializeFileTable(); // 0x00141914 | nintendogs:callseq-callee [tier A]
    u32 GetWaveFileSize(u32 waveIndex) const; // 0x00141F30 | fefates:bytes [tier B]
    u32 GetWaveFileOffsetFromFileHead(u32 waveIndex) const; // 0x00141F58 | fefates:bytes [tier B]

    // the waves of the archive (0: not initialized)
    u32 GetWaveFileCount() const; // 0x0013D508 (name is ours)

    const WaveArchiveFile::FileHeader* m_pHeader;          // 0x0
    const WaveArchiveFile::InfoBlockBody* m_pInfoBlockBody; // 0x4
    const void** m_pLoadTable;                             // 0x8 the addresses of the loaded waves (individual load)
    bool m_IsInitialized;                                  // 0xC
};
ASSERT_SIZE(WaveArchiveFileReader, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
