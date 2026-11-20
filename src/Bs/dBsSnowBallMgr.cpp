#include "Utl/dUtlBase.h"
#include "Other/dBase.h"
#include "Bs/dBsSnowBallMgr.h"

// ctor candidate(s) 0x0022705C (unverified)
BsSnowBallMgr::BsSnowBallMgr()
{
}

// 0x002270B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
BsSnowBallMgr::~BsSnowBallMgr()
{
}

// 0x00226F5C slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsSnowBallMgr::Calc()
{
}

// 0x00226F54 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsSnowBallMgr::Draw()
{
}

// 0x00225CF8 slot 0x40
bool BsSnowBallMgr::StartInitialize()
{
}

// 0x00225C9C slot 0x44
oml::framework::Result BsSnowBallMgr::UpdateInitialize()
{
}

// 0x00225C64 slot 0x48
bool BsSnowBallMgr::EndInitialize()
{
}

// 0x0022616C slot 0x4C
bool BsSnowBallMgr::StartFinalize()
{
}

// 0x00226164 slot 0x50
oml::framework::Result BsSnowBallMgr::UpdateFinalize()
{
}

// 0x00225EE4 slot 0x54
bool BsSnowBallMgr::EndFinalize()
{
}

// 0x009516A4 (name is ours)
BsSnowBallMgr* BsSnowBallMgr::s_pInstance;
