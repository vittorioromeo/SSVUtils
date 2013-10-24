// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_MACROS
#define SSVU_UTILS_MACROS

	#define SSVU_DEFINE_HAS_MEMBER_CHECKER(name, memberName) \
		template<typename, typename T> class name; \
		template<typename C, typename TReturn, typename... TArgs> class name<C, TReturn(TArgs...)> \
		{ \
			template<typename T> static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().memberName(std::declval<TArgs>()...)), TReturn>::type { return {}; }; \
			template<typename> static constexpr std::false_type check(...) { return {}; } \
			public: static constexpr bool Value = decltype(check<C>(0))::value; \
		} \

	#define SSVU_DEFINE_HAS_MEMBER_INVOKER(name, memberName, checker) \
		namespace _ssvuMacroImpl { \
		template<typename T, bool TCheck, typename... TArgs> struct _ ## name ## Impl; \
		template<typename T, typename... TArgs> struct _ ## name ## Impl<T, true, TArgs...> { inline static void call(T& mArg, TArgs&&... mArgs) { mArg.memberName(std::forward<TArgs>(mArgs)...); } }; \
		template<typename T, typename... TArgs> struct _ ## name ## Impl<T, false, TArgs...> { inline static void call(T&, TArgs&&...) { } }; } \
		template<typename T, typename... TArgs> inline void name(T& mArg, TArgs&&... mArgs)	{ _ssvuMacroImpl::_ ## name ## Impl<T, checker, TArgs...>::call(mArg, std::forward<TArgs>(mArgs)...); } \
		struct _dummy ## name ## memberName { }

#endif
