#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::InfoBlockBody
{
public:
    void GetChannelInfo(int) const; // 0x007431B8 | nintendogs:bytes [tier A]
};
