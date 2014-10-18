// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_UNCERTAIN
#define SSVU_HANDLEVECTOR_INTERNAL_UNCERTAIN

namespace ssvu
{
	namespace Internal
	{
		/// @brief Provides aligned storage for a class of type T.
		/// @details The class is not always initialized - it must be constructed and destroyed by the user.
		template<typename T> class Uncertain
		{
			private:
				AlignedStorageFor<T> storage;

			public:
				/// @brief Allocates and construct the T instance.
				template<typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
				{
					new (&storage) T(fwd<TArgs>(mArgs)...);
				}

				/// @brief Deallocates and destroys the T instance.
				inline void deinit() noexcept(isNothrowDtor<T>()) { get().~T(); }

				/// @brief Returns a reference to the T instance. Assumes `init()` was called.
				inline T& get() noexcept { return reinterpret_cast<T&>(storage); }

				/// @brief Returns a const reference to the T instance. Assumes `init()` was called.
				inline const T& get() const noexcept { return reinterpret_cast<const T&>(storage); }

				inline static constexpr Uncertain<T>* getUncertainFromData(T* mData) noexcept
				{
					return SSVU_GET_BASEPTR_FROM_MEMBERPTR(Uncertain<T>, mData, storage);
				}
		};
	}
}

#endif
