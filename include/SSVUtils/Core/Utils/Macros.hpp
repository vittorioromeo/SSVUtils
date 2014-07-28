// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_MACROS
#define SSVU_CORE_UTILS_MACROS

namespace ssvu
{
	namespace Internal
	{
		/// @brief Internal structure with `isStandardLayout` static assertion used for "baseptr" macros.
		template<typename T> struct StandardLayoutChecker
		{
			SSVU_ASSERT_STATIC(isStandardLayout<T>(), "T must be a standard-layout type");
			using Type = T;
		};
	}
}

#define SSVU_DEFINE_DUMMY_STRUCT(...) struct SSVPP_CAT(__dummyStruct, __VA_ARGS__, __LINE__) { } __attribute__ ((unused))

/// @macro Define a template class with name `mName` that checks if a certain type T has a member of name `mMemberName`.
/// @code
/// struct Example { void testMethod() { } };
/// SSVU_DEFINE_MEMFN_DETECTOR(hasTestMethod, testMethod);
/// SSVU_DEFINE_MEMFN_CALLER_IMPL(callTestMethod, testMethod, (hasTestMethod<T, void()>()));
/// Example example{};
/// callTestMethod(example);
/// @endcode
#define SSVU_DEFINE_MEMFN_DETECTOR(mName, mMemberName) \
	template<typename, typename T> struct SSVPP_CAT(__, mName, __impl); \
	template<typename TC, typename TReturn, typename... TArgs> struct SSVPP_CAT(__, mName, __impl)<TC, TReturn(TArgs...)> \
	{ \
		template<typename T> inline static constexpr auto check(T*) -> ssvu::IsSame<decltype(std::declval<T>().mMemberName(std::declval<TArgs>()...)), TReturn> { return {}; } \
		template<typename> inline static constexpr std::false_type check(...) { return {}; } \
		static constexpr bool value{decltype(check<TC>(0))::value}; \
	}; \
	template<typename T, typename TSignature> inline constexpr bool mName() noexcept { return SSVPP_CAT(__, mName, __impl) < T, TSignature > :: value; } \
	SSVU_DEFINE_DUMMY_STRUCT(mName, mMemberName, SSVPP_CAT(__, mName, __impl))

/// @macro Define a template function with name `mName` that invokes `mMemberName` on objects
/// if those objects have a `mMemberName` member, otherwise does nothing. Must be used with
/// a valid `mChecker`, which must be a previously defined `SSVU_DEFINE_MEMFN_DETECTOR`.
#define SSVU_DEFINE_MEMFN_CALLER_IMPL(mName, mMemberName, mChecker) \
	namespace __ssvuMacroImpl \
	{ \
		template<typename T, bool TCheck, typename... TArgs> struct _ ## mName ## Impl; \
		template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, true, TArgs...> \
		{ \
			inline static auto call(T& mArg, TArgs&&... mArgs) -> decltype(mArg.mMemberName(std::forward<TArgs>(mArgs)...)) \
			{ \
				return mArg.mMemberName(std::forward<TArgs>(mArgs)...); \
			} \
		}; \
		template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, false, TArgs...> \
		{ \
			inline static void call(T&, TArgs&&...) { } \
		}; \
	} \
	template<typename T, typename... TArgs> inline auto mName(T& mArg, TArgs&&... mArgs) \
		-> decltype(__ssvuMacroImpl::_ ## mName ## Impl<T, mChecker, TArgs...>::call(mArg, std::forward<TArgs>(mArgs)...)) \
	{ \
		return __ssvuMacroImpl::_ ## mName ## Impl<T, mChecker, TArgs...>::call(mArg, std::forward<TArgs>(mArgs)...); \
	} \
	SSVU_DEFINE_DUMMY_STRUCT(mName, mMemberName)

/// @macro Define a template function with name `mName` that invokes `mMemberName` on objects
/// if those objects have a `mMemberName` member, otherwise does nothing.
/// @code
/// struct Example { void testMethod() { } };
/// SSVU_DEFINE_MEMFN_CALLER(callTestMethod, testMethod, (void()));
/// Example example{};
/// callTestMethod(example);
/// @endcode
#define SSVU_DEFINE_MEMFN_CALLER(mName, mMemberName, mSignature) \
	SSVU_DEFINE_MEMFN_DETECTOR(SSVPP_CAT(__ssvuInvoker, mName, mMemberName, __LINE__), mMemberName); \
	SSVU_DEFINE_MEMFN_CALLER_IMPL(mName, mMemberName, ( SSVPP_CAT(__ssvuInvoker, mName, mMemberName, __LINE__)<T, mSignature>() ))

/// @macro Gets the base `mType*` structure pointer from a `mMemberPointer` pointer to a member of `mType`, with member name `mMemeberName`. Const version.
/// @details Requires `mType` to be a standard-layout type. Uses offsetof(...) internally.
/// Assumes that `mMemberPtr` actually points to an inner member of an existing `mType` instance.
#define SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(mType, mMemberPtr, mMemberName) \
	reinterpret_cast<const typename ssvu::Internal::StandardLayoutChecker<mType>::Type*>(reinterpret_cast<const char*>(mMemberPtr) - offsetof(mType, mMemberName))

/// @macro Gets the base `mType*` structure pointer from a `mMemberPointer` pointer to a member of `mType`, with member name `mMemeberName`.
/// @details Requires `mType` to be a standard-layout type. Uses offsetof(...) internally.
/// Assumes that `mMemberPtr` actually points to an inner member of an existing `mType` instance.
#define SSVU_GET_BASEPTR_FROM_MEMBERPTR(mType, mMemberPtr, mMemberName) \
	const_cast<mType*>(SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(mType, mMemberPtr, mMemberName))

#endif
