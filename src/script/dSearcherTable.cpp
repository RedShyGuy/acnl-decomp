#include "script/dSearcher.h"

// The handlers of the modes of the Searcher (its own file: GCC simplifies the calls through the tables otherwise; name of
// the file is ours).

namespace script {
// 0x00891ED0
const Searcher::CharFunc Searcher::s_CharFuncs[4] = {
    &Searcher::OnCharFind,
    &Searcher::OnCharReturn,
    &Searcher::OnCharNotReturn,
    NULL,
};
// 0x00891EF0
const Searcher::TagFunc Searcher::s_TagFuncs[4] = {
    NULL,
    NULL,
    &Searcher::OnTagGroup,
    &Searcher::OnTagFind,
};
} // namespace script
