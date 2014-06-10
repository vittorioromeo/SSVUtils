// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_ITEMSTORAGE
#define SSVU_MEMORYMANAGER_INTERNAL_ITEMSTORAGE

namespace ssvu
{
	namespace Internal
	{
		namespace ItemStorage
		{
			/// @brief Storage class for the alive/dead boolean and the item.
			template<typename T> struct Layout
			{
				AlignedStorageBasic<bool> storageBool;
				AlignedStorageBasic<T> storageItem;
			};

			/// @brief Allocates and returns enough storage for a Layout<T>.
			template<typename T> inline char* allocate() { return new char[sizeof(Layout<T>)]; }

			/// @brief Deallocates Layout<T> storage.
			inline void deallocate(char* mPtr) noexcept { SSVU_ASSERT(mPtr != nullptr); delete[] mPtr; }

			/// @brief Returns the address of the boolean member of a Layout<T>.
			template<typename T> inline constexpr char* getBoolAddress(char* mPtr) noexcept { return reinterpret_cast<char*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageBool); }

			/// @brief Returns the address of the item member of a Layout<T>.
			template<typename T> inline constexpr char* getItemAddress(char* mPtr) noexcept { return reinterpret_cast<char*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageItem); }

			/// @brief Constructs a Layout<T> instance in an allocated `mPtr` storage.
			template<typename T, typename... TArgs> inline void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
			{
				SSVU_ASSERT(mPtr != nullptr);

				new (getBoolAddress<T>(mPtr)) bool{true};
				new (getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
			}

			/// @brief Destroyes a previously-constructed Layout<T> instance.
			template<typename TBase> inline void destroy(TBase* mBase) noexcept(noexcept(mBase->TBase::~TBase()))
			{
				SSVU_ASSERT(mBase != nullptr);
				mBase->TBase::~TBase();
			}

			// Memory-manipulation getters/setters
			template<typename TBase> inline constexpr Layout<TBase>* getLayout(TBase* mBase) noexcept				{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(Layout<TBase>, mBase, storageItem); }
			template<typename TBase> inline constexpr const Layout<TBase>* getLayout(const TBase* mBase) noexcept	{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(Layout<TBase>, mBase, storageItem); }

			template<typename TBase> inline constexpr char* getByte(TBase* mBase) noexcept	{ return reinterpret_cast<char*>(getLayout<TBase>(mBase)); }
			template<typename T> inline constexpr T* getBase(char* mPtr) noexcept			{ return reinterpret_cast<T*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageItem); }

			template<typename TBase> inline void setBool(TBase* mBase, bool mBool) noexcept		{ *reinterpret_cast<bool*>(&getLayout(mBase)->storageBool) = mBool; }
			template<typename TBase> inline constexpr bool getBool(const TBase* mBase) noexcept	{ return *reinterpret_cast<const bool*>(&getLayout(mBase)->storageBool); }
		}
	}
}

#endif
