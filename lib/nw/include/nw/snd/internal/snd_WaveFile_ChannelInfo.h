#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::ChannelInfo
{
public:
    void GetDspAdpcmInfo() const; // 0x007431A0 | nintendogs:callseq-callee [tier A]
    void GetSamplesAddress(const void*) const; // 0x007431AC | nintendogs:callseq-callee [tier A]
};
