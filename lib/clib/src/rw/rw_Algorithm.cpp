// std::sort of the C++ library (Rogue Wave): the instance of the introsort loop for int (ARM code in the original), used
// by pia and imgdb. See rw/rw_Algorithm.h.

#include "rw/rw_Algorithm.h"

namespace std {
// 0x00837BA4 | fefates:bytes [tier B]
template void __introsort_loop<int*, int, __rw::__rw_lt<int> >(int* first, int* last, int maxDepth,
                                                              __rw::__rw_lt<int> compare);
} // namespace std
