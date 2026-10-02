#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal10BasicSoundE @ 0x008D097C
// vtable 0x00903004 (vptr 0x0090300C), offset_to_top 0, 13 entries
class BasicSound
{
public:
    struct AmbientInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BasicSound(); // ctor candidate(s) 0x004C57E4 (unverified)
    virtual void GetRuntimeTypeInfo() const; // 0x0073F2E8 slot 0x00 | slot vf_0x00 of nw::snd::internal::BasicSound
    virtual ~BasicSound(); // 0x004C58C8 slot 0x04 | slot vf_0x04 of nw::snd::internal::BasicSound
    virtual void vf_0x08(); // 0x004C58B0 slot 0x08 | virtual slot, introduced by nw::snd::internal::BasicSound
    virtual void Initialize(); // 0x004C49AC slot 0x0C | fefates:bytes
    virtual void Finalize(); // 0x004C5604 slot 0x10 | fefates:bytes
    virtual void IsPrepared() const; // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void IsAttachedTempSpecialHandle(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void DetachTempSpecialHandle(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void GetBasicSoundPlayerHandle(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void OnUpdatePlayerPriority(); // 0x004C4F48 slot 0x24 | slot vf_0x24 of nw::snd::internal::BasicSound
    virtual void UpdateMoveValue(); // 0x004C4E3C slot 0x28 | fefates:bytes
    virtual void OnUpdateParam(); // 0x004C4D70 slot 0x2C | slot vf_0x2C of nw::snd::internal::BasicSound
    virtual void UpdateParam(); // 0x004C4BBC slot 0x30 | fefates:bytes
    void DetachGeneralHandle(); // 0x00137B28 | nintendogs:callgraph [tier A]
    void Stop(int); // 0x00137CC0 | fefates:bytes [tier B]
    void SetPanCurve(nw::snd::PanCurve); // 0x004C4B54 | fefates:bytes [tier B]
    void StartPrepared(); // 0x004C4D74 | nintendogs:callgraph [tier A]
    void SetAmbientInfo(const nw::snd::internal::BasicSound::AmbientInfo&); // 0x004C4D80 | nintendogs:bytes [tier A]
    void SetFrontBypass(bool); // 0x004C4DCC | fefates:bytes [tier B]
    void SetInitialVolume(float); // 0x004C4E7C | fefates:bytes [tier B]
    void SetPlayerPriority(int); // 0x004C4ED8 | fefates:bytes [tier B]
    void GetAmbientPriority(const nw::snd::internal::BasicSound::AmbientInfo&, unsigned); // 0x004C4F08 | nintendogs:bytes [tier A]
    void DetachTempGeneralHandle(); // 0x004C4F4C | nintendogs:callgraph [tier A]
    void IsAttachedGeneralHandle(); // 0x004C4F54 | nintendogs:callgraph [tier A]
    void IsAttachedTempGeneralHandle(); // 0x004C4F94 | nintendogs:callgraph [tier A]
    void Pause(bool, int); // 0x004C4FA4 | mk7dlp:callgraph [tier A]
    void FadeIn(int); // 0x004C50AC | fefates:bytes [tier B]
    void Update(); // 0x004C511C | fefates:bytes [tier B]
    void SetVolume(float, int); // 0x004C5798 | fefates:bytes [tier B]
    void SetPanMode(nw::snd::PanMode); // 0x004C9150 | nintendogs:callseq [tier A]
    void IsPause() const; // 0x0073F2F4 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
