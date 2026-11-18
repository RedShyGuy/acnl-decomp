#include "bg/dExchangeBank.h"

// The instances of bg::ExchangeBank (ARMCC keeps a copy of each function per instance).

// 0x00344A88 slot 0x00
// 0x00344A50 (deleting dtor)
template bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>::~ExchangeBank();
// 0x007D338C slot 0x14
template s32 bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007D32A0 slot 0x18
template bool bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007D335C slot 0x1C
template bool bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x00344ABC
template bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>::ExchangeBank();
// 0x00344C7C slot 0x00
// 0x00344C44 (deleting dtor)
template bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>::~ExchangeBank();
// 0x007D3518 slot 0x14
template s32 bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007D342C slot 0x18
template bool bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007D34E8 slot 0x1C
template bool bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
