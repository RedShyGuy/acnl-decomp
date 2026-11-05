#pragma once

#include "decomp.h"
#include "sead/seadAudioSubsetBase.h"

// RTTI 18SoundDoubutsugoMgr @ 0x008CC8C8
// vtable 0x008F4AA4 (vptr 0x008F4AAC), offset_to_top 0, 20 entries
class SoundDoubutsugoMgr : public ::sead::AudioSubsetBase
{
public:
    SoundDoubutsugoMgr(); // ctor candidate(s) 0x00128D1C (unverified)
    virtual ~SoundDoubutsugoMgr();
    virtual void calc() override; // 0x002E7C3C slot 0x1C
};
