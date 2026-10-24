#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::ChannelInfo
{
public:
    // the DSP ADPCM parameters followed by the loop parameters
    const void* GetDspAdpcmInfo() const; // 0x007431A0 | nintendogs:bytes [tier A]
    // the samples in the body of the data block
    const void* GetSamplesAddress(const void* pDataBlockBody) const; // 0x007431AC | nintendogs:bytes [tier A]

    BinaryReference samples;      // 0x0 from the body of the data block
    BinaryReference dspAdpcmInfo; // 0x8 (offset 0: none)
};
