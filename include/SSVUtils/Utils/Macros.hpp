// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_MACROS
#define SSVU_UTILS_MACROS

#include "SSVUtils/Preprocessor/Preprocessor.hpp"

namespace ssvu
{
	#define SSVU_DEFINE_DUMMY_STRUCT(...) struct SSVPP_CAT(__dummyStruct, __VA_ARGS__, __LINE__) { } __attribute__ ((unused))

	/// @macro Define a template class with name `name` that checks if a certain type T has
	/// a member of name `mMemberName`.
	/// @code
	/// struct Example { void testMethod() { } };
	/// SSVU_DEFINE_HAS_MEMBER_CHECKER(HasTestMethod, testMethod);
	/// SSVU_DEFINE_HAS_MEMBER_INVOKER(callTestMethod, testMethod, (HasTestMethod<T, void()>::Value));
	/// Example example{};
	/// callTestMethod(example);
	/// @endcode
	#define SSVU_DEFINE_HAS_MEMBER_CHECKER(mName, mMemberName) \
		template<typename, typename T> class mName; \
		template<typename C, typename TReturn, typename... TArgs> class mName<C, TReturn(TArgs...)> \
		{ \
			template<typename T> static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().mMemberName(std::declval<TArgs>()...)), TReturn>::type { return {}; } \
			template<typename> static constexpr std::false_type check(...) { return {}; } \
			public: static constexpr bool Value = decltype(check<C>(0))::value; \
		} \

	/// @macro Define a template function with name `name` that invokes `mMemberName` on objects
	/// if those objects have a `mMemberName` member, otherwise does nothing. Must be used with
	/// a valid `mChecker`, which must be a previously defined `SSVU_DEFINE_HAS_MEMBER_CHECKER`.
	#define SSVU_DEFINE_HAS_MEMBER_INVOKER(mName, mMemberName, mChecker) \
		namespace _ssvuMacroImpl { \
			template<typename T, bool TCheck, typename... TArgs> struct _ ## mName ## Impl; \
			template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, true, TArgs...> { inline static void call(T& mArg, TArgs&&... mArgs) { mArg.mMemberName(std::forward<TArgs>(mArgs)...); } }; \
			template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, false, TArgs...> { inline static void call(T&, TArgs&&...) { } }; } \
			template<typename T, typename... TArgs> inline void mName(T& mArg, TArgs&&... mArgs) { _ssvuMacroImpl::_ ## mName ## Impl<T, mChecker, TArgs...>::call(mArg, std::forward<TArgs>(mArgs)...); \
		} \
		SSVU_DEFINE_DUMMY_STRUCT(mName, mMemberName)

	#ifdef __clang__
		/// @macro When compiling with clang, using `assert` in constexpr functions seems to be allowed.
		#define SSVU_CONSTEXPR_ASSERT(mTest) assert(mTest)
	#else
		/// @macro When compiling with g++, using `assert` in constexpr functions causes an error.
		#define SSVU_CONSTEXPR_ASSERT(mTest)
	#endif
}

#endif
