#include "sead/hostio/seadNode.h"
#include "sead/seadAudioMgr.h"

namespace sead {
// 0x009762D8
AudioMgr* AudioMgr::s_pInstance = nullptr;

// ctor address unknown
sead::AudioMgr::AudioMgr()
{
}

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::AudioMgr::vf_0x00()
{
}

// 0x00560C0C slot 0x04 | virtual slot, introduced by sead::AudioMgr
void sead::AudioMgr::vf_0x04()
{
}

// 0x00560B78 slot 0x08 | virtual slot, introduced by sead::AudioMgr
void sead::AudioMgr::vf_0x08()
{
}

} // namespace sead
