#include "nn/snd/CTR/snd_VoiceManager.h"
#include <new>
#include "nn/math/math_Api.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00AECE88 (name is ours)
VoiceManager g_VoiceManager;

// (the constructor is in a static initializer, 0x0079B824)
nn::snd::CTR::VoiceManager::VoiceManager()
{
    for (s32 i = 0; i < VOICE_COUNT; i++) {
        m_VoicePointers[i] = new (m_VoiceBuffer[i]) Voice(i);
        m_VoicePointers[i]->m_Impl = new (m_VoiceImplBuffer[i]) VoiceImpl(i);
    }
}

// 0x00130820 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceManager::Initialize()
{
    m_Head = NULL;
    m_Tail = NULL;
    m_UsedMask = 0;
    m_UsedCount = 0;
    m_Mode = 0;
    m_Lock.Initialize();
}

// takes the voice out of the list
inline void nn::snd::CTR::VoiceManager::Remove(Voice* voice)
{
    Voice* prev = voice->m_Prev;
    Voice* next = voice->m_Next;
    if (prev == NULL && next == NULL) {
        m_Head = NULL;
        m_Tail = NULL;
        return;
    }
    if (next != NULL) {
        next->m_Prev = prev;
        if (prev == NULL) {
            m_Head = next;
        }
    }
    if (prev != NULL) {
        prev->m_Next = voice->m_Next;
        if (voice->m_Next == NULL) {
            m_Tail = prev;
        }
    }
}

// puts the voice before the first one of the same or a lower priority
inline void nn::snd::CTR::VoiceManager::Insert(Voice* voice, int priority)
{
    Voice* v = m_Head;
    if (v == NULL) {
        voice->m_Prev = NULL;
        m_Head = voice;
        voice->m_Next = NULL;
        m_Tail = voice;
        return;
    }
    for (;;) {
        if (v->m_Priority <= priority) {
            voice->m_Prev = v->m_Prev;
            voice->m_Next = v;
            if (v->m_Prev != NULL) {
                v->m_Prev->m_Next = voice;
            } else {
                voice->m_Prev = NULL;
                m_Head = voice;
            }
            v->m_Prev = voice;
            return;
        }
        if (v->m_Next == NULL) {
            break;
        }
        v = v->m_Next;
    }
    v->m_Next = voice;
    voice->m_Prev = v;
    voice->m_Next = NULL;
    m_Tail = voice;
}

// 0x00461500 (name is ours)
Voice* nn::snd::CTR::VoiceManager::AllocVoice(int priority, VoiceDropCallback callback, uptr arg)
{
    if (static_cast<u32>(priority) >= 0x8000) {
        return NULL;
    }
    m_Lock.Enter();
    if (nn::math::CountOneBits(m_UsedMask) == VOICE_COUNT) {
        // drop the voice of the lowest priority if it is lower
        Voice* tail = m_Tail;
        if (tail->m_Priority == Voice::PRIORITY_MAX || tail->m_Priority > priority) {
            m_Lock.Exit();
            return NULL;
        }
        VoiceDropCallback dropCallback = tail->m_DropCallback;
        uptr dropCallbackArg = tail->m_DropCallbackArg;
        FreeVoice(tail);
        if (dropCallback != NULL) {
            dropCallback(tail, dropCallbackArg);
        }
    }
    // the first free voice
    const u32 mask = m_UsedMask;
    const s32 index = 31 - __builtin_clz((mask + 1) & ~mask);
    Voice* voice = NULL;
    if (index != -1 && index < VOICE_COUNT) {
        m_UsedMask = mask | (1 << index);
        m_UsedCount++;
        voice = m_VoicePointers[index];
        voice->Initialize();
    }
    voice->m_Priority = priority;
    Insert(voice, priority);
    m_Lock.Exit();
    voice->m_Impl->SetState(Voice::STATE_PAUSE);
    voice->m_Impl->m_SyncCount++;
    voice->m_DropCallback = callback;
    voice->m_DropCallbackArg = arg;
    return voice;
}

// 0x00461690 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceManager::SetPriority(Voice* voice, int priority)
{
    m_Lock.Enter();
    Remove(voice);
    Insert(voice, priority);
    m_Lock.Exit();
}

// 0x0046176C (name is ours)
void nn::snd::CTR::VoiceManager::UpdateDspParams()
{
    for (s32 i = 0; i < VOICE_COUNT; i++) {
        m_VoicePointers[i]->m_Impl->UpdateDspParams();
    }
}

// 0x004617A0 (name is ours)
void nn::snd::CTR::VoiceManager::ForceUpdateDspParams()
{
    for (s32 i = 0; i < VOICE_COUNT; i++) {
        m_VoicePointers[i]->m_Impl->ForceUpdateDspParams();
    }
}

// 0x004617D4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceManager::AdjustVoicePlayState(int cycles, int usedCycles)
{
    if (m_Mode == 1) {
        const s32 over = usedCycles - 0x9FFC4;
        if (over > 0) {
            cycles -= over;
        }
    }
    m_Lock.Enter();
    // the voices of the higher priority play, the others are dropped when the cycles run out
    for (Voice* voice = m_Head; voice != NULL; voice = voice->m_Next) {
        VoiceImpl* impl = voice->m_Impl;
        if (impl->m_State != Voice::STATE_PLAY || impl->m_WaveBuffers == NULL) {
            continue;
        }
        const s32 voiceCycles = impl->m_DspCycles;
        if (voiceCycles <= cycles || voice->m_Priority == Voice::PRIORITY_MAX) {
            voice->m_Impl->SetSyncCount();
            if (!voice->m_Impl->m_IsPlaying) {
                voice->m_Impl->Start();
            }
            cycles -= voiceCycles;
        } else {
            FreeVoice(voice);
            if (voice->m_DropCallback != NULL) {
                voice->m_DropCallback(voice, voice->m_DropCallbackArg);
            }
        }
    }
    m_Lock.Exit();
}

// 0x0046194C (name is ours)
void nn::snd::CTR::VoiceManager::UpdateWaveBufferLists()
{
    for (s32 i = 0; i < VOICE_COUNT; i++) {
        m_VoicePointers[i]->m_Impl->UpdateWaveBufferList();
    }
}

// 0x00461980 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::VoiceManager::Finalize()
{
    m_Lock.Finalize();
}

// 0x0046198C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::VoiceManager::FreeVoice(Voice* voice)
{
    if (!(m_UsedMask & (1 << voice->m_Id))) {
        return;
    }
    if (voice->m_State != Voice::STATE_STOP) {
        voice->SetState(Voice::STATE_STOP);
    }
    m_Lock.Enter();
    Remove(voice);
    m_UsedMask &= ~(1 << voice->m_Id);
    m_UsedCount--;
    m_Lock.Exit();
}

// 0x00461A44 (name is ours)
nn::snd::CTR::VoiceManager::~VoiceManager()
{
}

// 0x004668CC (name is ours)
void nn::snd::CTR::VoiceManager::UpdateStatus(int channel, const void* status)
{
    m_VoicePointers[channel]->m_Impl->UpdateStatus(status);
}

} // namespace CTR
} // namespace snd
} // namespace nn
