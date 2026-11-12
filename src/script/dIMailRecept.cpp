#include "script/dIMailRecept.h"

namespace script {
// 0x005DDF60 (name after the RTTI)
IMailRecept::IMailRecept()
    : m_pToWord(NULL), m_pBodyWord(NULL), m_pFromWord(NULL), m_pToResult(NULL), m_ToFile(), m_BodyFile(), m_FromFile(),
      m_Mode(MODE_NONE), m_Status(STATUS_NONE)
{
    m_Labels[0] = -1;
    m_Labels[1] = -1;
    m_Labels[2] = -1;
    m_Labels[3] = -1;
    m_Labels[4] = -1;
}

// 0x005DDFDC slot 0x00 (name after the RTTI)
// 0x005DDFCC (deleting dtor)
IMailRecept::~IMailRecept()
{
    // (only the members are destroyed)
}
// 0x005DDD64 (name is ours)
void IMailRecept::SetupRandom(IWord* to, IWord* body, IWord* from, s32* toResult, const char* toFile, s32 toLabel,
                              const char* bodyFile, s32 labelA, s32 labelB, s32 labelC, const char* fromFile,
                              s32 fromLabel)
{
    m_pToWord = to;
    m_pBodyWord = body;
    m_pFromWord = from;
    m_pToResult = toResult;
    m_ToFile.copy(sead::SafeStringBase<char>(toFile));
    m_BodyFile.copy(sead::SafeStringBase<char>(bodyFile));
    m_FromFile.copy(sead::SafeStringBase<char>(fromFile));
    m_Status = 0;
    m_Labels[0] = toLabel;
    m_Mode = MODE_RANDOM;
    m_Labels[1] = labelA;
    m_Labels[2] = labelB;
    m_Labels[3] = labelC;
    m_Labels[4] = fromLabel;
}

// 0x005DDE18 (name is ours)
void IMailRecept::SetupNormal(IWord* to, IWord* body, IWord* from, s32* toResult, const char* toFile, s32 toLabel,
                              const char* bodyFile, s32 labelA, const char* fromFile, s32 fromLabel)
{
    m_pToWord = to;
    m_pBodyWord = body;
    m_pFromWord = from;
    m_pToResult = toResult;
    m_ToFile.copy(sead::SafeStringBase<char>(toFile));
    m_BodyFile.copy(sead::SafeStringBase<char>(bodyFile));
    m_FromFile.copy(sead::SafeStringBase<char>(fromFile));
    m_Mode = MODE_NORMAL;
    m_Labels[2] = -1;
    m_Labels[3] = -1;
    m_Status = 0;
    m_Labels[4] = fromLabel;
    m_Labels[0] = toLabel;
    m_Labels[1] = labelA;
}

// 0x005DDED0 (name is ours)
void IMailRecept::SetupBbs(IWord* body, const char* bodyFile, s32 labelA)
{
    m_pToWord = NULL;
    m_pBodyWord = body;
    m_pFromWord = NULL;
    m_pToResult = NULL;
    m_ToFile.clear();
    m_BodyFile.copy(sead::SafeStringBase<char>(bodyFile));
    m_FromFile.clear();
    m_Labels[0] = -1;
    m_Labels[1] = labelA;
    m_Labels[2] = -1;
    m_Labels[3] = -1;
    m_Labels[4] = -1;
    m_Mode = MODE_BBS;
    m_Status = 0;
}
} // namespace script
