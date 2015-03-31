// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_TPLFORHELPER
#define SSVU_CORE_TUPLE_INTERNAL_TPLFORHELPER

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"

#define SSVU_IMPL_DEFINE_FORHELPER(mName, mBody) \
	template<SizeT TS, typename... TTpls> struct mName \
	{ \
		template<SizeT TI, typename TF> inline static EnableIf<TI == TS, void> exec(TF, TTpls&&...) noexcept { } \
		\
		template<SizeT TI = 0, typename TF> inline static EnableIf<TI < TS, void> exec(TF&& mF, TTpls&&... mTpls) \
			noexcept(noexcept(mBody)) \
		{ \
			mBody ; \
			exec<TI + 1, TF>(FWD(mF), FWD(mTpls)...); \
		} \
	};

namespace ssvu
{
	namespace Impl
	{
		template<typename TTypes, SizeT TIdx> struct TplForData
		{
			using Types = TTypes;
			static constexpr SizeT idx{TIdx};
		};

		SSVU_IMPL_DEFINE_FORHELPER(ForHelper,		(FWD(mF)(std::get<TI>(FWD(mTpls))...)))
		SSVU_IMPL_DEFINE_FORHELPER(ForDataHelper,	(FWD(mF)(TplForData<MPL::List<decltype(std::get<TI>(FWD(mTpls)))...>, TI>{}, std::get<TI>(FWD(mTpls))...)))
	}

	template<typename T> inline constexpr auto getIdx(const T&) noexcept { return T::idx; }
}

#undef SSVU_IMPL_DEFINE_FORHELPER

#endif
