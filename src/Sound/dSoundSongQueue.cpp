#include "Sound/dSoundSongQueue.h"
#include "Sound/dSoundKKInfo.h"
#include "sead/seadRandom.h"
#include <cstring>

namespace {
// the scores of the songs
enum Score
{
    SCORE_NONE = 0,
    SCORE_IN_QUEUE = 1, // never picked
    SCORE_SKIP = 2,
    SCORE_SAME_KEY = 3,
    SCORE_OCTAVE_KEY = 4, // the same note in the other half of the keys
    SCORE_KEY_UP = 5,
    SCORE_KEY_DOWN = 6,
    SCORE_RELATIVE_KEY = 7,
};
} // namespace

// 0x002E7008
SoundSongQueueData::SoundSongQueueData()
{
    seed = 0;
    isAlternate = 1;
    index = 0;
    std::memset(songs, INVALID_SONG, SONG_NUM);
}

// 0x002E6F98
void SoundSongQueueData::Store(const SoundSongQueue* queue)
{
    for (u32 i = 0; i < SONG_NUM; i++) {
        if (queue->m_Count <= static_cast<s32>(i)) {
            songs[i] = INVALID_SONG;
        } else {
            songs[i] = *queue->At(i);
        }
    }
    seed = queue->m_Seed;
    index = queue->m_Index;
    isAlternate = queue->m_IsAlternate;
}

// 0x007214C8
void SoundSongQueueData::Load(SoundSongQueue* queue) const
{
    if (queue == NULL) {
        return;
    }
    queue->m_Count = 0;
    queue->m_Head = 0;
    queue->m_Seed = seed;
    queue->m_Index = index;
    queue->m_IsAlternate = isAlternate;
    for (u32 i = 0; i < SONG_NUM && songs[i] != INVALID_SONG; i++) {
        queue->PushBack(songs[i]);
    }
}

// 0x002A3140
void SoundSongQueue::Reset(SoundSongQueue* queue)
{
    if (queue == NULL) {
        return;
    }
    sead::Random random;
    random.init();
    queue->m_Count = 0;
    queue->m_Head = 0;
    for (u32 i = 0; i < 4; i++) {
        u8 song = queue->PickSong(&random);
        if (queue->m_Count >= queue->m_Capacity) {
            queue->PopFront();
        }
        queue->PushBack(song);
    }
    queue->m_Index = 0;
    queue->m_IsAlternate = 0;
    queue->m_Seed = random.getU32();
}

// 0x002A3228
u8 SoundSongQueue::PickSong(sead::Random* random)
{
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    if (info == NULL) {
        return 0;
    }
    if (m_Count == 0) {
        return random->getU32(SoundKKInfo::RANDOM_SONG_NUM) + SONG_FLAG;
    }

    s32 counts[5] = {0, 0, 0, 0, 0};
    u32 last = *(m_Count > 0 ? At(m_Count - 1) : m_pData) & SONG_MASK;
    s32 key = info->GetKey(last);

    // not more than three songs in a row in the same key
    bool isSameKeyAllowed = true;
    if (m_Count > 1) {
        s32 sameKey = 1;
        for (s32 i = 1; m_Count > i; i++) {
            const u8* song = static_cast<u32>(m_Count) <= static_cast<u32>(i) ? NULL : At(m_Count - i - 1);
            if (info->GetKey(*song & SONG_MASK) != key) {
                break;
            }
            sameKey++;
        }
        if (sameKey >= 3) {
            isSameKeyAllowed = false;
        }
    }

    s32 scores[SoundKKInfo::RANDOM_SONG_NUM];
    for (u32 song = 0; song < SoundKKInfo::RANDOM_SONG_NUM; song++) {
        scores[song] = SCORE_NONE;
        bool isInQueue = false;
        for (s32 i = 0; i < m_Count; i++) {
            if ((*At(i) & SONG_MASK) == song) {
                scores[song] = SCORE_IN_QUEUE;
                isInQueue = true;
                break;
            }
        }
        if (isInQueue) {
            continue;
        }
        if (info->GetBeatsPerBar(last) == 3 && info->GetBeatsPerBar(song) == 3) {
            scores[song] = SCORE_SKIP;
            continue;
        }

        s32 score = SCORE_NONE;
        switch (info->GetKey(song) - key) {
        case 0:
            if (isSameKeyAllowed) {
                scores[song] = SCORE_SAME_KEY;
                counts[0]++;
            } else {
                scores[song] = SCORE_SKIP;
            }
            continue;
        case 1:
            if (key != 11 && key != 23) {
                score = SCORE_KEY_UP;
            }
            break;
        case -11:
            if (key == 11 || key == 23) {
                score = SCORE_KEY_UP;
            }
            break;
        case -1:
            if (key != 0 && key != 12) {
                score = SCORE_KEY_DOWN;
            }
            break;
        case 11:
            if (key == 0 || key == 12) {
                score = SCORE_KEY_DOWN;
            }
            break;
        case 12:
            if (static_cast<u32>(key) < 12) {
                score = SCORE_OCTAVE_KEY;
            }
            break;
        case -12:
            if (static_cast<u32>(key) >= 12) {
                score = SCORE_OCTAVE_KEY;
            }
            break;
        case 21:
            if (static_cast<u32>(key) < 3) {
                score = SCORE_RELATIVE_KEY;
            }
            break;
        case 9:
            if (static_cast<u32>(key - 3) < 9) {
                score = SCORE_RELATIVE_KEY;
            }
            break;
        case -9:
            if (static_cast<u32>(key - 12) < 9) {
                score = SCORE_RELATIVE_KEY;
            }
            break;
        case -21:
            if (static_cast<u32>(key) >= 21) {
                score = SCORE_RELATIVE_KEY;
            }
            break;
        }
        if (score != SCORE_NONE) {
            scores[song] = score;
            counts[score - SCORE_SAME_KEY]++;
        }
    }

    s32 wanted = SCORE_NONE;
    if (counts[0] > 0) {
        wanted = SCORE_SAME_KEY;
    } else {
        u8 options[4];
        u32 optionNum = 0;
        if (counts[1] > 0) {
            options[0] = SCORE_OCTAVE_KEY;
            optionNum = 1;
        }
        if (counts[2] > 0) {
            options[optionNum++] = SCORE_KEY_UP;
        }
        if (counts[3] > 0) {
            options[optionNum++] = SCORE_KEY_DOWN;
        }
        if (counts[4] > 0) {
            options[optionNum++] = SCORE_RELATIVE_KEY;
        }
        if (optionNum != 0) {
            wanted = options[random->getU32(optionNum)];
        }
    }

    u32 candidates[SoundKKInfo::RANDOM_SONG_NUM];
    u32 candidateNum = 0;
    for (u32 song = 0; song < SoundKKInfo::RANDOM_SONG_NUM; song++) {
        if (scores[song] == wanted) {
            candidates[candidateNum++] = song;
        }
    }

    u32 picked;
    if (candidateNum != 0) {
        picked = candidates[random->getU32(candidateNum)];
    } else {
        picked = random->getU32(SoundKKInfo::RANDOM_SONG_NUM);
    }
    if (info->GetKey(picked) != key) {
        u8 back = *(m_Count > 0 ? At(m_Count - 1) : m_pData);
        if (back < SONG_FLAG) {
            return picked + SONG_FLAG;
        }
    }
    return picked;
}

// 0x002A36E4
void SoundSongQueue::Advance(SoundSongQueue* queue)
{
    if (queue == NULL) {
        return;
    }
    sead::Random random;
    random.init(queue->m_Seed);
    u8 song = queue->PickSong(&random);
    if (queue->m_Count >= queue->m_Capacity) {
        queue->PopFront();
    }
    queue->PushBack(song);
    if (queue->m_Index != 0) {
        queue->m_Index--;
    }
    queue->m_Seed = random.getU32();
}

// 0x002A37C0
void SoundSongQueue::MakeData(SoundSongQueueData* data)
{
    if (data == NULL) {
        return;
    }
    SoundSongQueue queue;
    Reset(&queue);
    data->Store(&queue);
}
