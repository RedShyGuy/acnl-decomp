// The instances of the delegate templates (the collection area of ARMCC). The game classes they call
// are declared here only (their names are from the RTTI of the instances); the instances of
// TakumiImportPhrase are in dSvPlayer.cpp, whose unnamed namespace has the class.

#include "sead/seadDelegate.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadDelegate1.h"
#include "sead/seadDelegate2.h"
#include "sead/seadDelegateR.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadIDelegate2.h"
#include "sead/seadIDelegateR.h"

class EvBbsNotice;
class StrcBbsNotice;
class DemoDollSetNpcCafeHold;
class SetCommonStrItemNameForMail;
class SetCommonStrTownNameForMail;
class SeaDemoCtrl;
namespace svnpc {
class Animal;
} // namespace svnpc
namespace svqst {
class TimeCapsule;
} // namespace svqst
namespace svfscnve {
class FishingBbsNotice;
} // namespace svfscnve
namespace ugc {
class Text;
} // namespace ugc

namespace {
// a class of the unnamed namespace of a file of the game that is not known
class InsectBbsNotice;
} // namespace

namespace sead {
class CalculateTask;
class FaderTaskBase;
class DualScreenTask;
class MethodTreeNode;
class UlcdTask;
class TaskBase;
class TaskMgr;
class Thread;

// 0x00828340 slot 0x04
template IDelegate1<HumanModel*>* IDelegate1<HumanModel*>::clone(Heap*) const;
// 0x00828348 slot 0x04
template IDelegate1<script::Phrase*>* IDelegate1<script::Phrase*>::clone(Heap*) const;
// 0x00828350 slot 0x04
template IDelegate1<TaskBase*>* IDelegate1<TaskBase*>::clone(Heap*) const;
// 0x00828358 slot 0x04
template IDelegate2<Thread*, s32>* IDelegate2<Thread*, s32>::clone(Heap*) const;
// 0x00828360 slot 0x04
template IDelegateR<bool>* IDelegateR<bool>::clone(Heap*) const;

// 0x00813EC8 slot 0x00
template void sead::Delegate<AcNpcSpResetsan>::invoke();
// 0x00829E70 slot 0x04
template sead::IDelegate* sead::Delegate<AcNpcSpResetsan>::clone(Heap*) const;
// 0x00813F18 slot 0x00
template void sead::Delegate<nfp::Framework>::invoke();
// 0x00829EB8 slot 0x04
template sead::IDelegate* sead::Delegate<nfp::Framework>::clone(Heap*) const;
// 0x00813F68 slot 0x00
template void sead::Delegate<sead::CalculateTask>::invoke();
// 0x00829F00 slot 0x04
template sead::IDelegate* sead::Delegate<sead::CalculateTask>::clone(Heap*) const;
// 0x00813FB8 slot 0x00
template void sead::Delegate<sead::FaderTaskBase>::invoke();
// 0x00829F48 slot 0x04
template sead::IDelegate* sead::Delegate<sead::FaderTaskBase>::clone(Heap*) const;
// 0x00814008 slot 0x00
template void sead::Delegate<sead::DualScreenTask>::invoke();
// 0x00829F90 slot 0x04
template sead::IDelegate* sead::Delegate<sead::DualScreenTask>::clone(Heap*) const;
// 0x00814058 slot 0x00
template void sead::Delegate<sead::MethodTreeNode>::invoke();
// 0x00829FD8 slot 0x04
template sead::IDelegate* sead::Delegate<sead::MethodTreeNode>::clone(Heap*) const;
// 0x008140A8 slot 0x00
template void sead::Delegate<sead::UlcdTask>::invoke();
// 0x0082A020 slot 0x04
template sead::IDelegate* sead::Delegate<sead::UlcdTask>::clone(Heap*) const;

// 0x0081450C slot 0x00
template void sead::Delegate1<EvBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A068 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<EvBbsNotice, script::Phrase*>::clone(Heap*) const;
// 0x00814564 slot 0x00
template void sead::Delegate1<StrcBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A0B0 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<StrcBbsNotice, script::Phrase*>::clone(Heap*) const;
// 0x008145BC slot 0x00
template void sead::Delegate1<DemoDollSetNpcCafeHold, HumanModel*>::invoke(HumanModel*);
// 0x0082A0F8 slot 0x04
template sead::IDelegate1<HumanModel*>* sead::Delegate1<DemoDollSetNpcCafeHold, HumanModel*>::clone(Heap*) const;
// 0x00814614 slot 0x00
template void sead::Delegate1<SetCommonStrItemNameForMail, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A140 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<SetCommonStrItemNameForMail, script::Phrase*>::clone(Heap*) const;
// 0x0081466C slot 0x00
template void sead::Delegate1<SetCommonStrTownNameForMail, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A188 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<SetCommonStrTownNameForMail, script::Phrase*>::clone(Heap*) const;
// 0x0081471C slot 0x00
template void sead::Delegate1<InsectBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A218 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<InsectBbsNotice, script::Phrase*>::clone(Heap*) const;
// 0x00814774 slot 0x00
template void sead::Delegate1<svnpc::Animal, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A260 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<svnpc::Animal, script::Phrase*>::clone(Heap*) const;
// 0x008147CC slot 0x00
template void sead::Delegate1<svqst::TimeCapsule, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A2A8 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<svqst::TimeCapsule, script::Phrase*>::clone(Heap*) const;
// 0x00814824 slot 0x00
template void sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A2F0 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*>::clone(Heap*) const;
// 0x0081487C slot 0x00
template void sead::Delegate1<sead::FaderTaskBase, sead::TaskBase*>::invoke(sead::TaskBase*);
// 0x0082A338 slot 0x04
template sead::IDelegate1<sead::TaskBase*>* sead::Delegate1<sead::FaderTaskBase, sead::TaskBase*>::clone(Heap*) const;

// 0x008148D4 slot 0x00
template void sead::Delegate2<BossSys, sead::Thread*, s32>::invoke(sead::Thread*, s32);
// 0x0082A380 slot 0x04
template sead::IDelegate2<sead::Thread*, s32>* sead::Delegate2<BossSys, sead::Thread*, s32>::clone(Heap*) const;
// 0x0081492C slot 0x00
template void sead::Delegate2<net::nex::Framework, sead::Thread*, s32>::invoke(sead::Thread*, s32);
// 0x0082A3C8 slot 0x04
template sead::IDelegate2<sead::Thread*, s32>* sead::Delegate2<net::nex::Framework, sead::Thread*, s32>::clone(Heap*) const;
// 0x00814984 slot 0x00
template void sead::Delegate2<ugc::Text, sead::Thread*, s32>::invoke(sead::Thread*, s32);
// 0x0082A410 slot 0x04
template sead::IDelegate2<sead::Thread*, s32>* sead::Delegate2<ugc::Text, sead::Thread*, s32>::clone(Heap*) const;
// 0x008149DC slot 0x00
template void sead::Delegate2<sead::TaskMgr, sead::Thread*, s32>::invoke(sead::Thread*, s32);
// 0x0082A458 slot 0x04
template sead::IDelegate2<sead::Thread*, s32>* sead::Delegate2<sead::TaskMgr, sead::Thread*, s32>::clone(Heap*) const;

// 0x00814A34 slot 0x00
template bool sead::DelegateR<SeaDemoCtrl, bool>::invoke();
// 0x0082A4A0 slot 0x04
template sead::IDelegateR<bool>* sead::DelegateR<SeaDemoCtrl, bool>::clone(Heap*) const;
// 0x00814A88 slot 0x00
template bool sead::DelegateR<AcNpcSpMaiko, bool>::invoke();
// 0x0082A4E8 slot 0x04
template sead::IDelegateR<bool>* sead::DelegateR<AcNpcSpMaiko, bool>::clone(Heap*) const;
// 0x00814ADC slot 0x00
template bool sead::DelegateR<MoveFromTalkReceptBase, bool>::invoke();
// 0x0082A530 slot 0x04
template sead::IDelegateR<bool>* sead::DelegateR<MoveFromTalkReceptBase, bool>::clone(Heap*) const;
// 0x00814B30 slot 0x00
template bool sead::DelegateR<AcNpc, bool>::invoke();
// 0x0082A578 slot 0x04
template sead::IDelegateR<bool>* sead::DelegateR<AcNpc, bool>::clone(Heap*) const;
// 0x00814B84 slot 0x00
template bool sead::DelegateR<AcNpcSp, bool>::invoke();
// 0x0082A5C0 slot 0x04
template sead::IDelegateR<bool>* sead::DelegateR<AcNpcSp, bool>::clone(Heap*) const;
// 0x00814BD8 (name is ours)
template sead::DelegateR<AcNpcSp, bool>::DelegateR(AcNpcSp* object, bool (AcNpcSp::*method)());

// 0x008118B0 slot 0x00
// 0x0081183C (deleting dtor)
template sead::DelegateEvent<const char*>::~DelegateEvent();
// 0x00811A4C slot 0x00
// 0x008119D8 (deleting dtor)
template sead::DelegateEvent<sead::TaskBase*>::~DelegateEvent();
// 0x00811B30 slot 0x00
// 0x00811ABC (deleting dtor)
template sead::DelegateEvent<void*>::~DelegateEvent();
// symbols.json names the complete one DelegateEvent<const char*>::Slot; it stores the vptr of <TaskBase*>
// 0x00811980 slot 0x00
// 0x00811920 (deleting dtor)
template sead::DelegateEvent<sead::TaskBase*>::Slot::~Slot();
} // namespace sead
