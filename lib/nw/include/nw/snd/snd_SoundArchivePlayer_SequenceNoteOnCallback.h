#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_NoteOnCallback.h"
#include "nw/snd/snd_SoundArchivePlayer.h"

// RTTI N2nw3snd18SoundArchivePlayer22SequenceNoteOnCallbackE @ 0x008D0934
// vtable 0x00902F98 (vptr 0x00902FA0), offset_to_top 0, 3 entries
class nw::snd::SoundArchivePlayer::SequenceNoteOnCallback : public ::nw::snd::internal::driver::NoteOnCallback
{
public:
    SequenceNoteOnCallback(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C2B7C slot 0x00 | virtual slot, introduced by nw::snd::SoundArchivePlayer::SequenceNoteOnCallback
    virtual void vf_0x04(); // 0x004C2B78 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchivePlayer::SequenceNoteOnCallback
    virtual void NoteOn(nw::snd::internal::driver::SequenceSoundPlayer*, unsigned char, const nw::snd::internal::driver::NoteOnInfo&); // 0x004C2AF4 slot 0x08 | nintendogs:callseq
};
