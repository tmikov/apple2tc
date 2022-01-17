# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

add_library(support STATIC
  support.cpp ${A2TC_INC}/support.h
  ${A2TC_INC}/ArrayRef.h
  ${A2TC_INC}/BitSet.h
  ${A2TC_INC}/CircularList.h
  ${A2TC_INC}/IteratorRange.h
  )

