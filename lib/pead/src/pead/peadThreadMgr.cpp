#include "pead/hostio/peadNode.h"
#include "pead/peadThreadMgr.h"

namespace pead {
// 0x0097E43C
ThreadMgr* ThreadMgr::s_pInstance = nullptr;

// 0x0053DEBC slot 0x00 | nintendogs:bytes
pead::ThreadMgr::~ThreadMgr()
{
}

// 0x0053DE74 | nintendogs:bytes [tier A]
pead::ThreadMgr::ThreadMgr()
{
}

} // namespace pead
