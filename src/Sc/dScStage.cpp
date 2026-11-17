#include "Utl/dUtlBase.h"
#include "Bs/dBsScene.h"
#include "Sc/dScStage.h"

// 0x00952684
s32 g_StageFrameCounter;

// ctor address unknown
ScStage::ScStage()
{
}

// 0x0061373C slot 0x00 | slot vf_0x00 of oml::framework::Process
ScStage::~ScStage()
{
}

// 0x006134F4 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result ScStage::Calc()
{
}

// 0x006134EC slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result ScStage::Draw()
{
}

// 0x0075EAE4 slot 0x40 | virtual slot, introduced by BsScene
void ScStage::vf_0x40()
{
}

// 0x00612394 slot 0x44
bool ScStage::StartInitialize()
{
}

// 0x00611ECC slot 0x48
oml::framework::Result ScStage::UpdateInitialize()
{
}

// 0x00611B8C slot 0x4C
bool ScStage::EndInitialize()
{
}

// 0x00612E18 slot 0x50
bool ScStage::StartFinalize()
{
}

// 0x00612D48 slot 0x54
oml::framework::Result ScStage::UpdateFinalize()
{
}

// 0x00612B20 slot 0x58
bool ScStage::EndFinalize()
{
}

