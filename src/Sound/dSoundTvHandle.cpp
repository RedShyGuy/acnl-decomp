#include "Sound/dSoundTvBase.h"
#include "Sound/sound_Extern.h"
#include "Sound/dSoundTv00Storm.h"
#include "Sound/dSoundTv01Color.h"
#include "Sound/dSoundTv02News.h"
#include "Sound/dSoundTv03Drama.h"
#include "Sound/dSoundTv04Song.h"
#include "Sound/dSoundTv05Quiz.h"
#include "Sound/dSoundTv06Travel.h"
#include "Sound/dSoundTv07Hero.h"
#include "Sound/dSoundTv08Variety.h"
#include "Sound/dSoundTv09Movie.h"
#include "Sound/dSoundTv10Soccer.h"
#include "Sound/dSoundTv11Talk.h"
#include "Sound/dSoundTv12Anime.h"
#include "Sound/dSoundTv13Cooking.h"
#include "Sound/dSoundTv14Exercize.h"
#include "Sound/dSoundTv15AppleCM.h"
#include "Sound/dSoundTv16SingCM.h"
#include "Sound/dSoundTv17SnowCM.h"
#include "Sound/dSoundTv18UfoCM.h"
#include "Sound/dSoundTv19ShoppingCM.h"
#include "Sound/dSoundTv20JuiceCM.h"
#include "Sound/dSoundTv21LaundryCM.h"
#include "Sound/dSoundTv255Weather.h"
#include "sead/seadNew.h"

namespace {
// the factory of each program
// 0x0084631C
SoundTvHandle* (* const s_TvCreateFuncs[])(const SoundTvHandle::CreateArg*) = {
    &SoundTvHandle::Create<SoundTv00Storm>,
    &SoundTvHandle::Create<SoundTv01Color>,
    &SoundTvHandle::Create<SoundTv02News>,
    &SoundTvHandle::Create<SoundTv03Drama>,
    &SoundTvHandle::Create<SoundTv04Song>,
    &SoundTvHandle::Create<SoundTv05Quiz>,
    &SoundTvHandle::Create<SoundTv06Travel>,
    &SoundTvHandle::Create<SoundTv07Hero>,
    &SoundTvHandle::Create<SoundTv08Variety>,
    &SoundTvHandle::Create<SoundTv09Movie>,
    &SoundTvHandle::Create<SoundTv10Soccer>,
    &SoundTvHandle::Create<SoundTv11Talk>,
    &SoundTvHandle::Create<SoundTv12Anime>,
    &SoundTvHandle::Create<SoundTv13Cooking>,
    &SoundTvHandle::Create<SoundTv14Exercize>,
    &SoundTvHandle::Create<SoundTv15AppleCM>,
    &SoundTvHandle::Create<SoundTv16SingCM>,
    &SoundTvHandle::Create<SoundTv17SnowCM>,
    &SoundTvHandle::Create<SoundTv18UfoCM>,
    &SoundTvHandle::Create<SoundTv19ShoppingCM>,
    &SoundTvHandle::Create<SoundTv20JuiceCM>,
    &SoundTvHandle::Create<SoundTv21LaundryCM>,
    &SoundTvHandle::Create<SoundTv255Weather>,
};
} // namespace

template <typename T>
SoundTvHandle* SoundTvHandle::Create(const CreateArg* arg)
{
    SoundTvHandle* handle = new (arg->heap, 4) SoundTvHandle;
    handle->m_pTv = new (arg->heap, 4) T;
    handle->m_IsStarted = false;
    handle->m_IsLoaded = false;
    return handle;
}

// 0x0081FFB4
template SoundTvHandle* SoundTvHandle::Create<SoundTv00Storm>(const SoundTvHandle::CreateArg* arg);
// 0x0082000C
template SoundTvHandle* SoundTvHandle::Create<SoundTv01Color>(const SoundTvHandle::CreateArg* arg);
// 0x0081FDBC
template SoundTvHandle* SoundTvHandle::Create<SoundTv02News>(const SoundTvHandle::CreateArg* arg);
// 0x00820064
template SoundTvHandle* SoundTvHandle::Create<SoundTv03Drama>(const SoundTvHandle::CreateArg* arg);
// 0x0081FE1C
template SoundTvHandle* SoundTvHandle::Create<SoundTv04Song>(const SoundTvHandle::CreateArg* arg);
// 0x0081FE80
template SoundTvHandle* SoundTvHandle::Create<SoundTv05Quiz>(const SoundTvHandle::CreateArg* arg);
// 0x008201F0
template SoundTvHandle* SoundTvHandle::Create<SoundTv06Travel>(const SoundTvHandle::CreateArg* arg);
// 0x0081FEDC
template SoundTvHandle* SoundTvHandle::Create<SoundTv07Hero>(const SoundTvHandle::CreateArg* arg);
// 0x00820368
template SoundTvHandle* SoundTvHandle::Create<SoundTv08Variety>(const SoundTvHandle::CreateArg* arg);
// 0x008200C0
template SoundTvHandle* SoundTvHandle::Create<SoundTv09Movie>(const SoundTvHandle::CreateArg* arg);
// 0x00820250
template SoundTvHandle* SoundTvHandle::Create<SoundTv10Soccer>(const SoundTvHandle::CreateArg* arg);
// 0x0081FF48
template SoundTvHandle* SoundTvHandle::Create<SoundTv11Talk>(const SoundTvHandle::CreateArg* arg);
// 0x00820118
template SoundTvHandle* SoundTvHandle::Create<SoundTv12Anime>(const SoundTvHandle::CreateArg* arg);
// 0x008203C8
template SoundTvHandle* SoundTvHandle::Create<SoundTv13Cooking>(const SoundTvHandle::CreateArg* arg);
// 0x008204F0
template SoundTvHandle* SoundTvHandle::Create<SoundTv14Exercize>(const SoundTvHandle::CreateArg* arg);
// 0x00820434
template SoundTvHandle* SoundTvHandle::Create<SoundTv15AppleCM>(const SoundTvHandle::CreateArg* arg);
// 0x008202B8
template SoundTvHandle* SoundTvHandle::Create<SoundTv16SingCM>(const SoundTvHandle::CreateArg* arg);
// 0x00820310
template SoundTvHandle* SoundTvHandle::Create<SoundTv17SnowCM>(const SoundTvHandle::CreateArg* arg);
// 0x00820180
template SoundTvHandle* SoundTvHandle::Create<SoundTv18UfoCM>(const SoundTvHandle::CreateArg* arg);
// 0x00820610
template SoundTvHandle* SoundTvHandle::Create<SoundTv19ShoppingCM>(const SoundTvHandle::CreateArg* arg);
// 0x0082048C
template SoundTvHandle* SoundTvHandle::Create<SoundTv20JuiceCM>(const SoundTvHandle::CreateArg* arg);
// 0x008205B0
template SoundTvHandle* SoundTvHandle::Create<SoundTv21LaundryCM>(const SoundTvHandle::CreateArg* arg);
// 0x0082054C
template SoundTvHandle* SoundTvHandle::Create<SoundTv255Weather>(const SoundTvHandle::CreateArg* arg);

// 0x00606004
SoundTvHandle* SoundTvHandle::Create(u32 index, const CreateArg* arg)
{
    return s_TvCreateFuncs[index](arg);
}

// 0x00606018
void SoundTvHandle::Destroy(SoundTvHandle* handle)
{
    if (handle == NULL) {
        return;
    }
    if (handle->m_pTv != NULL) {
        delete handle->m_pTv;
        handle->m_pTv = NULL;
    }
    ::operator delete(handle);
}

// 0x00605FC0
void SoundTvHandle::Start()
{
    m_pTv->Start();
    m_IsStarted = true;
}

// 0x00605FE4
void SoundTvHandle::Load()
{
    if (m_pTv->Load()) {
        m_IsLoaded = true;
    }
}

// 0x00606054
void SoundTvHandle::Update(s32 frame, u16 program, const nn::math::VEC3* world)
{
    nn::math::VEC3 position;
    Func1E5E50(&position, world);
    m_pTv->Update(frame, program, &position);
}

// 0x00606090
void SoundTvHandle::Stop()
{
    m_pTv->Stop();
    m_IsStarted = false;
    m_IsLoaded = false;
}
