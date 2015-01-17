// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_TPLFOR
#define SSVU_CORE_TUPLE_TPLFOR

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"
#include "SSVUtils/Core/Tuple/Internal/CTMin.hpp"
#include "SSVUtils/Core/Tuple/Internal/TplForHelper.hpp"

#define SSVU_IMPL_TPLFOR_CALL(mHelper) \
	Internal:: mHelper < Internal::getCTMin<getTplSize<TTpls>()...>() , TTpls...>::exec(fwd<TF>(mF), fwd<TTpls>(mTpls)...)

#define SSVU_IMPL_DEFINE_TPLFOR_FN(mName, mHelper) \
	template<typename TF, typename... TTpls> inline void mName (TF&& mF, TTpls&&... mTpls) \
		noexcept(noexcept(SSVU_IMPL_TPLFOR_CALL(mHelper))) \
	{ \
		SSVU_IMPL_TPLFOR_CALL(mHelper); \
	}

namespace ssvu
{
	/// @brief Iterates over a tuple's elements passing them to `mF` one at a time.
	/// @details Can iterate over multiple tuples at once, passing the Nth element of every tuple to
	/// `mF` simultaneously.
	/// If the tuples have different sizes, the minimum size will be used.
	SSVU_IMPL_DEFINE_TPLFOR_FN(tplFor, ForHelper)

	/// @brief Iterates over a tuple's elements passing the current index and them to `mF` one at a time.
	/// @details Can iterate over multiple tuples at once, passing the Nth element of every tuple to
	/// `mF` simultaneously.
	/// If the tuples have different sizes, the minimum size will be used.
	SSVU_IMPL_DEFINE_TPLFOR_FN(tplForIdx, ForIdxHelper)
}

#undef SSVU_IMPL_TPLFOR_CALL
#undef SSVU_IMPL_DEFINE_TPLFOR_FN

#endif

// TODO: static assert that: arity of the passed function is equal to minsize(CTMin)
