#include "sead/hostio/seadNode.h"
#include "sead/seadThreadMgr.h"

namespace sead {
// 0x009763F0
ThreadMgr* ThreadMgr::s_pInstance = nullptr;

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::ThreadMgr::vf_0x00()
{
}

// 0x00562B14 slot 0x04 | slot vf_0x04 of sead::ThreadMgr
sead::ThreadMgr::~ThreadMgr()
{
}

// 0x0053DE74 | nintendogs:bytes [tier A]
sead::ThreadMgr::ThreadMgr()
{
}

} // namespace sead
