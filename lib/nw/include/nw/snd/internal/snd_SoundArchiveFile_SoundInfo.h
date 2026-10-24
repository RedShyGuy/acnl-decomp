#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

class nw::snd::internal::SoundArchiveFile::SoundInfo
{
public:
    // the bits of the optional values (names are ours)
    enum OptionBit
    {
        OPTION_STRING_ID = 0,
        OPTION_PAN = 1,               // byte 0 the pan mode, byte 1 the pan curve
        OPTION_PLAYER = 2,            // byte 0 the player priority, byte 1 the actor player
        OPTION_SOUND_3D = 8,          // the offset of the Sound3DInfo
        OPTION_FRONT_BYPASS = 17,
    };

    u32 GetSoundType() const; // 0x0013FE44 | nintendogs:bytes [tier A]
    const WaveSoundInfo* GetWaveSoundInfo() const; // 0x0013FE74 | nintendogs:callgraph [tier A]
    const SequenceSoundInfo* GetSequenceSoundInfo() const; // 0x0013FE80 (name after GetWaveSoundInfo)
    const StreamSoundInfo* GetStreamSoundInfo() const; // 0x007403EC | nintendogs:bytes [tier C]
    const StreamSoundInfoLegacy* GetStreamSoundInfoLegacy() const; // 0x007403F8 | nintendogs:bytes [tier C]
    u8 GetPanMode() const; // 0x00141E68 | nintendogs:bytes [tier A]
    u8 GetPanCurve() const; // 0x00141E8C | nintendogs:bytes [tier A]
    bool IsFrontBypass() const; // 0x00141EB8 | nintendogs:bytes [tier A]
    u8 GetActorPlayerId() const; // 0x00141EDC | nintendogs:bytes [tier A]
    u8 GetPlayerPriority() const; // 0x00141F08 | nintendogs:bytes [tier A]
    bool ReadUserParam(int index, u32& value) const; // 0x007403A0 | fefates:bytes [tier B]
    const void* GetSound3DInfo() const; // 0x007403C4 | nintendogs:bytes [tier A]

    u32 fileId;                 // 0x00
    u32 playerId;               // 0x04
    u8 volume;                  // 0x08
    BinaryReference detailInfo; // 0x0C the info of the kind of sound
    Util::BitFlag optionParams; // 0x14
};
