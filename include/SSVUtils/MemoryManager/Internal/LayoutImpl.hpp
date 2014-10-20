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
			/// @brief Storage class for the the item, without a bool.
			template<typename T> struct LNoBool
			{
				AlignedStorageFor<T> storageItem;
			};

			/// @brief Storage class for the alive/dead boolean and the item.
			template<typename T> struct LBool
			{
				// `bool` must be the first member of the struct, as `T` is of variable size.
				// Since we get instances of this struct by `TBase`, we cannot reliably get the bool address unless
				// it's the first member in the struct.
				AlignedStorageFor<bool> storageBool;
				AlignedStorageFor<T> storageItem;
			};

			/// @brief Base class used for Layout CRTP.
			template<typename TBase, template<typename> class TLT> struct LHelperBase
			{
				using TLType = TLT<TBase>;
				template<typename T> using Lyt = TLT<T>;

				template<typename T> inline static auto allocate()								{ return reinterpret_cast<TLT<T>*>(new char[sizeof(TLT<T>)]); }
				inline static void deallocate(char* mPtr) noexcept								{ SSVU_ASSERT(mPtr != nullptr); delete[] mPtr; }
				inline static void destroy(TBase* mBase) noexcept(noexcept(mBase->~TBase()))	{ SSVU_ASSERT(mBase != nullptr); mBase->~TBase(); }

				inline static constexpr auto getLayout(TBase* mBase) noexcept		{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(TLType, mBase, storageItem); }
				inline static constexpr auto getLayout(const TBase* mBase) noexcept	{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(TLType, mBase, storageItem); }
			};

			/// @brief CRTP implementation for a layout with no extra bool.
			template<typename TBase> struct LHelperNoBool : public LHelperBase<TBase, LNoBool>
			{
				template<typename T, typename... TArgs> inline static void construct(LNoBool<T>* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(fwd<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (&mPtr->storageItem) T(fwd<TArgs>(mArgs)...);
				}
			};

			/// @brief CRTP implementation for a layout with a bool.
			template<typename TBase> struct LHelperBool : public LHelperBase<TBase, LBool>
			{
				template<typename T, typename... TArgs> inline static void construct(LBool<T>* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(fwd<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					new (&mPtr->storageBool) bool{true};
					new (&mPtr->storageItem) T(fwd<TArgs>(mArgs)...);
				}

				inline static void setBool(TBase* mBase, bool mX) noexcept	{ castStorage<bool>(LHelperBool::getLayout(mBase)->storageBool) = mX; }
				inline static bool getBool(const TBase* mBase) noexcept		{ return castStorage<bool>(LHelperBool::getLayout(mBase)->storageBool); }
			};
		}
	}
}

#endif
