#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::InfoBlockBody
{
public:
    // the channel (NULL: beyond the table)
    const ChannelInfo* GetChannelInfo(int channelIndex) const; // 0x007431B8 | nintendogs:bytes [tier A]

    Encoding encoding;                  // 0x00
    bool isLoop;                        // 0x01 (1)
    u16 padding;                        // 0x02
    u32 sampleRate;                     // 0x04
    u32 loopStartFrame;                 // 0x08
    u32 loopEndFrame;                   // 0x0C
    u32 originalLoopStartFrame;         // 0x10 (version 2.1.1.0 and later)
    BinaryReferenceTable channelInfoTable; // 0x14
};
