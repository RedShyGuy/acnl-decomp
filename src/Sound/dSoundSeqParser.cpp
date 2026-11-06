#include "Sound/dSoundSeqParser.h"
#include "Sound/dSoundSeqParser_SingletonDisposer_.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_SequenceSoundFileReader.h"
#include "nw/snd/snd_SoundArchive.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"
#include "sead/seadNew.h"

namespace {
// the MML commands that matter here
const u32 MML_WAIT = 0x80;
const u32 MML_PRG = 0x81;
const u32 MML_OPEN_TRACK = 0x88;
const u32 MML_JUMP = 0x89;
const u32 MML_CALL = 0x8A;
const u32 MML_IF = 0xA2;
const u32 MML_TIME = 0xA3;
const u32 MML_TIME_RANDOM = 0xA4;
const u32 MML_TIME_VARIABLE = 0xA5;
const u32 MML_RANDOM = 0xA0;
const u32 MML_VARIABLE = 0xA1;
const u32 MML_TEMPO = 0xE1;
const u32 MML_EX_COMMAND = 0xF0;
const u32 MML_RET = 0xFD;
const u32 MML_ALLOC_TRACK = 0xFE;
const u32 MML_FIN = 0xFF;
const u32 MML_EX_SET_VARIABLE = 0x80;

const s32 HEADER_COMMAND_NUM = 50;
const s32 COMMAND_NUM = 1000000;
const s32 KEY_CENTER = 60;
const u32 DEFAULT_VALUE = 1000000;
const u32 END_TICK = 0x7FFFFFFF;

inline u32 ReadU24(const u8*& data)
{
    u32 value = *data++;
    value = (value << 8) | *data++;
    value = (value << 8) | *data++;
    return value;
}

inline s32 ReadVarLength(const u8*& data)
{
    s32 value = 0;
    u32 byte;
    do {
        byte = *data++;
        value = (value << 7) | (byte & 0x7F);
    } while (byte & 0x80);
    return value;
}
} // namespace

// 0x0094E570
SoundSeqParser* SoundSeqParser::s_pInstance = NULL;
// 0x0094E574
SoundSeqParser::SingletonDisposer_* SoundSeqParser::s_pSingletonDisposer = NULL;

SoundSeqParser::SoundSeqParser() : m_SoundId(0xFFFFFFFF), m_StartOffset(0), m_pData(NULL), m_Variable(0)
{
}

// 0x00123604
SoundSeqParser* SoundSeqParser::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundSeqParser)];
        SoundSeqParser* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundSeqParser();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00278898
void SoundSeqParser::Parse(Entry* entries, u32 track, u32 mode, s32 entryNum)
{
    if (m_SoundId == 0xFFFFFFFF) {
        return;
    }
    for (s32 i = 0; i < entryNum; i++) {
        entries[i].tempo = DEFAULT_VALUE;
        entries[i].tick = END_TICK;
    }
    u32 offset = 0;
    if (track != 0) {
        const u8* data = m_pData + m_StartOffset;
        bool isFound = false;
        for (s32 i = 0; i < HEADER_COMMAND_NUM; i++) {
            u32 command = *data++;
            if (command == MML_FIN) {
                return;
            }
            if (command != MML_OPEN_TRACK) {
                continue;
            }
            u32 number = *data++;
            offset = ReadU24(data);
            if (number == track) {
                if (offset == 0xFFFFFFFF) {
                    return;
                }
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            return;
        }
    }
    ParseData(entries, m_pData + offset, mode, entryNum);
}

// 0x00278980
void SoundSeqParser::Load(u32 soundId)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    sead::AudioSoundDataMgrCtr* dataMgr = mgr->m_pPlayer->m_pSoundDataMgr;
    nw::snd::SoundArchive* archive = mgr->m_pArchive;
    nw::snd::SoundArchive::SequenceSoundInfo info;
    if (!archive->ReadSequenceSoundInfo(soundId, &info)) {
        return;
    }
    m_StartOffset = info.startOffset;
    nw::snd::SoundArchive::SoundInfo soundInfo;
    if (!archive->ReadSoundInfo(soundId, &soundInfo)) {
        return;
    }
    nw::snd::internal::SequenceSoundFileReader reader(dataMgr->detail_GetFileAddress(soundInfo.fileId));
    m_SoundId = soundId;
    m_pData = static_cast<const u8*>(reader.m_pDataBlockBody);
}

// 0x00278A40
void SoundSeqParser::ParseVariable(Entry* entries, u32 track, u8 variable, s32 entryNum)
{
    m_Variable = variable;
    Parse(entries, track, MODE_VARIABLE, entryNum);
}

// 0x00278A5C
void SoundSeqParser::ParseData(Entry* entries, const u8* data, u32 mode, s32 entryNum)
{
    (void)entryNum;
    if (mode >= 4) {
        return;
    }
    u32 tick = 0;
    s32 index = 0;
    s32 noteCount = 0;
    const u8* returnAddress = NULL;
    for (s32 i = 0; i < COMMAND_NUM; i++) {
        u32 command = *data++;
        if (command == MML_IF) {
            command = *data++;
        }
        if (command == MML_TIME || command == MML_TIME_RANDOM || command == MML_TIME_VARIABLE) {
            command = *data++;
        }
        if (command == MML_RANDOM || command == MML_VARIABLE) {
            command = *data++;
        }
        if ((command & 0x80) == 0) {
            data++;
            s32 length = ReadVarLength(data);
            s32 key = static_cast<s32>(command) - KEY_CENTER;
            if (mode == MODE_FIRST_NOTE) {
                if (noteCount == 0) {
                    noteCount = 1;
                    entries[index].tick = tick;
                    entries[index].tempo = key;
                }
            } else if (mode == MODE_LONGEST_NOTE) {
                if (noteCount == 0) {
                    noteCount = 1;
                    entries[index].tempo = length;
                    entries[index].tick = tick;
                } else if (static_cast<s32>(entries[index].tempo) < length) {
                    noteCount++;
                    entries[index].tempo = length;
                    entries[index].tick = tick;
                }
            }
            continue;
        }
        switch ((command & 0xF0) >> 4) {
        case 0x8:
            if (command == MML_OPEN_TRACK) {
                data += 4;
            } else if (command == MML_WAIT) {
                tick += ReadVarLength(data);
                if (noteCount != 0) {
                    noteCount = 0;
                    index++;
                }
            } else if (command == MML_PRG) {
                ReadVarLength(data);
            } else if (command == MML_JUMP) {
                data += 3;
            } else if (command == MML_CALL) {
                u32 offset = ReadU24(data);
                if (returnAddress == NULL) {
                    returnAddress = data;
                    data = m_pData + offset;
                }
            }
            break;
        case 0xB:
        case 0xC:
        case 0xD:
            data++;
            break;
        case 0xE: {
            u32 value = *data++ << 8;
            value = (value & 0xFFFF) | *data++;
            if (mode == MODE_TEMPO && command == MML_TEMPO) {
                entries[index].tempo = value;
                entries[index].tick = tick;
                noteCount++;
            }
            break;
        }
        case 0xF:
            if (command == MML_EX_COMMAND) {
                u32 extended = *data++;
                u32 group = extended & 0xF0;
                if (group == 0x80 || group == 0x90) {
                    u32 variable = *data++;
                    u32 value = *data++ << 8;
                    value = (value & 0xFFFF) | *data++;
                    if (mode == MODE_VARIABLE && extended == MML_EX_SET_VARIABLE && m_Variable == variable) {
                        entries[index].tempo = value;
                        entries[index].tick = tick;
                        noteCount++;
                    }
                } else if (group == 0xE0) {
                    data += 2;
                }
            } else if (command == MML_RET) {
                if (returnAddress != NULL) {
                    data = returnAddress;
                    returnAddress = NULL;
                }
            } else if (command == MML_ALLOC_TRACK) {
                data += 2;
            } else if (command == MML_FIN) {
                if (noteCount != 0) {
                    index++;
                }
                entries[index].tick = END_TICK;
                i = COMMAND_NUM;
            }
            break;
        }
    }
}
