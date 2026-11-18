#pragma once

#include "decomp.h"
#include "Exchange/dExchangeBank.h"

// An exchange bank that loads files (class name from the RTTI; the member names are ours): MODEL_NUM loaders per slot
// (TLoader: item::ResLoader or g3d::ResourceLoader). The heap of a slot is 384 bytes per loader larger than the files (the loader
// needs them). No virtual functions of its own.
// Instantiations found in the binary:
//   ExchangeResBank<13772u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 1u, true>  typeinfo 0x008CC02C  vtable 0x008F1918
//   ExchangeResBank<16768u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 1u, false>  typeinfo 0x008CC038  vtable 0x008F1940
//   ExchangeResBank<17703u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 2u, true>  typeinfo 0x008CC044  vtable 0x008F1968
//   ExchangeResBank<17829u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC050  vtable 0x008F1990
//   ExchangeResBank<17879u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC05C  vtable 0x008F19B8
//   ExchangeResBank<24448u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 2u, false>  typeinfo 0x008CC068  vtable 0x008F19E0
//   ExchangeResBank<25891u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, g3d::ResourceLoader, 1u, false>  typeinfo 0x008CC074  vtable 0x008F1A08
//   ExchangeResBank<31423u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, item::ResLoader, 2u, true>  typeinfo 0x008CC080  vtable 0x008F1A30
//   ExchangeResBank<32891u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, item::ResLoader, 2u, true>  typeinfo 0x008CC08C  vtable 0x008F1A58
//   ExchangeResBank<66000u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 1u, false>  typeinfo 0x008CC098  vtable 0x008F1A80
//   ExchangeResBank<8600u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 1u, false>  typeinfo 0x008CC0A4  vtable 0x008F1AA8
//   ExchangeResBank<8600u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, item::ResLoader, 1u, false>  typeinfo 0x008CC0B0  vtable 0x008F1AD0
template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
class ExchangeResBank : public ExchangeBank<FILE_SIZE + 384 * MODEL_NUM, TVramConfig, TKeyword, FLAG>
{
public:
    ExchangeResBank() {}
    virtual ~ExchangeResBank();

    // (inline) the loader index of the slot (the first one for a slot out of range)
    TLoader* GetLoader(u32 slot, u32 index) { return slot < 2 ? &m_Loaders[index][slot] : &m_Loaders[0][0]; }
    const TLoader* GetLoader(u32 slot, u32 index) const { return slot < 2 ? &m_Loaders[index][slot] : &m_Loaders[0][0]; }

    TLoader m_Loaders[MODEL_NUM][2];
};

template <u32 FILE_SIZE, typename TVramConfig, typename TKeyword, typename TLoader, u32 MODEL_NUM, bool FLAG>
inline ExchangeResBank<FILE_SIZE, TVramConfig, TKeyword, TLoader, MODEL_NUM, FLAG>::~ExchangeResBank()
{
    // (only the members and the base are destroyed)
}
