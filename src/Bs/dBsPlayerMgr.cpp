#include "Utl/dUtlBase.h"
#include "Other/dBase.h"
#include "Bs/dBsPlayerMgr.h"

// ctor address unknown
BsPlayerMgr::BsPlayerMgr()
{
}

// 0x001C50E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
BsPlayerMgr::~BsPlayerMgr()
{
}

// 0x001C4BF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsPlayerMgr::Calc()
{
}

// 0x001C4B40 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsPlayerMgr::Draw()
{
}

// 0x001C4748 slot 0x40
bool BsPlayerMgr::StartInitialize()
{
}

// 0x001C4740 slot 0x44
oml::framework::Result BsPlayerMgr::UpdateInitialize()
{
}

// 0x001C472C slot 0x48
bool BsPlayerMgr::EndInitialize()
{
}

// 0x001C481C slot 0x4C
bool BsPlayerMgr::StartFinalize()
{
}

// 0x001C4800 slot 0x50
oml::framework::Result BsPlayerMgr::UpdateFinalize()
{
}

// 0x001C47D0 slot 0x54
bool BsPlayerMgr::EndFinalize()
{
}

// 0x001C4834 (name is ours)
void BsPlayerMgr::PlaceOtherPlayers()
{
}
