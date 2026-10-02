#include "pead/hostio/peadNode.h"
#include "pead/peadThreadMgr.h"

namespace pead {
// 0x0097E43C
ThreadMgr* ThreadMgr::s_pInstance = nullptr;

// ctor candidate(s) 0x0053DE74 (unverified)
pead::ThreadMgr::ThreadMgr()
{
}

// 0x0053DEBC slot 0x00 | nintendogs:bytes
pead::ThreadMgr::~ThreadMgr()
{
}

} // namespace pead
