#include "Utl/dUtlBase.h"
#include "Other/dBase.h"
#include "Bs/dBsFtrMgr.h"

// ctor address unknown
BsFtrMgr::BsFtrMgr()
{
}

// 0x006950A0 slot 0x00 | slot vf_0x00 of oml::framework::Process
BsFtrMgr::~BsFtrMgr()
{
}

// 0x00694800 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsFtrMgr::Calc()
{
}

// 0x006946D4 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsFtrMgr::Draw()
{
}

// 0x0069114C slot 0x40
bool BsFtrMgr::StartInitialize()
{
}

// 0x00691044 slot 0x44
oml::framework::Result BsFtrMgr::UpdateInitialize()
{
}

// 0x006906C8 slot 0x48
bool BsFtrMgr::EndInitialize()
{
}

// 0x006921E0 slot 0x4C
bool BsFtrMgr::StartFinalize()
{
}

// 0x006921C0 slot 0x50
oml::framework::Result BsFtrMgr::UpdateFinalize()
{
}

// 0x00691E6C slot 0x54
bool BsFtrMgr::EndFinalize()
{
}

// 0x00690F8C | libgarden [tier A]
void BsFtrMgr::CreateFtrActor(BsFtrMgr::CreateFtrArg const&)
{
}
