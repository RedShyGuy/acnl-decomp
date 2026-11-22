#include "Ac/dAcNpcDemoDollBase.h"
#include "Ac/dAcNpcDemoDollPlayer.h"
#include "Player/dPlayerModel.h"

// ctor candidate(s) 0x007F1594 (unverified)
AcNpcDemoDollPlayer::AcNpcDemoDollPlayer()
{
}

// 0x002ECDD8 slot 0x00 | slot vf_0x00 of oml::framework::Process
AcNpcDemoDollPlayer::~AcNpcDemoDollPlayer()
{
}

// 0x002ECC48 slot 0x80
bool AcNpcDemoDollPlayer::EndInitialize()
{
}

// 0x002ECD10 slot 0x164 | virtual slot, introduced by AcNpcDemoDollBase
void AcNpcDemoDollPlayer::vf_0x164()
{
}

// 0x001D3168 slot 0x168 | virtual slot, introduced by AcNpcDemoDollBase
void AcNpcDemoDollPlayer::vf_0x168()
{
    // (the PlayerModel at +0xB24 is unloaded; the class layout is not written yet)
    reinterpret_cast<PlayerModel*>(reinterpret_cast<u8*>(this) + 0xB24)->Unload();
}

