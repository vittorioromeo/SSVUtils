// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE
#define SSVU_CORE_TUPLE

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"
#include "SSVUtils/Core/Tuple/Internal/CTMin.hpp"
#include "SSVUtils/Core/Tuple/Internal/Exploder.hpp"
#include "SSVUtils/Core/Tuple/Internal/TplForHelper.hpp"
#include "SSVUtils/Core/Tuple/Internal/TplIdxOfHelper.hpp"
#include "SSVUtils/Core/Tuple/Internal/TplRepeatHelper.hpp"
#include "SSVUtils/Core/Tuple/TplFor.hpp"

namespace ssvu
{
	/// @brief Type of a tuple with `T` repeated `TN` times.
	/// @code
	///	ssvu::TplRepeat<int, 5>
	/// // ...is equal to...
	/// std::tuple<int, int, int, int, int>
	/// @endcode
	template<typename T, SizeT TN, typename TIdxs = IdxSeq<TN>>
	using TplRepeat = typename Internal::TplRepeatImpl<T, TN>::Type;

	/// @brief Returns the index of the first occurrence of the type `T` in a tuple.
	template<typename T, typename TTpl> inline constexpr auto getTplIdxOf() noexcept
	{
		return Internal::TplIdxOf<T, TTpl>::value;
	}

	/// @brief Calls the function `mF` using `mT`'s values as arguments.
	template<typename TF, typename T> inline auto explode(TF&& mF, T&& mT)
		noexcept(noexcept(Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT))))
	{
		return Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT));
	}
}

#endif
