#include "script/dTalkSeq.h"
#include "script/dTalkSystem.h"
#include "script/dTalkWin.h"
#include "Sound/dSoundMgr.h"

namespace script {
// 0x005D5270 (name is ours)
void TalkSong::UpdateEnd()
{
    // empty (in the original too)
}

// 0x005D5274 (name is ours)
void TalkSong::UpdateRepeat()
{
    if (phrase == curPhrase) {
        return;
    }
    if (phrase == -1) {
        prevPhrase = curPhrase;
        curPhrase = -1;
        state = STATE_END;
        return;
    }
    TalkSystem* talk = pOwner;
    talk->HideWaitIcon(false);
    pOwner->m_IsJumped = 1;
    isWaiting = 1;
    prevPhrase = curPhrase;
    curPhrase = 3;
    state = STATE_SING;
}

// 0x005D52E8 (name is ours)
void TalkSong::UpdateIdle()
{
    if (phrase == -1) {
        return;
    }
    isWaiting = 1;
    prevPhrase = curPhrase;
    curPhrase = 0;
    state = STATE_SING;
}

// 0x005D5314 (name is ours)
void TalkSong::UpdateSing()
{
    if (phrase == curPhrase) {
        return;
    }
    pOwner->m_pSeq->m_Song.isStopped = 1;
    state = STATE_NEXT;
}

// 0x005D5340 (name is ours)
void TalkSong::UpdateNext()
{
    TalkSystem* talk = pOwner;
    if (talk->m_State != TalkSystem::STATE_PAGE) {
        return;
    }
    talk->HideWaitIcon(false);
    pOwner->m_IsJumped = 1;
    if (phrase == -1) {
        prevPhrase = curPhrase;
        curPhrase = -1;
        state = STATE_END;
        return;
    }
    isWaiting = 1;
    s16 current = curPhrase;
    switch (current) {
    case 0:
        prevPhrase = current;
        curPhrase = 1;
        state = STATE_SING;
        break;
    case 1:
        prevPhrase = current;
        curPhrase = 2;
        state = STATE_REPEAT;
        break;
    case 3:
        prevPhrase = current;
        curPhrase = 4;
        state = STATE_SING;
        break;
    default:
        prevPhrase = current;
        curPhrase = -1;
        state = STATE_END;
        break;
    }
}

// 0x005D5424 (name is ours)
void TalkSong::Update()
{
    if (!isPlaying) {
        return;
    }
    SoundMgr::SUpdateBarcarolle();
    prevNote = note;
    note = SoundMgr::SGetBarcarolleNoteIndex();
    if (note < prevNote) {
        noteNum = 0;
    } else if (note > prevNote) {
        noteNum = SoundMgr::SGetBarcarolleFrames();
        restNum += note - prevNote;
    }
    isBetween = SoundMgr::SIsBarcarolleBetween();
    isSinging = SoundMgr::SIsBarcarolleSinging();
    isActive = SoundMgr::SIsBarcarolleActive();
    phrase = SoundMgr::SGetBarcarollePhrase();
    if (isWaiting) {
        if (curPhrase == 2 ? isBetween : note != prevNote) {
            pOwner->SetWaitIcon(0);
            isWaiting = 0;
        }
    }
    (this->*s_StateFuncs[state])();
}

} // namespace script
