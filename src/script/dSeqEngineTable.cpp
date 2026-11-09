#include "script/dSeqEngine.h"

// The tables of the grammar tags of SeqEngine (a file of their own, else GCC folds them into the code).

namespace script {
// 0x00891744
const char16* const SeqEngine::s_EpenthesisTexts[2] = {u"ad", u"ed"};
// 0x0089174C
const char16* const SeqEngine::s_ConjunctionTexts[2] = {u"a", u"e"};

// 0x00892144
const SeqEngine::TagId SeqEngine::s_GenderExTags[38] = {
    {5, 10},
    {5, 14},
    {5, 15},
    {5, 16},
    {5, 17},
    {5, 18},
    {5, 19},
    {5, 20},
    {5, 21},
    {5, 29},
    {5, 31},
    {5, 33},
    {5, 34},
    {17, 0},
    {17, 1},
    {17, 2},
    {17, 3},
    {17, 4},
    {17, 5},
    {17, 6},
    {17, 7},
    {17, 8},
    {17, 9},
    {18, 0},
    {18, 3},
    {18, 4},
    {18, 5},
    {18, 6},
    {18, 10},
    {18, 11},
    {18, 13},
    {18, 14},
    {18, 17},
    {18, 18},
    {18, 19},
    {18, 22},
    {18, 23},
    {18, 24},
};
} // namespace script
