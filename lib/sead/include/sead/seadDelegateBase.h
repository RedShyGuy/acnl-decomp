#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DelegateBase<@anon::InsectBbsNotice, void (@anon::InsectBbsNotice::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D1510  vtable 0x0090516C
//   sead::DelegateBase<@anon::TakumiImportPhrase, void (@anon::TakumiImportPhrase::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D14E0  vtable 0x0090512C
//   sead::DelegateBase<AcNpc, bool (AcNpc::*)(), sead::IDelegateR<bool> >  typeinfo 0x008D14BC  vtable 0x009050FC
//   sead::DelegateBase<AcNpcSp, bool (AcNpcSp::*)(), sead::IDelegateR<bool> >  typeinfo 0x008D14C8  vtable 0x0090510C
//   sead::DelegateBase<AcNpcSpMaiko, bool (AcNpcSpMaiko::*)(), sead::IDelegateR<bool> >  typeinfo 0x008D1468  vtable 0x0090508C
//   sead::DelegateBase<AcNpcSpResetsan, void (AcNpcSpResetsan::*)(), sead::IDelegate>  typeinfo 0x008D1480  vtable 0x009050AC
//   sead::DelegateBase<BossSys, void (BossSys::*)(sead::Thread*, int), sead::IDelegate2<sead::Thread*, int> >  typeinfo 0x008D14D4  vtable 0x0090511C
//   sead::DelegateBase<DemoDollSetNpcCafeHold, void (DemoDollSetNpcCafeHold::*)(HumanModel*), sead::IDelegate1<HumanModel*> >  typeinfo 0x008D148C  vtable 0x009050BC
//   sead::DelegateBase<EvBbsNotice, void (EvBbsNotice::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D1450  vtable 0x0090506C
//   sead::DelegateBase<MoveFromTalkReceptBase, bool (MoveFromTalkReceptBase::*)(), sead::IDelegateR<bool> >  typeinfo 0x008D1498  vtable 0x009050CC
//   sead::DelegateBase<SeaDemoCtrl, bool (SeaDemoCtrl::*)(), sead::IDelegateR<bool> >  typeinfo 0x008D145C  vtable 0x0090507C
//   sead::DelegateBase<SetCommonStrItemNameForMail, void (SetCommonStrItemNameForMail::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D14A4  vtable 0x009050DC
//   sead::DelegateBase<SetCommonStrTownNameForMail, void (SetCommonStrTownNameForMail::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D14B0  vtable 0x009050EC
//   sead::DelegateBase<StrcBbsNotice, void (StrcBbsNotice::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D1474  vtable 0x0090509C
//   sead::DelegateBase<net::nex::Framework, void (net::nex::Framework::*)(sead::Thread*, int), sead::IDelegate2<sead::Thread*, int> >  typeinfo 0x008D14EC  vtable 0x0090513C
//   sead::DelegateBase<nfp::Framework, void (nfp::Framework::*)(), sead::IDelegate>  typeinfo 0x008D14F8  vtable 0x0090514C
//   sead::DelegateBase<sead::CalculateTask, void (sead::CalculateTask::*)(), sead::IDelegate>  typeinfo 0x008D1540  vtable 0x009051AC
//   sead::DelegateBase<sead::DualScreenTask, void (sead::DualScreenTask::*)(), sead::IDelegate>  typeinfo 0x008D1564  vtable 0x009051DC
//   sead::DelegateBase<sead::FaderTaskBase, void (sead::FaderTaskBase::*)(), sead::IDelegate>  typeinfo 0x008D1558  vtable 0x009051CC
//   sead::DelegateBase<sead::FaderTaskBase, void (sead::FaderTaskBase::*)(sead::TaskBase*), sead::IDelegate1<sead::TaskBase*> >  typeinfo 0x008D154C  vtable 0x009051BC
//   sead::DelegateBase<sead::MethodTreeNode, void (sead::MethodTreeNode::*)(), sead::IDelegate>  typeinfo 0x008D1570  vtable 0x009051EC
//   sead::DelegateBase<sead::TaskMgr, void (sead::TaskMgr::*)(sead::Thread*, int), sead::IDelegate2<sead::Thread*, int> >  typeinfo 0x008D157C  vtable 0x009051FC
//   sead::DelegateBase<sead::UlcdTask, void (sead::UlcdTask::*)(), sead::IDelegate>  typeinfo 0x008D1588  vtable 0x0090520C
//   sead::DelegateBase<svfscnve::FishingBbsNotice, void (svfscnve::FishingBbsNotice::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D1534  vtable 0x0090519C
//   sead::DelegateBase<svnpc::Animal, void (svnpc::Animal::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D151C  vtable 0x0090517C
//   sead::DelegateBase<svqst::TimeCapsule, void (svqst::TimeCapsule::*)(script::Phrase*), sead::IDelegate1<script::Phrase*> >  typeinfo 0x008D1528  vtable 0x0090518C
//   sead::DelegateBase<ugc::Text, void (ugc::Text::*)(sead::Thread*, int), sead::IDelegate2<sead::Thread*, int> >  typeinfo 0x008D1504  vtable 0x0090515C
template <typename T0, typename T1, typename T2>
class DelegateBase
{
public:
    // TODO: members unknown
};
} // namespace sead
