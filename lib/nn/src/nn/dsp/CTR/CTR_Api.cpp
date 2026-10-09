// dsp_Api.cpp (the name is ours; static initializer at 0x007856A8: s_Lock, s_Session and
// s_SleepAcceptedCallbackInfo)
#include "nn/dsp/CTR/CTR_Api.h"
#include <string.h>
#include "nn/applet/CTR/applet_SysSleepAcceptedCallbackInfo.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/dsp/CTR/dsp_DSP.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/os/os_ReaderWriterLock.h"
#include "nn/os/os_Thread.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace dsp {
namespace CTR {
namespace detail {
// the default component, a DSP program image in .rodata that is not in the source (name is ours)
// 0x009391A4
extern const u8 s_DefaultComponent[];
} // namespace detail

namespace {
// results (module 41; the names are ours)
const bit32 RESULT_NOT_INITIALIZED = 0xC8A0A7F8; // status, invalid state, 1016
const bit32 RESULT_ALREADY_LOADED = 0xC8A0A7FC;  // status, invalid state, 1020

const size_t DEFAULT_COMPONENT_SIZE = 0xC26C;
const u16 DEFAULT_PROGRAM_MASK = 0xFF;
const u16 DEFAULT_DATA_MASK = 0xFF;

const s32 CALLBACK_COUNT = 8;

// how long to wait between tries to lock the component
const s64 LOCK_WAIT_MSEC = 1;

// the priority of the sleep callback among the ones of the applet library
const s32 SLEEP_ACCEPTED_PRIORITY = 0x8000;

// 0x008B3F3C
const nn::Handle CURRENT_PROCESS(nn::PSEUDO_HANDLE_CURRENT_PROCESS);
// 0x008B3F40
const nn::Handle INVALID_HANDLE;

const char SERVICE_NAME[] = "dsp::DSP";

// set by the sleep callback (the system went to sleep), cleared by Awake / WakeUp
// 0x0097E894
bool s_IsSleptBySystem;
// 0x0097E895
bool s_IsLoaded;
// the component was unloaded by Sleep
// 0x0097E896
bool s_IsSleeping;
// what ForceHeadphoneOut is given after waking up (set by LoadDefaultComponent and UnloadComponent)
// 0x0097E897
bool s_IsHeadphoneOutForced;
// the masks and the component that is loaded again after waking up
// 0x0097E898
u16 s_ProgramMask;
// 0x0097E89A
u16 s_DataMask;
// the DSP object, 0 while not initialized
// 0x0097E89C
nn::dsp::CTR::DSP* s_pDsp;
// a bit for each registered interrupt (1 << (interrupt + channel))
// 0x0097E8A0
bit32 s_RegisteredInterrupts;
// readers use the component, a writer loads / unloads it
// 0x0097E8A4
nn::os::ReaderWriterLock s_Lock;
// 0x0097E8A8
nn::dsp::CTR::DSP s_Dsp;
// 0x0097E8AC
nn::Handle s_Session;
// 0x0097E8B0
const u8* s_pComponent;
// 0x0097E8B4
size_t s_ComponentSize;

// 0x00AE9560
SleepWakeUpCallback s_SleepCallbacks[CALLBACK_COUNT];
// 0x00AE9580
SleepWakeUpCallback s_WakeUpCallbacks[CALLBACK_COUNT];
// 0x00AE95A0
SleepWakeUpCallback s_WaitForFinalizeCallbacks[CALLBACK_COUNT];

void SleepAcceptedCallback(uptr argument);

// 0x00AE95C0
nn::applet::CTR::SysSleepAcceptedCallbackInfo s_SleepAcceptedCallbackInfo(SleepAcceptedCallback, 0, SLEEP_ACCEPTED_PRIORITY);

// takes the component for loading / unloading
inline void LockForWrite()
{
    while (!s_Lock.TryLockForWrite()) {
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(LOCK_WAIT_MSEC));
    }
}

inline void CallCallbacks(SleepWakeUpCallback* callbacks)
{
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        if (callbacks[i] != 0) {
            callbacks[i]();
        }
    }
}

// unloads the component for sleeping; false if there is nothing to do
inline bool SleepImpl()
{
    if (!(s_IsLoaded && !s_IsSleeping)) {
        return false;
    }
    CallCallbacks(s_SleepCallbacks);
    if (s_IsLoaded) {
        LockForWrite();
        s_pDsp->UnloadComponent();
        s_IsLoaded = false;
        s_Lock.UnlockForWrite();
    }
    s_IsSleeping = true;
    return true;
}

// loads the component again after Sleep
inline void WakeUpImpl()
{
    if (!s_IsSleeping) {
        return;
    }
    nn::Result result = LoadComponent(s_pComponent, s_ComponentSize, s_ProgramMask, s_DataMask);
    uptr address = nn::err::CTR::GetCurrentAddress();
    if (result.IsFailure()) {
        nn::err::CTR::ThrowFatalErr(result, address);
    }
    CallCallbacks(s_WakeUpCallbacks);
    s_IsSleeping = false;
    if (s_pDsp != 0) {
        s_pDsp->ForceHeadphoneOut(s_IsHeadphoneOutForced);
    }
}

// the callback of the applet library when the system goes to sleep (name is ours)
// 0x0035173C (name is ours)
void SleepAcceptedCallback(uptr)
{
    s_IsSleptBySystem = SleepImpl();
}
} // namespace

// 0x001245B0 | nintendogs:bytes [tier A]
void OrderToWaitForFinalize()
{
    if (s_IsSleeping) {
        CallCallbacks(s_WaitForFinalizeCallbacks);
        s_IsSleeping = false;
    }
}

// 0x00129D38 | fefates:callgraph [tier C]
void Awake()
{
    if (s_IsSleptBySystem) {
        s_IsSleptBySystem = false;
        WakeUpImpl();
    }
}

// 0x00130A60 | nintendogs:callgraph [tier A]
bool IsComponentLoaded()
{
    return s_IsLoaded;
}

// 0x00130A70 (name is ours)
nn::Result LoadComponent(const u8* component, size_t size, u16 programMask, u16 dataMask)
{
    if (s_pDsp == 0 || s_IsLoaded) {
        return RESULT_ALREADY_LOADED;
    }
    LockForWrite();
    nn::Result result = s_pDsp->LoadComponent(component, size, programMask, dataMask, &s_IsLoaded);
    s_Lock.UnlockForWrite();
    return result;
}

// 0x00130B60 | nintendogs:callgraph [tier A]
void Sleep()
{
    SleepImpl();
}

// 0x00130C04 | fefates:callgraph [tier C]
void WakeUp()
{
    s_IsSleptBySystem = false;
    WakeUpImpl();
}

// 0x001409EC | nintendogs:bytes [tier A]
nn::Result FlushDataCache(unsigned address, unsigned size)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->FlushDataCache(CURRENT_PROCESS, address, size);
}

// 0x00351280 | nintendogs:callgraph [tier A]
nn::Result Initialize()
{
    if (s_pDsp != 0) {
        return nn::Result();
    }
    nn::Result result = nn::srv::Initialize();
    if (result.IsFailure()) {
        return result;
    }
    result = nn::srv::GetServiceHandle(&s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
    if (result.IsFailure()) {
        return result;
    }
    s_pDsp = &s_Dsp;
    s_Dsp.m_Session = s_Session;
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        s_WaitForFinalizeCallbacks[i] = 0;
        s_WakeUpCallbacks[i] = 0;
        s_SleepCallbacks[i] = 0;
    }
    s_IsLoaded = false;
    s_pComponent = 0;
    s_ComponentSize = 0;
    s_ProgramMask = 0;
    s_DataMask = 0;
    s_IsSleeping = false;
    s_SleepAcceptedCallbackInfo.Register();
    s_Lock.Initialize();
    return nn::Result();
}

// 0x0035135C | fefates:callgraph [tier C]
nn::Result SetSemaphore(u16 value)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->SetSemaphore(value);
}

// 0x00351388 | fefates:callgraph [tier C]
void LockComponent()
{
    s_Lock.LockForRead();
}

// 0x00351394 (name is ours)
nn::Result RecvDataIsReady(u16 registerNumber, bool* pIsReady)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->RecvDataIsReady(registerNumber, pIsReady);
}

// 0x003513C8 (name is ours)
nn::Result UnloadComponent()
{
    // the users must be gone
    if (s_RegisteredInterrupts != 0) {
        nndbgPanic();
    }
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        if (s_SleepCallbacks[i] != 0) {
            nndbgPanic();
        }
    }
    nn::Result result;
    if (s_IsLoaded) {
        LockForWrite();
        result = s_pDsp->UnloadComponent();
        s_IsLoaded = false;
        s_Lock.UnlockForWrite();
    }
    s_IsHeadphoneOutForced = true;
    return result;
}

// 0x00351474 | fefates:callgraph [tier C]
void UnlockComponent()
{
    s_Lock.UnlockForRead();
}

// 0x00351480 | nintendogs:bytes [tier A]
nn::Result WriteProcessPipe(int channel, const void* buffer, unsigned size)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->WriteProcessPipe(channel, static_cast<const u8*>(buffer), size);
}

// 0x003514C8 | nintendogs:bytes [tier A]
nn::Result ReadPipeIfPossible(int channel, void* buffer, unsigned short size, unsigned short* pReadSize)
{
    if (s_pDsp == 0) {
        *pReadSize = 0;
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->ReadPipeIfPossible(channel, 0, static_cast<u8*>(buffer), size, pReadSize);
}

// 0x0035151C | nintendogs:callgraph [tier C]
nn::Result LoadDefaultComponent()
{
    s_pComponent = detail::s_DefaultComponent;
    s_ComponentSize = DEFAULT_COMPONENT_SIZE;
    s_ProgramMask = DEFAULT_PROGRAM_MASK;
    s_DataMask = DEFAULT_DATA_MASK;
    nn::Result result = LoadComponent(s_pComponent, s_ComponentSize, s_ProgramMask, s_DataMask);
    if (result.IsSuccess()) {
        s_IsHeadphoneOutForced = true;
    }
    return result;
}

// 0x00351564 (name is ours)
nn::Result SetSemaphoreMask(u16 mask)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->SetSemaphoreMask(mask);
}

// 0x00351590 (name is ours)
nn::Result GetSemaphoreEventHandle(nn::Handle* pEvent)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->GetSemaphoreEventHandle(pEvent);
}

// 0x003515BC | nintendogs:bytes [tier A]
nn::Result RegisterInterruptEvents(nn::Handle event, int interrupt, int channel)
{
    nn::Result result = RESULT_NOT_INITIALIZED;
    if (s_pDsp == 0) {
        return result;
    }
    bit32 bit = 1 << (interrupt + channel);
    if (event.IsValid()) {
        // (registering twice gives the result above)
        if ((s_RegisteredInterrupts & bit) == 0) {
            result = s_pDsp->RegisterInterruptEvents(event, interrupt, channel);
            s_RegisteredInterrupts |= bit;
        }
    } else {
        if ((s_RegisteredInterrupts & bit) != 0) {
            result = s_pDsp->RegisterInterruptEvents(event, interrupt, channel);
            s_RegisteredInterrupts &= ~bit;
        }
    }
    return result;
}

// 0x00351654 | nintendogs:bytes [tier A]
bool ClearSleepWakeUpCallback(SleepWakeUpCallback sleep, SleepWakeUpCallback, SleepWakeUpCallback)
{
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        if (s_SleepCallbacks[i] == sleep) {
            s_SleepCallbacks[i] = 0;
            s_WakeUpCallbacks[i] = 0;
            s_WaitForFinalizeCallbacks[i] = 0;
            return true;
        }
    }
    return false;
}

// 0x003516A4 | nintendogs:bytes [tier A]
bool RegisterSleepWakeUpCallback(SleepWakeUpCallback sleep, SleepWakeUpCallback wakeUp, SleepWakeUpCallback waitForFinalize)
{
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        if (s_SleepCallbacks[i] == 0) {
            s_SleepCallbacks[i] = sleep;
            s_WakeUpCallbacks[i] = wakeUp;
            s_WaitForFinalizeCallbacks[i] = waitForFinalize;
            return true;
        }
    }
    return false;
}

// 0x003516FC | fefates:bytes [tier B]
nn::Result ConvertProcessAddressFromDspDram(unsigned int address, unsigned int* pAddress)
{
    *pAddress = 0xFFFFFFFF;
    if (s_pDsp != 0) {
        // (the original drops the result of the command and always returns this one)
        s_pDsp->ConvertProcessAddressFromDspDram(address, pAddress);
    }
    return RESULT_NOT_INITIALIZED;
}

// 0x00351AE0 (name is ours)
nn::Result RecvData(u16 registerNumber, u16* pData)
{
    if (s_pDsp == 0) {
        return RESULT_NOT_INITIALIZED;
    }
    return s_pDsp->RecvData(registerNumber, pData);
}

// 0x00351A50 (name is ours)
void Finalize()
{
    if (s_pDsp == 0) {
        return;
    }
    if (s_RegisteredInterrupts != 0) {
        nndbgPanic();
    }
    for (s32 i = 0; i < CALLBACK_COUNT; i++) {
        if (s_SleepCallbacks[i] != 0) {
            nndbgPanic();
        }
    }
    s_SleepAcceptedCallbackInfo.Unregister();
    nn::svc::CloseHandle(s_Session);
    s_pDsp = 0;
    s_Session = INVALID_HANDLE;
    s_Lock.Finalize();
}

} // namespace CTR
} // namespace dsp
} // namespace nn
