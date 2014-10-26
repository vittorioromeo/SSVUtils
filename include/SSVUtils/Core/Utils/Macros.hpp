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

		template<typename T> using StandardLayoutCheckerT = typename ssvu::Internal::StandardLayoutChecker<T>::Type;

		template<typename... > struct Voider { using Type = void; };
		template<typename... TArgs> using VoidT = typename Voider<TArgs...>::Type;
	}
}

/// @macro Defines a dummy struct with a name generated from the current line and passed variadic args.
/// @details Must end with semicolon.
#define SSVU_DEFINE_DUMMY_STRUCT(...) struct SSVPP_CAT(__dummyStruct, __VA_ARGS__, __LINE__) { } __attribute__ ((unused))

/// @macro Define a template class with name `mName` that checks if a certain type T has a member of name `mMemberName`.
/// @code
/// struct Example { void testMethod() { } };
/// SSVU_DEFINE_MEMFN_DETECTOR(hasTestMethod, testMethod)
/// SSVU_DEFINE_MEMFN_CALLER_IMPL(callTestMethod, testMethod, (hasTestMethod<T, void()>()));
/// Example example{};
/// callTestMethod(example);
/// @endcode
/// @details Must end without semicolon.
#define SSVU_DEFINE_MEMFN_DETECTOR(mName, mMemberName) \
	template<typename, typename T> struct SSVPP_CAT(__, mName, __impl); \
	template<typename TC, typename TReturn, typename... TArgs> struct SSVPP_CAT(__, mName, __impl)<TC, TReturn(TArgs...)> \
	{ \
		template<typename T> inline static constexpr auto check(T*) -> ssvu::IsSame<decltype(std::declval<T>().mMemberName(std::declval<TArgs>()...)), TReturn> { return {}; } \
		template<typename> inline static constexpr std::false_type check(...) { return {}; } \
		static constexpr bool value{decltype(check<TC>(0))::value}; \
	}; \
	template<typename T, typename TSignature> inline constexpr bool mName() noexcept { return SSVPP_CAT(__, mName, __impl) < T, TSignature > :: value; }

/// @macro Define a template function with name `mName` that invokes `mMemberName` on objects
/// if those objects have a `mMemberName` member, otherwise does nothing. Must be used with
/// a valid `mChecker`, which must be a previously defined `SSVU_DEFINE_MEMFN_DETECTOR`.
/// @details Must end without semicolon.
#define SSVU_DEFINE_MEMFN_CALLER_IMPL(mName, mMemberName, mChecker) \
	namespace __ssvuMacroImpl \
	{ \
		template<typename T, bool TCheck, typename... TArgs> struct _ ## mName ## Impl; \
		template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, true, TArgs...> \
		{ \
			inline static auto call(T& mArg, TArgs&&... mArgs) \
			{ \
				return mArg.mMemberName(ssvu::fwd<TArgs>(mArgs)...); \
			} \
		}; \
		template<typename T, typename... TArgs> struct _ ## mName ## Impl<T, false, TArgs...> \
		{ \
			inline static void call(T&, TArgs&&...) { } \
		}; \
	} \
	template<typename T, typename... TArgs> inline auto mName(T& mArg, TArgs&&... mArgs) \
	{ \
		return __ssvuMacroImpl::_ ## mName ## Impl<T, mChecker, TArgs...>::call(mArg, ssvu::fwd<TArgs>(mArgs)...); \
	}

/// @macro Define a template function with name `mName` that invokes `mMemberName` on objects
/// if those objects have a `mMemberName` member, otherwise does nothing.
/// @code
/// struct Example { void testMethod() { } };
/// SSVU_DEFINE_MEMFN_CALLER(callTestMethod, testMethod, (void()))
/// Example example{};
/// callTestMethod(example);
/// @endcode
/// @details Must end without semicolon.
#define SSVU_DEFINE_MEMFN_CALLER(mName, mMemberName, mSignature) \
	SSVU_DEFINE_MEMFN_DETECTOR(SSVPP_CAT(__ssvuInvoker, mName, mMemberName, __LINE__), mMemberName) \
	SSVU_DEFINE_MEMFN_CALLER_IMPL(mName, mMemberName, ( SSVPP_CAT(__ssvuInvoker, mName, mMemberName, __LINE__)<T, mSignature>() ))

/// @macro Gets the base `mType*` structure pointer from a `mMemberPointer` pointer to a member of `mType`, with member name `mMemberName`. Const version.
/// @details Requires `mType` to be a standard-layout type. Uses offsetof(...) internally.
/// Assumes that `mMemberPtr` actually points to an inner member of an existing `mType` instance.
/// Must end with semicolon.
#define SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(mType, mMemberPtr, mMemberName) \
	reinterpret_cast<const ssvu::Internal::StandardLayoutCheckerT<mType>*>(reinterpret_cast<const char*>(mMemberPtr) - offsetof(mType, mMemberName))

/// @macro Gets the base `mType*` structure pointer from a `mMemberPointer` pointer to a member of `mType`, with member name `mMemberName`.
/// @details Requires `mType` to be a standard-layout type. Uses offsetof(...) internally.
/// Assumes that `mMemberPtr` actually points to an inner member of an existing `mType` instance.
/// Must end with semicolon.
#define SSVU_GET_BASEPTR_FROM_MEMBERPTR(mType, mMemberPtr, mMemberName) \
	const_cast<mType*>(SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(mType, mMemberPtr, mMemberName))

/// @macro Defines a basic "sink" setter for a class field.
/// @details Generates two functions, one that takes the parameter by && and moves it, and another that takes it by const&.
/// Must end without semicolon.
#define SSVU_DEFINE_SINK_SETTER_SIMPLE(mName, mMember) \
	inline void mName(const decltype(mMember)& mParam)		{ mMember = mParam; } \
	inline void mName(decltype(mMember)&& mParam) noexcept	{ mMember = std::move(mParam); }

/// @macro Defines a basic "sink" constructor for a class, taking one argument.
/// @details Generates two constructors.
/// Must end without semicolon.
#define SSVU_DEFINE_SINK_CTOR_SIMPLE_1(mClassName, mMember) \
	inline mClassName(const decltype(mMember)& mParam)		: mMember{mParam} { } \
	inline mClassName(decltype(mMember)&& mParam) noexcept	: mMember{std::move(mParam)} { }

/// @macro Defines a basic "sink" constructor for a class, taking two arguments.
/// @details Generates four constructors.
/// Must end without semicolon.
#define SSVU_DEFINE_SINK_CTOR_SIMPLE_2(mClassName, mMember1, mMember2) \
	inline mClassName(const decltype(mMember1)& mParam1,	const decltype(mMember2)& mParam2)		: mMember1{mParam1},			mMember2{mParam2} { } \
	inline mClassName(const decltype(mMember1)& mParam1,	decltype(mMember2)&& mParam2)			: mMember1{mParam1},			mMember2{std::move(mParam2)} { } \
	inline mClassName(decltype(mMember1)&& mParam1,			const decltype(mMember2)& mParam2)		: mMember1{std::move(mParam1)},	mMember2{mParam2} { } \
	inline mClassName(decltype(mMember1)&& mParam1,			decltype(mMember2)&& mParam2) noexcept	: mMember1{std::move(mParam1)},	mMember2{std::move(mParam2)} { }

/// @macro Uses SFINAE to enable/disable a particular template. Place this macro in the template arguments list.
/// @details Enables if `mT` has the same type of `mType`. Uses `RemoveAll` on the passed type.
#define SSVU_ENABLEIF_RA_IS(mT, mType) EnableIf<isSame<RemoveAll<mT>, mType>()>* = nullptr

/// @macro Uses SFINAE to enable/disable a particular template. Place this macro in the template arguments list.
/// /// @details Enables if `mT` has not the same type of `mType`. Uses `RemoveAll` on the passed type.
#define SSVU_ENABLEIF_RA_IS_NOT(mT, mType) EnableIf<!isSame<RemoveAll<mT>, mType>()>* = nullptr

// Unreachable macro
#if (defined(SSVU_COMPILER_CLANG) || defined(SSVU_COMPILER_GCC))
	/// @macro Unreachable code. Uses `__builtin_unreachable`. Requires semicolon at the end.
	#define SSVU_UNREACHABLE() __builtin_unreachable()
#else
	/// @macro Unreachable code. Uses an assert and `std::terminate()`. Requires semicolon at the end.
	#define SSVU_UNREACHABLE() do { SSVU_ASSERT(false); std::terminate(); } while(false)
#endif

#endif

// TODO: void_t callers?
