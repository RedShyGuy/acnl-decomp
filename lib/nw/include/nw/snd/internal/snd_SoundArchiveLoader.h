#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal18SoundArchiveLoaderE @ 0x008D0A14
// vtable 0x00903278 (vptr 0x00903280), offset_to_top 0, 5 entries
class SoundArchiveLoader
{
public:
    SoundArchiveLoader(); // ctor candidate(s) 0x0013C2EC (unverified)
    virtual void vf_0x00(); // 0x004C8CF4 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x004C8CDC slot 0x04 | virtual slot, introduced by nw::snd::internal::SoundArchiveLoader
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    void LoadData(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned); // 0x00132564 | nintendogs:callgraph [tier A]
    void IsAvailable() const; // 0x001347B8 | nintendogs:callgraph [tier A]
    void IsWaveSoundDataLoaded(unsigned, unsigned) const; // 0x001348A8 | nintendogs:bytes [tier B]
    void IsSoundGroupDataLoaded(unsigned, unsigned) const; // 0x00134AA0 | nintendogs:bytes [tier B]
    void LoadSoundGroup(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned); // 0x00137D94 | nintendogs:bytes [tier A]
    void LoadSequenceSound(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned); // 0x00137F44 | nintendogs:bytes [tier A]
    void PostProcessForLoadedGroupFile(const void*, nw::snd::SoundMemoryAllocatable*, unsigned); // 0x00138028 | nintendogs:callseq [tier A]
    void LoadWaveSound(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned, unsigned); // 0x0013BC2C | nintendogs:bytes [tier A]
    void LoadWaveArchiveImpl(unsigned, unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned); // 0x0013BDC0 | nintendogs:bytes [tier A]
    void SetWaveArchiveTableInEmbeddedGroupImpl(unsigned, nw::snd::SoundMemoryAllocatable*); // 0x0013BE80 | nintendogs:callseq [tier A]
    void LoadBank(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, unsigned); // 0x0013BFB8 | nintendogs:callgraph [tier A]
    void LoadImpl(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned, bool); // 0x0013C1C4 | nintendogs:bytes [tier A]
    void LoadIndividualWave(unsigned, unsigned, nw::snd::SoundMemoryAllocatable*, unsigned); // 0x0013F19C | nintendogs:callgraph [tier A]
    void LoadWaveArchiveTable(unsigned, nw::snd::SoundMemoryAllocatable*, unsigned); // 0x0014123C | nintendogs:callseq [tier A]
    void ReadFile(unsigned, void*, unsigned, int, unsigned); // 0x001413C4 | nintendogs:callgraph [tier A]
    void SetSoundArchive(const nw::snd::SoundArchive*); // 0x004C8B2C | nintendogs:callgraph [tier A]
    void detail_LoadWaveArchiveByBankFile(const void*, nw::snd::SoundMemoryAllocatable*); // 0x004C8B34 | nintendogs:callseq [tier A]
    void detail_LoadWaveArchiveByWaveSoundFile(const void*, int, nw::snd::SoundMemoryAllocatable*); // 0x004C8C54 | nintendogs:bytes [tier A]
    void detail_GetFileAddressByItemId(unsigned) const; // 0x007406F8 | nintendogs:callseq-callee [tier A]
    void GetFileAddressFromSoundArchive(unsigned) const; // 0x007407DC | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
