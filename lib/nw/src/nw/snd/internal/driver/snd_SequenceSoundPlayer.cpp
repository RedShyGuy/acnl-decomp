#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"

#include "nw/snd/internal/driver/snd_DisposeCallbackManager.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/internal/snd_SequenceSoundFileReader.h"
#include "nw/snd/internal/snd_TaskManager.h"
#include "nw/snd/snd_SoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the global variables of all sequences
// 0x00AF0CD8
s16 s_GlobalVariable[SequenceSoundPlayer::VARIABLE_COUNT];
// the most ticks a skip parses in one frame
// 0x0097F67C
int s_SkipIntervalTick = 768;

// the tick time in 1/2^28 ms: one tick and one sound frame (names are ours)
const f32 TICK_TIME_UNIT = 268111856.0f;
const u64 FRAME_TIME = 1310720000;
const f32 TICK_TIME_TO_FRACTION = 1.0f / 268435456.0f;
const f32 TICKS_PER_MSEC_PER_TEMPO = 1.0f / 60000.0f;
// the release of the notes of a skip
const int SKIP_RELEASE = 127;
} // namespace

// 0x004D26C0 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::SequenceSoundPlayer::Initialize()
{
    BasicSoundPlayer::Initialize();
    m_IsStarted = false;
    m_IsPause = false;
    m_IsReleasePriorityFix = false;
    m_IsActive = false;
    m_TempoRatio = 1.0f;
    m_TickFraction = 0.0f;
    m_SkipTickCounter = 0;
    m_SkipTimeCounter = 0.0f;
    m_PanRange = 1.0f;
    m_TickCounter = 0;
    m_SeqUserprocCallback = nullptr;
    m_pSeqUserprocCallbackArg = nullptr;
    m_Tempo = 120;
    m_Timebase = 48;
    m_SequenceVolume.InitValue(127);
    m_Priority = 64;
    m_pNoteOnCallback = nullptr;
    for (int i = 0; i < VARIABLE_COUNT; ++i) {
        m_LocalVariable[i] = -1;
    }
    for (int i = 0; i < TRACK_COUNT; ++i) {
        m_pTracks[i] = nullptr;
    }
    m_IsRegisteredPlayerCallback = false;
    m_IsInitialized = true;
}

// 0x004D279C | mk7dlp:callseq [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::UpdateTick()
{
    f32 tickPerMsec = static_cast<f32>(static_cast<u32>(m_Timebase * m_Tempo)) * m_TempoRatio * TICKS_PER_MSEC_PER_TEMPO;
    if (tickPerMsec == 0.0f) {
        return;
    }
    u64 tickTime = static_cast<u64>(m_TickFraction * TICK_TIME_UNIT / tickPerMsec);
    u64 restTime = FRAME_TIME;
    while (tickTime < restTime) {
        restTime -= tickTime;

        // one tick of all tracks
        m_SequenceVolume.Update();
        bool isActive = false;
        for (int i = 0; i < TRACK_COUNT; ++i) {
            SequenceTrack* pTrack = GetPlayerTrack(i);
            if (pTrack == nullptr) {
                continue;
            }
            pTrack->UpdateChannelLength();
            if (pTrack->ParseNextTick(true) < 0) {
                CloseTrack(i);
            }
            if (pTrack->m_OpenFlag) {
                isActive = true;
            }
        }
        if (!isActive) {
            FinishPlayer();
            m_IsFinished = true;
            return;
        }
        ++m_TickCounter;

        tickPerMsec = static_cast<f32>(static_cast<u32>(m_Timebase * m_Tempo)) * TICKS_PER_MSEC_PER_TEMPO * m_TempoRatio;
        if (tickPerMsec == 0.0f) {
            return;
        }
        tickTime = static_cast<u64>(TICK_TIME_UNIT / tickPerMsec);
    }
    // the rest of the tick for the next frame
    m_TickFraction = static_cast<f32>(tickTime - restTime) * tickPerMsec * TICK_TIME_TO_FRACTION;
}

// 0x004D2974 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::RequestLoad(const StartInfo& startInfo, const SequenceSoundLoader::Arg& arg)
{
    m_StartInfo = startInfo;
    m_LoaderArg = arg;
    m_ResState = RES_STATE_LOAD_REQUESTED;
    SoundThread::GetInstance().RegisterPlayerCallback(this);
    m_IsRegisteredPlayerCallback = true;
}

// 0x004D29C8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPan(unsigned trackBitFlag, f32 pan)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetPan, pan);
}

// 0x004D29E0 slot 0x14
void nw::snd::internal::driver::SequenceSoundPlayer::Stop()
{
    FinishPlayer();
}

// 0x004D29E4 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::FinishPlayer()
{
    if (m_IsRegisteredPlayerCallback) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        m_IsRegisteredPlayerCallback = false;
    }
    if (m_IsStarted) {
        m_IsStarted = false;
    }
    for (int i = 0; i < TRACK_COUNT; ++i) {
        CloseTrack(i);
    }
}

// 0x004D2A60 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackMute(unsigned trackBitFlag, SeqMute mute)
{
    SetTrackParam<SeqMute>(trackBitFlag, &SequenceTrack::SetMute, mute);
}

// 0x004D2AE4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPitch(unsigned trackBitFlag, f32 pitch)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetPitch, pitch);
}

// 0x004D2AFC | nintendogs:callseq-callee [tier A]
nw::snd::internal::driver::SequenceTrack* nw::snd::internal::driver::SequenceSoundPlayer::GetPlayerTrack(int trackNo)
{
    if (trackNo > TRACK_COUNT - 1) {
        return nullptr;
    }
    return m_pTracks[trackNo];
}

// 0x004D2B10 | nintendogs:callseq-callee [tier A]
s16* nw::snd::internal::driver::SequenceSoundPlayer::GetVariablePtr(int index)
{
    if (index < VARIABLE_COUNT) {
        return &m_LocalVariable[index];
    }
    if (index < VARIABLE_COUNT * 2) {
        return &s_GlobalVariable[index - VARIABLE_COUNT];
    }
    return nullptr;
}

// 0x004D2B3C slot 0x1C
// 0x007789B8 (thunk)
void nw::snd::internal::driver::SequenceSoundPlayer::InvalidateData(const void* pStart, const void* pEnd)
{
    if (!m_IsActive) {
        return;
    }
    for (int i = 0; i < TRACK_COUNT; ++i) {
        SequenceTrack* pTrack = GetPlayerTrack(i);
        if (pTrack == nullptr) {
            continue;
        }
        const void* pSeqBase = pTrack->m_pSeqBase;
        if (pStart <= pSeqBase && pSeqBase <= pEnd) {
            Finalize();
            break;
        }
    }
    for (int i = 0; i < BANK_COUNT; ++i) {
        const void* pBankFile = m_BankReaders[i].m_pHeader;
        if (pStart <= pBankFile && pBankFile <= pEnd) {
            m_BankReaders[i].Finalize();
        }
    }
}

// 0x004D2BDC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackVolume(unsigned trackBitFlag, f32 volume)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetVolume, volume);
}

// 0x004D2BF4 slot 0x20
void nw::snd::internal::driver::SequenceSoundPlayer::ChannelCallback(Channel*)
{
    // empty (in the original too)
}

// 0x004D2BF8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackLpfFreq(unsigned trackBitFlag, f32 lpfFreq)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetLpfFreq, lpfFreq);
}

// 0x004D2C10 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackSilence(unsigned long trackBitFlag, bool isSilence, int fadeTimes)
{
    for (int i = 0; i < TRACK_COUNT; ++i, trackBitFlag >>= 1) {
        if (trackBitFlag == 0) {
            break;
        }
        if (trackBitFlag & 1) {
            SequenceTrack* pTrack = GetPlayerTrack(i);
            if (pTrack != nullptr) {
                pTrack->SetSilence(isSilence, fadeTimes);
            }
        }
    }
}

// 0x004D2C98 (name is ours)
void nw::snd::internal::driver::SequenceSoundPlayer::SetLocalVariable(int index, s16 value)
{
    m_LocalVariable[index] = value;
}

// 0x00741680 (name is ours)
s16 nw::snd::internal::driver::SequenceSoundPlayer::GetLocalVariable(int index) const
{
    return m_LocalVariable[index];
}

// 0x004D2CA4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPanRange(unsigned trackBitFlag, f32 panRange)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetPanRange, panRange);
}

// 0x004D2CBC (name is ours)
s16 nw::snd::internal::driver::SequenceSoundPlayer::GetGlobalVariable(int index)
{
    return s_GlobalVariable[index];
}

// 0x004D2CD0 (name is ours)
void nw::snd::internal::driver::SequenceSoundPlayer::SetGlobalVariable(int index, s16 value)
{
    s_GlobalVariable[index] = value;
}

// 0x004D2CE4 | fefates:bytes [tier B]
bool nw::snd::internal::driver::SequenceSoundPlayer::SetTrackBankIndex(unsigned int trackBitFlag, int bankIndex)
{
    if (!m_BankReaders[bankIndex].m_IsInitialized) {
        return false;
    }
    SetTrackParam<int>(trackBitFlag, &SequenceTrack::SetBankIndex, bankIndex);
    return true;
}

// 0x004D2D80 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackTranspose(unsigned int trackBitFlag, signed char transpose)
{
    SetTrackParam<s8>(trackBitFlag, &SequenceTrack::SetTranspose, transpose);
}

// 0x004D2E04 (name is ours)
void nw::snd::internal::driver::SequenceSoundPlayer::SetSkipIntervalTickEntry(int tick)
{
    SetSkipIntervalTick(tick);
}

// 0x004D2E08 (name is ours)
void nw::snd::internal::driver::SequenceSoundPlayer::SetSkipIntervalTick(int tick)
{
    s_SkipIntervalTick = tick;
}

// 0x004D2E18 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackSurroundPan(unsigned trackBitFlag, f32 surroundPan)
{
    SetTrackParam<f32>(trackBitFlag, &SequenceTrack::SetSurroundPan, surroundPan);
}

// 0x004D2E30 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackBiquadFilter(unsigned trackBitFlag, int type, f32 value)
{
    for (int i = 0; i < TRACK_COUNT; ++i, trackBitFlag >>= 1) {
        if (trackBitFlag == 0) {
            break;
        }
        if (trackBitFlag & 1) {
            SequenceTrack* pTrack = GetPlayerTrack(i);
            if (pTrack != nullptr) {
                pTrack->SetBiquadFilter(type, value);
            }
        }
    }
}

// 0x004D2EC0 slot 0x28
// 0x00778BB8 (thunk)
void nw::snd::internal::driver::SequenceSoundPlayer::OnShutdown()
{
    Stop();
}

// 0x004D2ECC | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackVelocityRange(unsigned int trackBitFlag, unsigned char velocityRange)
{
    SetTrackParam<u8>(trackBitFlag, &SequenceTrack::SetVelocityRange, velocityRange);
}

// 0x004D2F50 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::InitSequenceSoundPlayer()
{
    for (int i = 0; i < VARIABLE_COUNT; ++i) {
        s_GlobalVariable[i] = -1;
    }
}

// 0x004D2F7C (name is ours)
void nw::snd::internal::driver::SequenceSoundPlayer::SetSeqUserprocCallback(SequenceUserprocCallback callback, void* pArg)
{
    m_SeqUserprocCallback = callback;
    m_pSeqUserprocCallbackArg = pArg;
}

// 0x004D2F88 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::CallSequenceUserprocCallback(unsigned short procId, SequenceTrack* pTrack)
{
    if (m_SeqUserprocCallback == nullptr) {
        return;
    }
    SequenceUserprocCallbackParam param;
    param.localVariable = m_LocalVariable;
    param.globalVariable = s_GlobalVariable;
    param.trackVariable = pTrack->GetVariablePtr(0);
    param.cmpFlag = pTrack->m_CmpFlag;
    m_SeqUserprocCallback(procId, &param, m_pSeqUserprocCallbackArg);
    pTrack->m_CmpFlag = param.cmpFlag;
}

// 0x004D2FF8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::Skip(OffsetType offsetType, int offset)
{
    if (!m_IsActive) {
        return;
    }
    switch (offsetType) {
    case OFFSET_TYPE_TICK:
        m_SkipTickCounter += offset;
        break;
    case OFFSET_TYPE_MILLISEC:
        m_SkipTimeCounter += offset;
        break;
    default:
        break;
    }
}

// 0x004D303C slot 0x18 | nintendogs:bytes-fuzzy
void nw::snd::internal::driver::SequenceSoundPlayer::Pause(bool isPause)
{
    m_IsPause = isPause;
    for (int i = 0; i < TRACK_COUNT; ++i) {
        SequenceTrack* pTrack = GetPlayerTrack(i);
        if (pTrack != nullptr) {
            pTrack->PauseAllChannel(isPause);
        }
    }
}

// 0x004D307C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Setup(const SetupArg& arg)
{
    m_pNoteOnCallback = arg.callback;

    // the tracks of the bits
    int trackCount = 0;
    for (u32 bits = arg.allocTracks; bits != 0; bits >>= 1) {
        if (bits & 1) {
            ++trackCount;
        }
    }
    if (trackCount > arg.trackAllocator->GetAllocatableTrackCount()) {
        Finalize();
        return;
    }
    u32 bits = arg.allocTracks;
    for (int i = 0; bits != 0; bits >>= 1, ++i) {
        if (!(bits & 1)) {
            continue;
        }
        SequenceTrack* pTrack = arg.trackAllocator->AllocTrack(this);
        if (i <= TRACK_COUNT - 1) {
            m_pTracks[i] = pTrack;
            pTrack->m_PlayerTrackNo = i;
            pTrack->m_FrontBypassFlag = m_IsFrontBypass;
        }
    }
    m_pTrackAllocator = arg.trackAllocator;
}

// 0x004D3148 slot 0x10
void nw::snd::internal::driver::SequenceSoundPlayer::Start()
{
    m_IsStarted = true;
}

// 0x004D3154 | fefates:bytes [tier B]
nw::snd::internal::driver::Channel* nw::snd::internal::driver::SequenceSoundPlayer::NoteOn(unsigned char bankIndex, const NoteOnInfo& info)
{
    return m_pNoteOnCallback->NoteOn(this, bankIndex, info);
}

// 0x004D3184 slot 0x24
// 0x00778BC4 (thunk)
void nw::snd::internal::driver::SequenceSoundPlayer::OnUpdateFrame()
{
    Update();
}

// 0x004D3188 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Update()
{
    switch (m_ResState) {
    case RES_STATE_LOAD_REQUESTED: {
        if (m_pLoaderManager == nullptr) {
            return;
        }
        SequenceSoundLoader* pLoader = m_pLoaderManager->Alloc();
        if (pLoader == nullptr) {
            return;
        }
        m_pLoader = pLoader;
        m_ResState = RES_STATE_LOADING;
        pLoader->Initialize(m_LoaderArg);
    }
        // fall through
    case RES_STATE_LOADING: {
        SequenceSoundLoader::DataLoadTask& task = m_pLoader->m_Task;
        if (task.m_pPlayerHeap == nullptr) {
            task.m_pPlayerHeap = task.m_Arg.soundPlayer->detail_AllocPlayerHeap();
            if (task.m_pPlayerHeap == nullptr) {
                return;
            }
            task.m_pPlayerHeap->Clear();
        }
        if (task.m_Status == Task::STATUS_FREE) {
            TaskManager::GetInstance().AppendTask(&task, TaskManager::PRIORITY_MIDDLE);
            return;
        }
        if (task.m_Status != Task::STATUS_DONE && task.m_Status != Task::STATUS_CANCEL) {
            return;
        }
        if (!m_pLoader->m_Task.m_IsSucceeded) {
            m_IsFinished = true;
            FinishPlayer();
            return;
        }

        // prepared from the player heap
        PrepareArg arg;
        arg.seqFile = m_pLoader->m_Task.m_Result.seqFile;
        arg.seqOffset = m_StartInfo.seqOffset;
        for (int i = 0; i < BANK_COUNT; ++i) {
            arg.bankFiles[i] = m_pLoader->m_Task.m_Result.bankFiles[i];
            arg.warcFiles[i] = m_pLoader->m_Task.m_Result.warcFiles[i];
            arg.warcIsIndividual[i] = m_pLoader->m_Task.m_Result.warcIsIndividual[i];
        }
        if (m_IsActive) {
            FinishPlayer();
        }
        SequenceTrack* pTrack = m_pTracks[0];
        if (pTrack == nullptr) {
            Finalize();
        } else {
            SequenceSoundFileReader reader(arg.seqFile);
            pTrack->SetSeqData(reader.m_pDataBlockBody, arg.seqOffset);
            pTrack->Open();
            for (int i = 0; i < BANK_COUNT; ++i) {
                m_BankReaders[i].Initialize(arg.bankFiles[i]);
                m_WarcReaders[i].Initialize(arg.warcFiles[i], arg.warcIsIndividual[i]);
            }
            m_ResState = RES_STATE_PREPARED;
            m_IsActive = true;
            DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
            m_IsRegisteredPlayerCallback = true;
        }
        if (!m_IsActive) {
            return;
        }
        // the start offset
        switch (m_StartInfo.startOffsetType) {
        case OFFSET_TYPE_TICK:
            m_SkipTickCounter += m_StartInfo.startOffset;
            break;
        case OFFSET_TYPE_MILLISEC:
            m_SkipTimeCounter += m_StartInfo.startOffset;
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    if (!m_IsActive || !m_IsStarted) {
        return;
    }
    if (m_SkipTickCounter != 0 || m_SkipTimeCounter > 0.0f) {
        SkipTick();
    } else if (!m_IsPause) {
        UpdateTick();
    }
    for (int i = 0; i < TRACK_COUNT; ++i) {
        SequenceTrack* pTrack = GetPlayerTrack(i);
        if (pTrack != nullptr) {
            pTrack->UpdateChannelParam();
        }
    }
}

// 0x004D3520 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Prepare(const PrepareArg& arg)
{
    if (m_IsActive) {
        FinishPlayer();
    }
    SequenceTrack* pTrack = m_pTracks[0];
    if (pTrack == nullptr) {
        Finalize();
        return;
    }
    SequenceSoundFileReader reader(arg.seqFile);
    pTrack->SetSeqData(reader.m_pDataBlockBody, arg.seqOffset);
    pTrack->Open();
    for (int i = 0; i < BANK_COUNT; ++i) {
        m_BankReaders[i].Initialize(arg.bankFiles[i]);
        m_WarcReaders[i].Initialize(arg.warcFiles[i], arg.warcIsIndividual[i]);
    }
    m_ResState = RES_STATE_PREPARED;
    m_IsActive = true;
    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
    SoundThread::GetInstance().RegisterPlayerCallback(this);
    m_IsRegisteredPlayerCallback = true;
}

// 0x004D3614 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::SequenceSoundPlayer::Finalize()
{
    m_IsFinished = true;
    FinishPlayer();
    if (m_IsActive) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
        m_IsActive = false;
    }
    for (int i = 0; i < BANK_COUNT; ++i) {
        m_BankReaders[i].Finalize();
        m_WarcReaders[i].Finalize();
    }
    if (m_IsInitialized) {
        BasicSoundPlayer::Finalize();
        m_IsInitialized = false;
    }
    if (m_pLoader != nullptr) {
        m_pLoaderManager->Free(m_pLoader);
        m_pLoader = nullptr;
    }
}

// 0x004D36F8 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SkipTick()
{
    // the playing notes end
    for (int i = 0; i < TRACK_COUNT; ++i) {
        SequenceTrack* pTrack = GetPlayerTrack(i);
        if (pTrack != nullptr) {
            pTrack->ReleaseAllChannel(SKIP_RELEASE);
            pTrack->FreeAllChannel();
        }
    }

    for (int count = 0;;) {
        if (m_SkipTickCounter == 0) {
            const f32 tickPerMsec = static_cast<f32>(static_cast<u32>(m_Timebase * m_Tempo)) * m_TempoRatio * TICKS_PER_MSEC_PER_TEMPO;
            if (tickPerMsec * m_SkipTimeCounter < 1.0f) {
                m_SkipTimeCounter = 0.0f;
                return;
            }
        }
        if (count >= s_SkipIntervalTick) {
            return;
        }
        if (m_SkipTickCounter != 0) {
            --m_SkipTickCounter;
        } else {
            const f32 tickPerMsec = static_cast<f32>(static_cast<u32>(m_Timebase * m_Tempo)) * TICKS_PER_MSEC_PER_TEMPO * m_TempoRatio;
            m_SkipTimeCounter -= 1.0f / tickPerMsec;
        }

        // one tick of all tracks without notes
        m_SequenceVolume.Update();
        bool isActive = false;
        for (int i = 0; i < TRACK_COUNT; ++i) {
            SequenceTrack* pTrack = GetPlayerTrack(i);
            if (pTrack == nullptr) {
                continue;
            }
            pTrack->UpdateChannelLength();
            if (pTrack->ParseNextTick(false) < 0) {
                CloseTrack(i);
            }
            if (pTrack->m_OpenFlag) {
                isActive = true;
            }
        }
        if (!isActive) {
            FinishPlayer();
            m_IsFinished = true;
            return;
        }
        ++m_TickCounter;
        ++count;
    }
}

// 0x004D38E8 | fefates:bytes [tier B]
nw::snd::internal::driver::SequenceSoundPlayer::SequenceSoundPlayer()
    : m_IsInitialized(false), m_IsRegisteredPlayerCallback(false), m_pLoaderManager(nullptr), m_pLoader(nullptr)
{
    m_SequenceVolume.InitValue(127);
    for (int i = 0; i < TRACK_COUNT; ++i) {
        m_pTracks[i] = nullptr;
    }
}

// 0x004D3A20 slot 0x00 | fefates:bytes
// 0x004D39DC (deleting dtor)
// 0x00778A9C (thunk 68)
// 0x00778A58 (deleting thunk 68)
// 0x00778C10 (thunk 80)
// 0x00778BCC (deleting thunk 80)
nw::snd::internal::driver::SequenceSoundPlayer::~SequenceSoundPlayer()
{
    SequenceSoundPlayer::Finalize();
}

// 0x007E792C | nintendogs:bytes-fuzzy [confirmed by fefates, mk7dlp] [tier A]
template <>
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackParam<float>(unsigned trackBitFlag, void (SequenceTrack::*pFunc)(float), float param)
{
    for (int i = 0; i < TRACK_COUNT; ++i, trackBitFlag >>= 1) {
        if (trackBitFlag == 0) {
            break;
        }
        if (trackBitFlag & 1) {
            SequenceTrack* pTrack = GetPlayerTrack(i);
            if (pTrack != nullptr) {
                (pTrack->*pFunc)(param);
            }
        }
    }
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
