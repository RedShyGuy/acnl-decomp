#pragma once

#include "decomp.h"
#include "Bank/bank_Extern.h"
#include "Sv/dSvFgName.h"

// The keywords of the exchange banks: what a slot holds (class names from the template arguments in the RTTI; the
// members are ours, their meanings are unknown so far). EMPTY is the item of an empty keyword (NAME_EMPTY 0x7FFE or
// 0x7FFD).

// three bytes that several keywords share (ARMCC copies them as one value; 200, 200, 0 when empty)
struct ExchangeBankKeywordUnknown3
{
    ExchangeBankKeywordUnknown3() : m_0(200), m_1(200), m_2(0) {}
    bool operator==(const ExchangeBankKeywordUnknown3& other) const
    {
        return m_0 == other.m_0 && m_1 == other.m_1 && m_2 == other.m_2;
    }

    u8 m_0; // 0x0
    u8 m_1; // 0x1
    u8 m_2; // 0x2
};
ASSERT_SIZE(ExchangeBankKeywordUnknown3, 0x3);

// an item
template <SvFgName::Name EMPTY>
struct ExchangeBankKeywordFgNameEx : public SvFgName
{
    ExchangeBankKeywordFgNameEx() : SvFgName(static_cast<SvFgName::ID>(EMPTY)) {}
    ~ExchangeBankKeywordFgNameEx() {}

    bool IsSame(const ExchangeBankKeywordFgNameEx& other) const { return Func76BA20(this, &other); }
};

// an item that may be a My Design (the item and more values)
template <SvFgName::Name EMPTY>
struct ExchangeBankKeywordMyDesignFgNameEx
{
    ExchangeBankKeywordMyDesignFgNameEx() : m_Name(static_cast<SvFgName::ID>(EMPTY)), m_pUnknown4(NULL), m_pUnknown8(NULL) {}
    explicit ExchangeBankKeywordMyDesignFgNameEx(const SvFgName& name) : m_Name(name), m_pUnknown4(NULL), m_pUnknown8(NULL) {}
    ~ExchangeBankKeywordMyDesignFgNameEx() {}

    // (the two values at +4 and +8 are not compared)
    bool IsSame(const ExchangeBankKeywordMyDesignFgNameEx& other) const
    {
        return m_UnknownC == other.m_UnknownC && Func76BA20(&m_Name, &other.m_Name);
    }

    // (inline) the data of the design of the item (NULL: none)
    const void* GetDesignData() const
    {
        if (!Func76BBF0(&m_Name) && !Func76BA5C(&m_Name)) {
            return NULL;
        }
        if (m_pUnknown8 != NULL) {
            return m_pUnknown8;
        }
        const u32 index = Func76CAE0(&m_Name) % 10;
        if (m_pUnknown4 == NULL) {
            return NULL;
        }
        return Func7133B4(m_pUnknown4, index);
    }

    SvFgName m_Name;                       // 0x0
    const void* m_pUnknown4;               // 0x4 (Func7133B4 gives the data from it when m_pUnknown8 is NULL)
    const void* m_pUnknown8;               // 0x8 (data passed to the item loader)
    ExchangeBankKeywordUnknown3 m_UnknownC; // 0xC
};

// the keyword of the caps of the player (a My Design keyword and one more value)
struct PlayerCapBankKeyword : public ExchangeBankKeywordMyDesignFgNameEx<SvFgName::NAME_EMPTY>
{
    PlayerCapBankKeyword() : m_Unknown10(-1) {}
    ~PlayerCapBankKeyword() {}

    s32 m_Unknown10; // 0x10
};
ASSERT_SIZE(PlayerCapBankKeyword, 0x14);

// the keyword of the heads of the player (the first four field names after libgarden)
struct PlayerHeadBankKeyword
{
    // (copies for the arrays of the bank)
    DECOMP_EMIT_COPY PlayerHeadBankKeyword() // 0x0032AA5C
        : hairStyle(34), hairColor(-1), eyeColor(-1), item(SvFgName::s_Empty), m_Unknown10(0)
    {
    }
    DECOMP_EMIT_COPY ~PlayerHeadBankKeyword() {} // 0x0032AAA4

    u8 hairStyle;                           // 0x00
    s32 hairColor;                          // 0x04
    s32 eyeColor;                           // 0x08
    SvFgName item;                          // 0x0C
    u8 m_Unknown10;                         // 0x10
    ExchangeBankKeywordUnknown3 m_Unknown11; // 0x11
};
ASSERT_SIZE(PlayerHeadBankKeyword, 0x14);
