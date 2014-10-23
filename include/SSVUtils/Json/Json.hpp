// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON
#define SSVU_JSON

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Container/Container.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/UnionVariant/UnionVariant.hpp"

#include "SSVUtils/Json/Common/Common.hpp"
#include "SSVUtils/Json/Num/Num.hpp"
#include "SSVUtils/Json/Val/Val.hpp"
#include "SSVUtils/Json/Io/Io.hpp"
#include "SSVUtils/Json/Val/Val.inl"
#include "SSVUtils/Json/Io/Writer.inl"
#include "SSVUtils/Json/Val/Internal/CnvFuncs.hpp"
#include "SSVUtils/Json/Stringifier/Stringifier.hpp"

#endif

// TODO: bottleneck: readStr reserve
//       don't use exceptions and noexcept everything? (?)
//       dispatch iteration based on types for obj and arr
//       docs
