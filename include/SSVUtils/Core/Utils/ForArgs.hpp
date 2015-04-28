// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_FORARGS
#define SSVU_CORE_UTILS_FORARGS

#include "SSVUtils/Core/Common/Aliases.hpp"

// Implemented thanks to Daniel Frey:
// http://stackoverflow.com/a/29901074/598696

namespace ssvu
{
	namespace Impl
	{
		template<typename, typename> struct ForNArgsHlpr;

		template<SizeT... Bs, SizeT... Cs> struct ForNArgsHlpr<IdxSeq<Bs...>, IdxSeq<Cs...>>
		{
			using Swallow = bool[];

			#define IMPL_IMPL_FORNARGS_EXECN_BODY() \
				FWD(mFn)(std::get<TArity + Cs>(FWD(mXs))...)

			template<SizeT TArity, typename TF, typename TTpl, typename... Ts>
			inline static constexpr void execN(TF&& mFn, TTpl&& mXs)
				noexcept(noexcept(IMPL_IMPL_FORNARGS_EXECN_BODY()))
			{
				IMPL_IMPL_FORNARGS_EXECN_BODY();
			}

			#undef IMPL_IMPL_FORNARGS_EXECN_BODY

			#define IMPL_IMPL_FORNARGS_EXEC_BODY() \
				(void) Swallow{(execN<(Bs * sizeof...(Cs))>(FWD(mFn), FWD(mXs)), true)..., true}

			template<typename TF, typename TTpl, typename... Ts>
			inline static constexpr void exec(TF&& mFn, TTpl&& mXs)
				noexcept(noexcept(IMPL_IMPL_FORNARGS_EXEC_BODY()))
			{
				IMPL_IMPL_FORNARGS_EXEC_BODY();
			}

			#undef IMPL_IMPL_FORNARGS_EXEC_BODY
		};
	}

	#define IMPL_FORNARGS_BODY() \
		Impl::ForNArgsHlpr<MkIdxSeq<sizeof...(Ts) / TArity>, MkIdxSeq<TArity>>::exec(FWD(mFn), fwdAsTpl(FWD(mXs)...))

	template<SizeT TArity = 1, typename TF, typename... Ts>
	inline constexpr void forArgs(TF&& mFn, Ts&&... mXs)
		noexcept(noexcept(IMPL_FORNARGS_BODY()))
	{
		SSVU_ASSERT_STATIC(sizeof...(Ts) % TArity == 0, "Unallowed arity: not divisible by number of arguments");
		IMPL_FORNARGS_BODY();
	}

	#undef IMPL_FORNARGS_BODY
}

#endif
