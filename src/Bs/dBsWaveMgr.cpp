#include "Utl/dUtlBase.h"
#include "Other/dBase.h"
#include "Bs/dBsWaveMgr.h"

// 0x0095182C
BsWaveMgr* BsWaveMgr::s_pInstance;

// 0x006E1B0C (name is ours)
BsWaveMgr* BsWaveMgr::GetInstance()
{
    return s_pInstance;
}

// 0x006E1708 (name is ours)
FieldAttribute BsWaveMgr::GetShoreAttribute(FieldAttribute, const nn::math::VEC3&) const
{
}

// ctor candidate(s) 0x006E1BBC (unverified)
BsWaveMgr::BsWaveMgr()
{
}

// 0x006E1BFC slot 0x00 | slot vf_0x00 of oml::framework::Process
BsWaveMgr::~BsWaveMgr()
{
}

// 0x006E1B3C slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result BsWaveMgr::Calc()
{
}

// 0x006E1B34 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result BsWaveMgr::Draw()
{
}

// 0x006E1700 slot 0x40
bool BsWaveMgr::StartInitialize()
{
}

// 0x006E16F8 slot 0x44
oml::framework::Result BsWaveMgr::UpdateInitialize()
{
}

// 0x006E16E4 slot 0x48
bool BsWaveMgr::EndInitialize()
{
}

// 0x006E1B24 slot 0x4C
bool BsWaveMgr::StartFinalize()
{
}

// 0x006E1B1C slot 0x50
oml::framework::Result BsWaveMgr::UpdateFinalize()
{
}

// 0x006E1AF4 slot 0x54
bool BsWaveMgr::EndFinalize()
{
}

