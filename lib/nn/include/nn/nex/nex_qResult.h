#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The result of nex: a code (negative: failure) and the source file and line where it was made
// (the constructors note their own place in Result.cpp). ARMCC inlines the constructors and the
// assignment almost everywhere; the out-of-line copies come from these inline definitions. The
// member names are ours.
class qResult
{
public:
    static const s32 SUCCESS = 0x00010001;
    static const s32 FAILURE = static_cast<s32>(0x80010001);

    qResult() // 0x003D4030 | mk7dlp:callseq-callee [tier A]
    {
        m_Code = FAILURE;
        m_Line = 106;
        m_pFile = FILE_NAME;
    }
    qResult(const bool& isSuccess) // 0x003D3FDC | fefates:callgraph [tier C]
    {
        m_Line = 127;
        m_pFile = FILE_NAME;
        m_Code = isSuccess ? SUCCESS : FAILURE;
    }
    qResult(const int& code) // 0x003D4010 | mk7dlp:callgraph [tier A]
    {
        m_Code = code;
        m_Line = 113;
        m_pFile = FILE_NAME;
    }
    qResult(const qResult& other) : m_Code(other.m_Code), m_pFile(other.m_pFile), m_Line(other.m_Line) {}
    nn::nex::qResult& operator=(const nn::nex::qResult& other) // 0x003D4054 | mk7dlp:callgraph [tier A]
    {
        m_Code = other.m_Code;
        m_pFile = other.m_pFile;
        m_Line = other.m_Line;
        return *this;
    }
    // (inline; it notes its own place too)
    nn::nex::qResult& operator=(const bool& isSuccess)
    {
        m_Code = isSuccess ? SUCCESS : FAILURE;
        m_Line = 162;
        m_pFile = FILE_NAME;
        return *this;
    }
    // (inline in the original, with a copy out of line)
    bool Equals(const bool& isSuccess) const; // 0x0072E5FC | mk7dlp:bytes [tier A]
    // (inline in the original, with a copy out of line; name is ours)
    bool Equals(const int& code) const; // 0x0072E61C
    // true if the code is not negative (no failure)
    operator bool() const; // 0x0072E634

    s32 m_Code;              // 0x0
    const wchar_t* m_pFile;  // 0x4
    u32 m_Line;              // 0x8

    // (the source of the constructors)
    static constexpr const wchar_t* FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Platform/Core/Result.cpp";
};
ASSERT_SIZE(qResult, 0xC);
} // namespace nex
} // namespace nn
