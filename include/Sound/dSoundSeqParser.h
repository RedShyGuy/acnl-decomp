#pragma once

#include "decomp.h"
#include "Sound/dSoundConductor.h"

namespace sead { class Heap; }

// Reads events out of the data of a sequence without playing it: walks the MML commands of a track and fills a table of
// {value, tick} (the class name is from the RTTI of its disposer, everything else is ours).
class SoundSeqParser
{
public:
    class SingletonDisposer_;

    // what is collected
    enum Mode
    {
        MODE_FIRST_NOTE = 0,   // the key of the first note after each wait
        MODE_LONGEST_NOTE = 1, // the length of the longest note after each wait
        MODE_TEMPO = 2,        // the tempo commands
        MODE_VARIABLE = 3,     // the assignments to one variable
    };
    typedef SoundConductor::TempoEntry Entry;

    SoundSeqParser();
    static SoundSeqParser* createInstance(sead::Heap* heap); // 0x00123604

    void Parse(Entry* entries, u32 track, u32 mode, s32 entryNum); // 0x00278898
    void Load(u32 soundId); // 0x00278980
    void ParseVariable(Entry* entries, u32 track, u8 variable, s32 entryNum); // 0x00278A40
    void ParseData(Entry* entries, const u8* data, u32 mode, s32 entryNum); // 0x00278A5C

    static SoundSeqParser* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00
    u32 m_SoundId;                    // 0x10
    u32 m_StartOffset;                // 0x14
    const u8* m_pData;                // 0x18
    u8 m_Variable;                    // 0x1C
};
ASSERT_OFFSET(SoundSeqParser, m_SoundId, 0x10);
ASSERT_SIZE(SoundSeqParser, 0x20);
