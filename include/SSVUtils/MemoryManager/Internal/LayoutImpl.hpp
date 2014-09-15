// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_LAYOUTIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_LAYOUTIMPL

namespace ssvu
{
	namespace Internal
	{
		namespace LayoutImpl
		{
			/// @brief Storage class for the alive/dead boolean and the item.
			template<typename T> struct LBool
			{
				AlignedStorageBasic<bool> storageBool;
				AlignedStorageBasic<T> storageItem;
			};

			/// @brief Storage class for the the item, without a bool.
			template<typename T> struct LNoBool
			{
				AlignedStorageBasic<T> storageItem;
			};

			template<typename TBase, template<typename> class TLT> struct LHelperBase
			{
				using TLType = TLT<TBase>;

				template<typename T> inline static char* allocate()								{ return new char[sizeof(TLT<T>)]; }
				inline static void deallocate(char* mPtr) noexcept								{ SSVU_ASSERT(mPtr != nullptr); delete[] mPtr; }
				inline static void destroy(TBase* mBase) noexcept(noexcept(mBase->~TBase()))	{ SSVU_ASSERT(mBase != nullptr); mBase->~TBase(); }

				inline static constexpr TLType* getLayout(TBase* mBase) noexcept				{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(TLType, mBase, storageItem); }
				inline static constexpr const TLType* getLayout(const TBase* mBase) noexcept	{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(TLType, mBase, storageItem); }
				inline static constexpr char* getByte(TBase* mBase) noexcept					{ return reinterpret_cast<char*>(getLayout(mBase)); }

				template<typename T> inline static constexpr char* getItemAddress(char* mPtr) noexcept	{ return reinterpret_cast<char*>(&reinterpret_cast<TLT<T>*>(mPtr)->storageItem); }
				template<typename T> inline static constexpr T* getItem(char* mPtr) noexcept			{ return reinterpret_cast<T*>(getItemAddress<T>(mPtr)); }
			};

			template<typename TBase> struct LHelperBool : public LHelperBase<TBase, LBool>
			{
				template<typename T> inline static constexpr char* getBoolAddress(char* mPtr) noexcept { return reinterpret_cast<char*>(&reinterpret_cast<LBool<T>*>(mPtr)->storageBool); }

				template<typename T, typename... TArgs> inline static void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (getBoolAddress<T>(mPtr)) bool{true};
					new (LHelperBool::template getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
				}

				inline static void setBool(TBase* mBase, bool mBool) noexcept		{ *reinterpret_cast<bool*>(&LHelperBool::getLayout(mBase)->storageBool) = mBool; }
				inline static constexpr bool getBool(const TBase* mBase) noexcept	{ return *reinterpret_cast<const bool*>(&LHelperBool::getLayout(mBase)->storageBool); }
			};

			template<typename TBase> struct LHelperNoBool : public LHelperBase<TBase, LNoBool>
			{
				template<typename T, typename... TArgs> inline static void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (LHelperNoBool::template getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
				}
			};
		}
	}
}

#endif
