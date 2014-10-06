// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_SMARTPOINTERS
#define SSVU_CORE_COMMON_SMARTPOINTERS

namespace ssvu
{
	/// @brief Creates and returns an `ssvu::UPtr<T>`.
	/// @details Wraps `std::make_unique<T>`.
	template<typename T, typename... TArgs> inline decltype(auto) makeUPtr(TArgs&&... mArgs) { return std::make_unique<T>(fwd<TArgs>(mArgs)...); }

	/// @brief Creates and returns an `ssvu::SPtr<T>`.
	/// @details Wraps `std::make_shared<T>`.
	template<typename T, typename... TArgs> inline decltype(auto) makeSPtr(TArgs&&... mArgs) { return std::make_shared<T>(fwd<TArgs>(mArgs)...); }

	namespace Internal
	{
		/// @brief Internal functor that creates an `ssvu::UPtr`.
		template<typename T> struct MakerUPtr { template<typename... TArgs> inline static auto make(TArgs&&... mArgs) { return makeUPtr<T>(fwd<TArgs>(mArgs)...); } };

		/// @brief Internal functor that creates an `ssvu::SPtr`.
		template<typename T> struct MakerSPtr { template<typename... TArgs> inline static auto make(TArgs&&... mArgs) { return makeSPtr<T>(fwd<TArgs>(mArgs)...); } };
	}
}

#endif
