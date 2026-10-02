#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"
#include "Human/dHumanModel_FaceCtrl.h"

// RTTI 20HumanFaceAnimControl @ 0x008CCB7C
// vtable 0x008F5838 (vptr 0x008F5840), offset_to_top 0, 2 entries
class HumanFaceAnimControl : public ::HumanModel::FaceCtrl
{
public:
    HumanFaceAnimControl(); // ctor candidate(s) 0x0031E710 (unverified)
    virtual void vf_0x00(); // 0x0031E7F0 slot 0x00 | virtual slot, introduced by HumanFaceAnimControl
    virtual void vf_0x04(); // 0x0031E7C8 slot 0x04 | virtual slot, introduced by HumanFaceAnimControl
};
