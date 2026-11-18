#include "Bank/dBankTable.h"
#include "Bank/dBank.h"
#include "Human/dHumanResTextureAnimBank.h"
#include "Npc/dNpcTopsBank.h"
#include "Player/dPlayerAcceBank.h"
#include "Player/dPlayerBottomsBank.h"
#include "Player/dPlayerCapBank.h"
#include "Player/dPlayerFaceBank.h"
#include "Player/dPlayerHeadBank.h"
#include "Player/dPlayerLegsBank.h"
#include "Player/dPlayerShoesBank.h"
#include "Player/dPlayerTopsBank.h"
#include "Other/dToolBank.h"

// The instances of BankTable (ARMCC keeps a copy of each function per instance).

// 0x00825C6C slot 0x00
template bool BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::Release(unsigned int index);
// 0x0082E0A4 slot 0x04
template bool BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::IsValid(unsigned int index);
// 0x0082E080 slot 0x08
template bool BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::CheckBit(unsigned int index);
// 0x00825CC0 slot 0x0C
template void BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::SetBit(unsigned int index);
// 0x00825C38 slot 0x10
template void BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::ClearBit(unsigned int index);
// 0x00825C64 slot 0x14
template void BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::ResetBit();
// 0x00825DDC slot 0x18
// 0x00825CEC (deleting dtor)
template BankTable<Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 50u>::~BankTable();

// 0x008259EC slot 0x00
template bool BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::Release(unsigned int index);
// 0x0082E058 slot 0x04
template bool BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::IsValid(unsigned int index);
// 0x0082E034 slot 0x08
template bool BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::CheckBit(unsigned int index);
// 0x00825A40 slot 0x0C
template void BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::SetBit(unsigned int index);
// 0x008259B4 slot 0x10
template void BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ClearBit(unsigned int index);
// 0x008259E0 slot 0x14
template void BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ResetBit();
// 0x00825B54 slot 0x18
// 0x00825A6C (deleting dtor)
template BankTable<Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::~BankTable();

// 0x00825F00 slot 0x00
template bool BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::Release(unsigned int index);
// 0x0082E0F0 slot 0x04
template bool BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::IsValid(unsigned int index);
// 0x0082E0CC slot 0x08
template bool BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::CheckBit(unsigned int index);
// 0x00825F54 slot 0x0C
template void BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::SetBit(unsigned int index);
// 0x00825EC8 slot 0x10
template void BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::ClearBit(unsigned int index);
// 0x00825EF4 slot 0x14
template void BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::ResetBit();
// 0x00825F90 slot 0x18
// 0x00825F80 (deleting dtor)
template BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u>::~BankTable();

// 0x008260AC slot 0x00
template bool BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::Release(unsigned int index);
// 0x0082E13C slot 0x04
template bool BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::IsValid(unsigned int index);
// 0x0082E118 slot 0x08
template bool BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::CheckBit(unsigned int index);
// 0x00826100 slot 0x0C
template void BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::SetBit(unsigned int index);
// 0x00826074 slot 0x10
template void BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ClearBit(unsigned int index);
// 0x008260A0 slot 0x14
template void BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ResetBit();
// 0x0082613C slot 0x18
// 0x0082612C (deleting dtor)
template BankTable<Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::~BankTable();

// 0x00826254 slot 0x00
template bool BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::Release(unsigned int index);
// 0x0082E188 slot 0x04
template bool BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::IsValid(unsigned int index);
// 0x0082E164 slot 0x08
template bool BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::CheckBit(unsigned int index);
// 0x008262A8 slot 0x0C
template void BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::SetBit(unsigned int index);
// 0x00826220 slot 0x10
template void BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::ClearBit(unsigned int index);
// 0x0082624C slot 0x14
template void BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::ResetBit();
// 0x008262E4 slot 0x18
// 0x008262D4 (deleting dtor)
template BankTable<Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >, 50u>::~BankTable();

// 0x00825840 slot 0x00
template bool BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::Release(unsigned int index);
// 0x0082E00C slot 0x04
template bool BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::IsValid(unsigned int index);
// 0x0082DFE8 slot 0x08
template bool BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::CheckBit(unsigned int index);
// 0x00825894 slot 0x0C
template void BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::SetBit(unsigned int index);
// 0x00825808 slot 0x10
template void BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ClearBit(unsigned int index);
// 0x00825834 slot 0x14
template void BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::ResetBit();
// 0x008258D0 slot 0x18
// 0x008258C0 (deleting dtor)
template BankTable<Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 10u>::~BankTable();

// 0x00825350 slot 0x00
template bool BankTable<HumanResTextureAnimBank, 10u>::Release(unsigned int index);
// 0x0082DF74 slot 0x04
template bool BankTable<HumanResTextureAnimBank, 10u>::IsValid(unsigned int index);
// 0x0082DF50 slot 0x08
template bool BankTable<HumanResTextureAnimBank, 10u>::CheckBit(unsigned int index);
// 0x0082545C slot 0x0C
template void BankTable<HumanResTextureAnimBank, 10u>::SetBit(unsigned int index);
// 0x00825318 slot 0x10
template void BankTable<HumanResTextureAnimBank, 10u>::ClearBit(unsigned int index);
// 0x00825344 slot 0x14
template void BankTable<HumanResTextureAnimBank, 10u>::ResetBit();
// 0x00825570 slot 0x18
// 0x00825488 (deleting dtor)
template BankTable<HumanResTextureAnimBank, 10u>::~BankTable();

// 0x00825690 slot 0x00
template bool BankTable<HumanResTextureAnimBank, 14u>::Release(unsigned int index);
// 0x0082DFC0 slot 0x04
template bool BankTable<HumanResTextureAnimBank, 14u>::IsValid(unsigned int index);
// 0x0082DF9C slot 0x08
template bool BankTable<HumanResTextureAnimBank, 14u>::CheckBit(unsigned int index);
// 0x008256E4 slot 0x0C
template void BankTable<HumanResTextureAnimBank, 14u>::SetBit(unsigned int index);
// 0x00825654 slot 0x10
template void BankTable<HumanResTextureAnimBank, 14u>::ClearBit(unsigned int index);
// 0x00825680 slot 0x14
template void BankTable<HumanResTextureAnimBank, 14u>::ResetBit();
// 0x00825720 slot 0x18
// 0x00825710 (deleting dtor)
template BankTable<HumanResTextureAnimBank, 14u>::~BankTable();

// 0x008237A8 slot 0x00
template bool BankTable<NpcTopsBank, 10u>::Release(unsigned int index);
// 0x0082DBE0 slot 0x04
template bool BankTable<NpcTopsBank, 10u>::IsValid(unsigned int index);
// 0x0082DBB8 slot 0x08
template bool BankTable<NpcTopsBank, 10u>::CheckBit(unsigned int index);
// 0x00823998 slot 0x0C
template void BankTable<NpcTopsBank, 10u>::SetBit(unsigned int index);
// 0x00823764 slot 0x10
template void BankTable<NpcTopsBank, 10u>::ClearBit(unsigned int index);
// 0x00823794 slot 0x14
template void BankTable<NpcTopsBank, 10u>::ResetBit();
// 0x008239D8 slot 0x18
// 0x008239C8 (deleting dtor)
template BankTable<NpcTopsBank, 10u>::~BankTable();

// 0x00823F18 slot 0x00
template bool BankTable<PlayerAcceBank, 7u>::Release(unsigned int index);
// 0x0082DCB0 slot 0x04
template bool BankTable<PlayerAcceBank, 7u>::IsValid(unsigned int index);
// 0x0082DC88 slot 0x08
template bool BankTable<PlayerAcceBank, 7u>::CheckBit(unsigned int index);
// 0x00823FDC slot 0x0C
template void BankTable<PlayerAcceBank, 7u>::SetBit(unsigned int index);
// 0x00823ED4 slot 0x10
template void BankTable<PlayerAcceBank, 7u>::ClearBit(unsigned int index);
// 0x00823F04 slot 0x14
template void BankTable<PlayerAcceBank, 7u>::ResetBit();
// 0x0082401C slot 0x18
// 0x0082400C (deleting dtor)
template BankTable<PlayerAcceBank, 7u>::~BankTable();

// 0x00825054 slot 0x00
template bool BankTable<PlayerBottomsBank, 7u>::Release(unsigned int index);
// 0x0082DF0C slot 0x04
template bool BankTable<PlayerBottomsBank, 7u>::IsValid(unsigned int index);
// 0x0082DEE8 slot 0x08
template bool BankTable<PlayerBottomsBank, 7u>::CheckBit(unsigned int index);
// 0x00825120 slot 0x0C
template void BankTable<PlayerBottomsBank, 7u>::SetBit(unsigned int index);
// 0x00825018 slot 0x10
template void BankTable<PlayerBottomsBank, 7u>::ClearBit(unsigned int index);
// 0x00825044 slot 0x14
template void BankTable<PlayerBottomsBank, 7u>::ResetBit();
// 0x0082515C slot 0x18
// 0x0082514C (deleting dtor)
template BankTable<PlayerBottomsBank, 7u>::~BankTable();

// 0x00823BA4 slot 0x00
template bool BankTable<PlayerCapBank, 7u>::Release(unsigned int index);
// 0x0082DC48 slot 0x04
template bool BankTable<PlayerCapBank, 7u>::IsValid(unsigned int index);
// 0x0082DC20 slot 0x08
template bool BankTable<PlayerCapBank, 7u>::CheckBit(unsigned int index);
// 0x00823C6C slot 0x0C
template void BankTable<PlayerCapBank, 7u>::SetBit(unsigned int index);
// 0x00823B60 slot 0x10
template void BankTable<PlayerCapBank, 7u>::ClearBit(unsigned int index);
// 0x00823B90 slot 0x14
template void BankTable<PlayerCapBank, 7u>::ResetBit();
// 0x00823CAC slot 0x18
// 0x00823C9C (deleting dtor)
template BankTable<PlayerCapBank, 7u>::~BankTable();

// 0x00824210 slot 0x00
template bool BankTable<PlayerFaceBank, 14u>::Release(unsigned int index);
// 0x0082DD14 slot 0x04
template bool BankTable<PlayerFaceBank, 14u>::IsValid(unsigned int index);
// 0x0082DCEC slot 0x08
template bool BankTable<PlayerFaceBank, 14u>::CheckBit(unsigned int index);
// 0x008242A0 slot 0x0C
template void BankTable<PlayerFaceBank, 14u>::SetBit(unsigned int index);
// 0x008241CC slot 0x10
template void BankTable<PlayerFaceBank, 14u>::ClearBit(unsigned int index);
// 0x008241FC slot 0x14
template void BankTable<PlayerFaceBank, 14u>::ResetBit();
// 0x008242E0 slot 0x18
// 0x008242D0 (deleting dtor)
template BankTable<PlayerFaceBank, 14u>::~BankTable();

// 0x00824418 slot 0x00
template bool BankTable<PlayerHeadBank, 7u>::Release(unsigned int index);
// 0x0082DD68 slot 0x04
template bool BankTable<PlayerHeadBank, 7u>::IsValid(unsigned int index);
// 0x0082DD40 slot 0x08
template bool BankTable<PlayerHeadBank, 7u>::CheckBit(unsigned int index);
// 0x008244A8 slot 0x0C
template void BankTable<PlayerHeadBank, 7u>::SetBit(unsigned int index);
// 0x008243D4 slot 0x10
template void BankTable<PlayerHeadBank, 7u>::ClearBit(unsigned int index);
// 0x00824404 slot 0x14
template void BankTable<PlayerHeadBank, 7u>::ResetBit();
// 0x008244E8 slot 0x18
// 0x008244D8 (deleting dtor)
template BankTable<PlayerHeadBank, 7u>::~BankTable();

// 0x00824694 slot 0x00
template bool BankTable<PlayerLegsBank, 7u>::Release(unsigned int index);
// 0x0082DDD0 slot 0x04
template bool BankTable<PlayerLegsBank, 7u>::IsValid(unsigned int index);
// 0x0082DDA8 slot 0x08
template bool BankTable<PlayerLegsBank, 7u>::CheckBit(unsigned int index);
// 0x00824760 slot 0x0C
template void BankTable<PlayerLegsBank, 7u>::SetBit(unsigned int index);
// 0x00824650 slot 0x10
template void BankTable<PlayerLegsBank, 7u>::ClearBit(unsigned int index);
// 0x00824680 slot 0x14
template void BankTable<PlayerLegsBank, 7u>::ResetBit();
// 0x008247A0 slot 0x18
// 0x00824790 (deleting dtor)
template BankTable<PlayerLegsBank, 7u>::~BankTable();

// 0x00824D54 slot 0x00
template bool BankTable<PlayerShoesBank, 7u>::Release(unsigned int index);
// 0x0082DEA8 slot 0x04
template bool BankTable<PlayerShoesBank, 7u>::IsValid(unsigned int index);
// 0x0082DE80 slot 0x08
template bool BankTable<PlayerShoesBank, 7u>::CheckBit(unsigned int index);
// 0x00824E1C slot 0x0C
template void BankTable<PlayerShoesBank, 7u>::SetBit(unsigned int index);
// 0x00824D10 slot 0x10
template void BankTable<PlayerShoesBank, 7u>::ClearBit(unsigned int index);
// 0x00824D40 slot 0x14
template void BankTable<PlayerShoesBank, 7u>::ResetBit();
// 0x00824E5C slot 0x18
// 0x00824E4C (deleting dtor)
template BankTable<PlayerShoesBank, 7u>::~BankTable();

// 0x008249F4 slot 0x00
template bool BankTable<PlayerTopsBank, 7u>::Release(unsigned int index);
// 0x0082DE3C slot 0x04
template bool BankTable<PlayerTopsBank, 7u>::IsValid(unsigned int index);
// 0x0082DE14 slot 0x08
template bool BankTable<PlayerTopsBank, 7u>::CheckBit(unsigned int index);
// 0x00824AC0 slot 0x0C
template void BankTable<PlayerTopsBank, 7u>::SetBit(unsigned int index);
// 0x008249B0 slot 0x10
template void BankTable<PlayerTopsBank, 7u>::ClearBit(unsigned int index);
// 0x008249E0 slot 0x14
template void BankTable<PlayerTopsBank, 7u>::ResetBit();
// 0x00824B00 slot 0x18
// 0x00824AF0 (deleting dtor)
template BankTable<PlayerTopsBank, 7u>::~BankTable();

// 0x008268C0 slot 0x00
template bool BankTable<ToolBank, 7u>::Release(unsigned int index);
// 0x0082E244 slot 0x04
template bool BankTable<ToolBank, 7u>::IsValid(unsigned int index);
// 0x0082E21C slot 0x08
template bool BankTable<ToolBank, 7u>::CheckBit(unsigned int index);
// 0x00826954 slot 0x0C
template void BankTable<ToolBank, 7u>::SetBit(unsigned int index);
// 0x0082687C slot 0x10
template void BankTable<ToolBank, 7u>::ClearBit(unsigned int index);
// 0x008268AC slot 0x14
template void BankTable<ToolBank, 7u>::ResetBit();
// 0x00826994 slot 0x18
// 0x00826984 (deleting dtor)
template BankTable<ToolBank, 7u>::~BankTable();

// 0x00826414 slot 0x00
template bool BankTable<ToolBank, 10u>::Release(unsigned int index);
// 0x0082E1D8 slot 0x04
template bool BankTable<ToolBank, 10u>::IsValid(unsigned int index);
// 0x0082E1B0 slot 0x08
template bool BankTable<ToolBank, 10u>::CheckBit(unsigned int index);
// 0x0082665C slot 0x0C
template void BankTable<ToolBank, 10u>::SetBit(unsigned int index);
// 0x008263D0 slot 0x10
template void BankTable<ToolBank, 10u>::ClearBit(unsigned int index);
// 0x00826400 slot 0x14
template void BankTable<ToolBank, 10u>::ResetBit();
// 0x0082669C slot 0x18
// 0x0082668C (deleting dtor)
template BankTable<ToolBank, 10u>::~BankTable();
