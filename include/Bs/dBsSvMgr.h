#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// RTTI 7BsSvMgr @ 0x008CD354
// vtable 0x008F8EB8 (vptr 0x008F8EC0), offset_to_top 0, 18 entries
// vtable 0x008F8F08 (vptr 0x008F8F10), offset_to_top -20, 63 entries
// vtable 0x008F900C (vptr 0x008F9014), offset_to_top -144, 3 entries
class BsSvMgr : public ::Base, public ::script::ITalkRecept, public ::state::Mode<BsSvMgr>
{
public:
    class SaveStep;
    BsSvMgr(); // ctor address unknown
    virtual ~BsSvMgr(); // 0x0060B9DC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0060B984 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00609258 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0060B8D8 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00609368 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void vf_0x40(); // 0x00607E38 slot 0x40 | virtual slot, introduced by BsSvMgr
    virtual void vf_0x44(); // 0x00607FE0 slot 0x44 | virtual slot, introduced by BsSvMgr
    void SetSaveConnectNetHst(); // 0x0060867C | libgarden [tier A]
    void SetSaveConnectNetVst(); // 0x00608698 | libgarden [tier A]
    void SetSaveContinueNetVst(); // 0x00608894 | libgarden [tier A]
    void SetSaveInterruptNetVst(); // 0x00608A74 | libgarden [tier A]
    void SetSaveSeeOffRandomMatchNetHst(); // 0x00608CBC | libgarden [tier A]
    void SetSaveSeeOffRandomMatchNetVst(); // 0x00608CD8 | libgarden [tier A]
    void SetSaveConnectRandomMatchNetHst(); // 0x00608DF4 | libgarden [tier A]
    void SetSaveConnectRandomMatchNetVst(); // 0x00608E10 | libgarden [tier A]
    void SetSaveStartTourRandomMatchNetHst(); // 0x00608F7C | libgarden [tier A]
    void SetSaveStartTourRandomMatchNetVst(); // 0x00608F98 | libgarden [tier A]
    void SetSaveSeeOffNetHostRandomMatchNetVst(); // 0x006091B4 | libgarden [tier A]
};
