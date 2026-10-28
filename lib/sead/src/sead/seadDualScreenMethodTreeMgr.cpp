#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadDualScreenMethodTreeMgr.h"

namespace sead {
// 0x0054D50C slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
sead::DualScreenMethodTreeMgr::~DualScreenMethodTreeMgr()
{
}

// 0x0054CBB8 slot 0x10 | nintendogs:callseq
void sead::DualScreenMethodTreeMgr::attachMethod(int, sead::MethodTreeNode*)
{
}

// 0x0054CDA4 slot 0x14 | virtual slot, introduced by sead::MethodTreeMgr
void sead::DualScreenMethodTreeMgr::vf_0x14()
{
}

// 0x0054CE84 slot 0x18 | nintendogs:bytes
void sead::DualScreenMethodTreeMgr::pauseAll(bool)
{
}

// 0x0054CCD4 slot 0x1C | virtual slot, introduced by sead::MethodTreeMgr
void sead::DualScreenMethodTreeMgr::vf_0x1C()
{
}

// 0x0054CFF8 | nintendogs:bytes [tier A]
sead::DualScreenMethodTreeMgr::DualScreenMethodTreeMgr()
{
}

} // namespace sead

// the type information of the classes below MethodTreeMgr
// 0x00828D28 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::MethodTreeMgr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
