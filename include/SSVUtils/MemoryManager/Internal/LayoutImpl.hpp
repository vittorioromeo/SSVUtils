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

			template<typename TBase> struct LHelperBase
			{
				inline static void deallocate(char* mPtr) noexcept { SSVU_ASSERT(mPtr != nullptr); delete[] mPtr; }
				inline static void destroy(TBase* mBase) noexcept(noexcept(mBase->TBase::~TBase()))
				{
					SSVU_ASSERT(mBase != nullptr);
					mBase->TBase::~TBase();
				}
			};

			template<typename TBase> struct LHelperBool : public LHelperBase<TBase>
			{
				using LType = LBool<TBase>;

				template<typename T> inline static char* allocate()										{ return new char[sizeof(LBool<T>)]; }
				template<typename T> inline static constexpr char* getBoolAddress(char* mPtr) noexcept	{ return reinterpret_cast<char*>(&reinterpret_cast<LBool<T>*>(mPtr)->storageBool); }
				template<typename T> inline static constexpr char* getItemAddress(char* mPtr) noexcept	{ return reinterpret_cast<char*>(&reinterpret_cast<LBool<T>*>(mPtr)->storageItem); }

				template<typename T, typename... TArgs> inline static void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (getBoolAddress<T>(mPtr)) bool{true};
					new (getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
				}

				// Memory-manipulation getters/setters
				inline static constexpr LType* getLayout(TBase* mBase) noexcept					{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(LType, mBase, storageItem); }
				inline static constexpr const LType* getLayout(const TBase* mBase) noexcept		{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(LType, mBase, storageItem); }
				inline static constexpr char* getByte(TBase* mBase) noexcept					{ return reinterpret_cast<char*>(getLayout(mBase)); }
				template<typename T> inline static constexpr T* getBase(char* mPtr) noexcept	{ return reinterpret_cast<T*>(&reinterpret_cast<LBool<T>*>(mPtr)->storageItem); }
				inline static void setBool(TBase* mBase, bool mBool) noexcept					{ *reinterpret_cast<bool*>(&getLayout(mBase)->storageBool) = mBool; }
				inline static constexpr bool getBool(const TBase* mBase) noexcept				{ return *reinterpret_cast<const bool*>(&getLayout(mBase)->storageBool); }
			};

			template<typename TBase> struct LHelperNoBool : public LHelperBase<TBase>
			{
				using LType = LNoBool<TBase>;

				template<typename T> inline static char* allocate()										{ return new char[sizeof(LNoBool<T>)]; }
				template<typename T> inline static constexpr char* getItemAddress(char* mPtr) noexcept	{ return reinterpret_cast<char*>(&reinterpret_cast<LNoBool<T>*>(mPtr)->storageItem); }

				template<typename T, typename... TArgs> inline static void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
				}

				// Memory-manipulation getters/setters
				inline static constexpr LType* getLayout(TBase* mBase) noexcept					{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(LType, mBase, storageItem); }
				inline static constexpr const LType* getLayout(const TBase* mBase) noexcept		{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(LType, mBase, storageItem); }
				inline static constexpr char* getByte(TBase* mBase) noexcept					{ return reinterpret_cast<char*>(getLayout(mBase)); }
				template<typename T> inline static constexpr T* getBase(char* mPtr) noexcept	{ return reinterpret_cast<T*>(&reinterpret_cast<LNoBool<T>*>(mPtr)->storageItem); }
			};
		}
	}
}

#endif
