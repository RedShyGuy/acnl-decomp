#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd12SoundArchiveE @ 0x008D08E0
// vtable 0x00902EDC (vptr 0x00902EE4), offset_to_top 0, 6 entries
class SoundArchive
{
public:
    class StreamTrackInfo;
    struct BankInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct FileInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct FileStreamPriority { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct GroupInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PlayerInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SequenceSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct Sound3DInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SoundArchivePlayerInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SoundGroupInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct StreamSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct StreamSoundInfo2 { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct WaveArchiveInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct WaveSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~SoundArchive(); // 0x004C0348 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x004C0338 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void OpenStream(void*, int, unsigned int, unsigned int); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const; // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    void IsAvailable() const; // 0x00139ACC | nintendogs:callseq-callee [tier A]
    void GetSoundType(unsigned) const; // 0x00139D4C | nintendogs:callgraph [tier A]
    void ReadBankInfo(unsigned, nw::snd::SoundArchive::BankInfo*) const; // 0x0013CF84 | nintendogs:callseq-callee [tier A]
    void ReadSoundInfo(unsigned, nw::snd::SoundArchive::SoundInfo*) const; // 0x0013CFF0 | nintendogs:callseq-callee [tier A]
    void ReadSequenceSoundInfo(unsigned, nw::snd::SoundArchive::SequenceSoundInfo*) const; // 0x0013D048 | nintendogs:callgraph [tier A]
    void detail_ReadWaveSoundInfo(unsigned, nw::snd::SoundArchive::WaveSoundInfo*) const; // 0x0013D0A0 | nintendogs:callgraph [tier A]
    void detail_ReadSoundGroupInfo(unsigned, nw::snd::SoundArchive::SoundGroupInfo*) const; // 0x0013D0F8 | nintendogs:callgraph [tier A]
    void GetItemFileId(unsigned) const; // 0x0013D520 | nintendogs:callgraph [tier A]
    void ReadWaveArchiveInfo(unsigned, nw::snd::SoundArchive::WaveArchiveInfo*) const; // 0x0013FA10 | nintendogs:callgraph [tier A]
    void detail_ReadFileInfo(unsigned, nw::snd::SoundArchive::FileInfo*) const; // 0x0013FA68 | nintendogs:callgraph [tier A]
    void detail_OpenFileStream(unsigned int, void*, int, void*, unsigned int, const char*, nw::snd::SoundArchive::FileStreamPriority); // 0x001410F4 | fefates:bytes [tier B]
    void detail_GetWaveArchiveIdTable(unsigned) const; // 0x0014200C | nintendogs:callgraph [tier A]
    void OpenExtStreamImpl(void*, int, const char*, unsigned, unsigned, nw::snd::SoundArchive::FileStreamPriority) const; // 0x00143980 | nintendogs:bytes-fuzzy [tier A]
    void Initialize(nw::snd::internal::SoundArchiveFileReader*); // 0x004C01B4 | fefates:bytes [tier B]
    void SetExternalFileRoot(const char*); // 0x004C022C | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004C02EC | fefates:bytes [tier B]
    SoundArchive(); // 0x004C0304 | fefates:bytes [tier B]
    void GetItemLabel(unsigned int) const; // 0x0073EAF4 | fefates:bytes [tier B]
    void ReadPlayerInfo(unsigned int, nw::snd::SoundArchive::PlayerInfo*) const; // 0x0073EB94 | fefates:bytes [tier B]
    void ReadSound3DInfo(unsigned int, nw::snd::SoundArchive::Sound3DInfo*) const; // 0x0073EBEC | fefates:bytes [tier B]
    void ReadSoundUserParam(unsigned int, int, unsigned int&) const; // 0x0073EC44 | fefates:bytes [tier B]
    void ReadStreamSoundInfo(unsigned int, nw::snd::SoundArchive::StreamSoundInfo*) const; // 0x0073ECA4 | fefates:bytes [tier B]
    void ReadSoundArchivePlayerInfo(nw::snd::SoundArchive::SoundArchivePlayerInfo*) const; // 0x0073ECFC | nintendogs:callgraph [tier A]
    void detail_ReadStreamSoundInfo2(unsigned int, nw::snd::SoundArchive::StreamSoundInfo2*) const; // 0x0073ED4C | fefates:bytes [tier B]
    void GetItemId(const char*) const; // 0x0073EDAC | fefates:bytes [tier B]
    void detail_GetFileCount() const; // 0x00740C94 | nintendogs:callgraph [tier A]
    void GetWaveArchiveCount() const; // 0x00740E78 | nintendogs:callseq-callee [tier A]
};
} // namespace snd
} // namespace nw
