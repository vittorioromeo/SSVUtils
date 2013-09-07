#ifndef SSVU_FASTFUNC
#define SSVU_FASTFUNC

#include <cstring>
#include <type_traits>
#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

namespace ssvu
{
	namespace Internal
	{
		class AnyClass;
		using AnyMemFunc = void(Internal::AnyClass::*)();

		constexpr std::size_t SingleMemFuncPtrSize{sizeof(void(AnyClass::*)())};

		template<class TOut, class TIn> union HorribleUnion { TOut out; TIn in; };
		template<class TOut, class TIn> inline TOut horrible_cast(TIn mIn) noexcept { HorribleUnion<TOut, TIn> u; static_assert(sizeof(TIn) == sizeof(u) && sizeof(TIn) == sizeof(TOut), "Cannot use horrible_cast<>"); u.in = mIn; return u.out; }
		template<class TOut, class TIn> inline TOut unsafe_horrible_cast(TIn mIn) noexcept { HorribleUnion<TOut, TIn> u; u.in = mIn; return u.out; }

		template<std::size_t N> struct SimplifyMemFunc
		{
			template<class X, class XFuncType, class AnyMemFunc> inline static AnyClass* convert(const X*, XFuncType, AnyMemFunc&) noexcept
			{
				static_assert(N - 100, "Unsupported member function pointer on this compiler");
				return 0;
			}
		};
		template<> struct SimplifyMemFunc<SingleMemFuncPtrSize>
		{
			template<class X, class XFuncType, class AnyMemFunc> inline static AnyClass* convert(const X* mPtrThis, XFuncType mFuncToBind, AnyMemFunc& mFuncBound) noexcept
			{
				mFuncBound = reinterpret_cast<AnyMemFunc>(mFuncToBind);
				return reinterpret_cast<AnyClass*>(const_cast<X*>(mPtrThis));
			}
		};

		template<class TAnyMemFunc, class TStaticFunc> struct ClosurePtr
		{
			private:
				AnyClass* ptrThis{nullptr};
				AnyMemFunc ptrFunction{nullptr};

			public:
				inline ClosurePtr() noexcept = default;
				inline ClosurePtr(std::nullptr_t) noexcept { }
				inline ClosurePtr(const ClosurePtr& mClosurePtr) noexcept : ptrThis{mClosurePtr.ptrThis}, ptrFunction{mClosurePtr.ptrFunction} { }
				inline ClosurePtr(ClosurePtr&& mClosurePtr) noexcept : ptrThis{std::move(mClosurePtr.ptrThis)}, ptrFunction{std::move(mClosurePtr.ptrFunction)} { mClosurePtr = nullptr; }

				inline ClosurePtr& operator=(std::nullptr_t) noexcept			{ ptrThis = nullptr; ptrFunction = nullptr; return *this; }
				inline ClosurePtr& operator=(const ClosurePtr& mRhs) noexcept	{ ptrThis = mRhs.ptrThis; ptrFunction = mRhs.ptrFunction; return *this; }
				inline ClosurePtr& operator=(ClosurePtr&& mRhs) noexcept		{ ptrThis = std::move(mRhs.ptrThis); ptrFunction = std::move(mRhs.ptrFunction); return *this; }

				template<class X, class XMemFunc> inline void bindMemFunc(X* mPtrThis, XMemFunc mFuncToBind) noexcept { ptrThis = SimplifyMemFunc<sizeof(mFuncToBind)>::convert(mPtrThis, mFuncToBind, ptrFunction); }
				template<class DerivedClass, class ParentInvokerSig> inline void bindStaticFunc(DerivedClass* mPtrParent, ParentInvokerSig mStaticFuncInvoker, TStaticFunc mFuncToBind) noexcept
				{
					static_assert(sizeof(AnyClass*) == sizeof(mFuncToBind), "Cannot use horrible_cast");
					if(mFuncToBind == nullptr) ptrFunction = nullptr; else bindMemFunc(mPtrParent, mStaticFuncInvoker);
					ptrThis = horrible_cast<AnyClass*>(mFuncToBind);
				}

				inline bool operator==(std::nullptr_t) const noexcept			{ return ptrThis == nullptr && ptrFunction == nullptr; }
				inline bool operator==(const ClosurePtr& mRhs) const noexcept	{ return ptrThis == mRhs.ptrThis && ptrFunction == mRhs.ptrFunction; }
				inline bool operator==(TStaticFunc mPtr) const noexcept			{ return mPtr == nullptr ? *this == nullptr : mPtr == reinterpret_cast<TStaticFunc>(getStaticFunc()); }
				inline bool operator!=(std::nullptr_t) const noexcept			{ return !operator==(nullptr); }
				inline bool operator!=(const ClosurePtr& mRhs) const noexcept	{ return !operator==(mRhs); }
				inline bool operator!=(TStaticFunc mPtr) const noexcept			{ return !operator==(mPtr); }
				inline bool operator<(const ClosurePtr& mRhs) const				{ return ptrThis != mRhs.ptrThis ? ptrThis < mRhs.ptrThis : std::memcmp(&ptrFunction, &mRhs.ptrFunction, sizeof(ptrFunction)) < 0; }
				inline bool operator>(const ClosurePtr& mRhs) const				{ return mRhs < *this; }

				inline std::size_t getHash() const noexcept						{ return reinterpret_cast<std::size_t>(ptrThis) ^ Internal::unsafe_horrible_cast<std::size_t>(ptrFunction); }
				inline AnyClass* getPtrThis() const noexcept					{ return ptrThis; }
				inline TAnyMemFunc getPtrFunction() const noexcept				{ return reinterpret_cast<TAnyMemFunc>(ptrFunction); }
				inline TStaticFunc getStaticFunc() const noexcept				{ static_assert(sizeof(TStaticFunc) == sizeof(this), "Cannot use horrible_cast"); return horrible_cast<TStaticFunc>(this); }
		};

		template<typename TReturn, typename... TArgs> class FastFuncImpl
		{
			protected:
				using GenericMemFn = TReturn(Internal::AnyClass::*)(TArgs...);
				using FuncSig = TReturn(*)(TArgs...);
				using ClosureType = Internal::ClosurePtr<GenericMemFn, FuncSig>;
				ClosureType closure;

			private:
				template<typename X> using XFuncToBind = TReturn(X::*)(TArgs...) const;
				inline TReturn invokeStaticFunc(TArgs... mArgs) const { return (*(closure.getStaticFunc()))(std::forward<TArgs>(mArgs)...); }
				inline void bind(FuncSig mFuncToBind) noexcept { closure.bindStaticFunc(this, &FastFuncImpl::invokeStaticFunc, mFuncToBind); }
				template<typename X, typename Y> inline void bind(Y* mPtrThis, XFuncToBind<X> mFuncToBind) noexcept { closure.bindMemFunc(reinterpret_cast<const X*>(mPtrThis), mFuncToBind); }

			public:
				inline FastFuncImpl() noexcept = default;
				inline FastFuncImpl(std::nullptr_t) noexcept { }
				inline FastFuncImpl(const FastFuncImpl& mImpl) noexcept : closure{mImpl.closure} { }
				inline FastFuncImpl(FastFuncImpl&& mImpl) noexcept : closure{std::move(mImpl.closure)} { }
				inline FastFuncImpl(FuncSig mFuncToBind) noexcept { bind(mFuncToBind); }
				template<typename X, typename Y> inline FastFuncImpl(Y* mPtrThis, XFuncToBind<X> mFuncToBind) noexcept { bind(mPtrThis, mFuncToBind); }

				inline void operator=(const FastFuncImpl& mImpl) noexcept	{ closure = mImpl.closure; }
				inline void operator=(FastFuncImpl&& mImpl) noexcept		{ closure = std::move(mImpl.closure); }
				inline void operator=(FuncSig mFuncToBind) noexcept			{ bind(mFuncToBind); }
				inline TReturn operator()(TArgs... mArgs) const				{ return (closure.getPtrThis()->*(closure.getPtrFunction()))(std::forward<TArgs>(mArgs)...); }

				inline bool operator==(std::nullptr_t) const noexcept				{ return closure == nullptr; }
				inline bool operator==(const FastFuncImpl& mImpl) const noexcept	{ return closure == mImpl.closure; }
				inline bool operator==(FuncSig mFuncPtr) const noexcept				{ return closure == mFuncPtr; }
				inline bool operator!=(std::nullptr_t) const noexcept				{ return !operator==(nullptr); }
				inline bool operator!=(const FastFuncImpl& mImpl) const noexcept	{ return !operator==(mImpl); }
				inline bool operator!=(FuncSig mFuncPtr) const noexcept				{ return !operator==(mFuncPtr); }
				inline bool operator<(const FastFuncImpl& mImpl) const				{ return closure < mImpl.closure; }
				inline bool operator>(const FastFuncImpl& mImpl) const				{ return closure > mImpl.closure; }
		};
	}
	template<typename TFunc> struct MemFuncPtrToFuncPtr;
	template<typename TReturn, typename TClass, typename... TArgs> struct MemFuncPtrToFuncPtr<TReturn(TClass::*)(TArgs...) const> { using type = TReturn(*)(TArgs...); };

	template<typename TFunc> struct is_fun_ptr : std::integral_constant<bool, std::is_pointer<TFunc>::value && std::is_function<typename std::remove_pointer<TFunc>::type>::value> { };
	template<typename TFunc> struct is_trivial : std::integral_constant<bool, std::is_constructible<typename MemFuncPtrToFuncPtr<decltype(&std::decay<TFunc>::type::operator())>::type, TFunc>::value> { };

	#define ENABLEIF_FUNCPTR(x)					typename std::enable_if<is_fun_ptr<x>::value>::type* = nullptr
	#define ENABLEIF_CONVERTIBLETOFUNCPTR(x)	typename std::enable_if<std::is_constructible<typename MemFuncPtrToFuncPtr<decltype(&std::decay<x>::type::operator())>::type, x>::value>::type* = nullptr
	#define ENABLEIF_NOTCONVERTIBLETOFUNCPTR(x) typename std::enable_if<!std::is_constructible<typename MemFuncPtrToFuncPtr<decltype(&std::decay<x>::type::operator())>::type, x>::value>::type* = nullptr
	#define ENABLEIF_ISSAMETYPE(x, y)			typename std::enable_if<!std::is_same<x, typename std::decay<y>::type>{}>::type

	template<typename TSignature> class FastFunc;
	template<typename TReturn, typename... TArgs> class FastFunc<TReturn(TArgs...)> : public Internal::FastFuncImpl<TReturn, TArgs...>
	{
		private:
			using BaseType = Internal::FastFuncImpl<TReturn, TArgs...>;
			std::shared_ptr<void> storage;
			template<typename T> inline static void funcDeleter(void* mPtr) { static_cast<T*>(mPtr)->~T(); operator delete(mPtr); }

		public:
			using BaseType::BaseType;

			inline FastFunc() noexcept = default;

			template<typename TFunc, typename = ENABLEIF_ISSAMETYPE(FastFunc, TFunc)> inline FastFunc(TFunc&& mFunc, ENABLEIF_CONVERTIBLETOFUNCPTR(TFunc))
			{
				using FuncType = typename std::decay<TFunc>::type;
				this->closure.bindMemFunc(&mFunc, &FuncType::operator());
			}
			template<typename TFunc, typename = ENABLEIF_ISSAMETYPE(FastFunc, TFunc)> inline FastFunc(TFunc&& mFunc, ENABLEIF_NOTCONVERTIBLETOFUNCPTR(TFunc))
				: storage(operator new(sizeof(TFunc)), funcDeleter<typename std::decay<TFunc>::type>)
			{
				using FuncType = typename std::decay<TFunc>::type;
				new (storage.get()) FuncType(std::forward<TFunc>(mFunc));
				this->closure.bindMemFunc(storage.get(), &FuncType::operator());
			}
	};

	#undef ENABLEIF_FUNCPTR
	#undef ENABLEIF_CONVERTIBLETOFUNCPTR
	#undef ENABLEIF_NOTCONVERTIBLETOFUNCPTR
	#undef ENABLEIF_ISSAMETYPE
}

#endif
