// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_GROWABLEARRAY
#define SSVU_HANDLEVECTOR_GROWABLEARRAY

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	/// @brief Low-level growable array storage class.
	/// @details Data must be explicitly constructed and destroyed.
	template<typename T> class GrowableArray
	{
		private:
			/// @typedef Aligned storage type for `T`.
			using TStorage = AlignedStorageFor<T>;

			/// @brief Internal storage.
			TStorage* data{nullptr};

		public:
			inline GrowableArray() noexcept = default;
			inline ~GrowableArray() noexcept { delete[] data; }

			inline GrowableArray(const GrowableArray& mGA) = delete;
			inline GrowableArray(GrowableArray&& mGA) noexcept
				: data(mGA.data)
			{
				mGA.data = nullptr;
			}

			inline auto& operator=(const GrowableArray& mGA) = delete;
			inline auto& operator=(GrowableArray&& mGA) noexcept
			{
				if (this != &mGA)
				{
					delete[] data;
					data = mGA.data;
					
					mGA.data = nullptr;
				}
				
				return *this;
			}

			/// @brief Grows the internal storage from `mCapacityOld` to `mCapacityNew`.
			/// @details The new capacity must be greater or equal than the old one.
			inline void grow(SizeT mCapacityOld, SizeT mCapacityNew)
			{
				SSVU_ASSERT(mCapacityOld <= mCapacityNew);

				auto newData(new TStorage[mCapacityNew]);
				for(auto i(0u); i < mCapacityOld; ++i) newData[i] = std::move(data[i]);

				std::swap(data, newData);
				delete[] newData;
			}

			/// @brief Constructs a `T` instance at index `mI`.
			template<typename... TArgs> inline void initAt(SizeT mI, TArgs&&... mArgs)
				noexcept(isNothrowCtor<T, TArgs...>())
			{
				new(&data[mI]) T(FWD(mArgs)...);
			}

			/// @brief Destroyes a `T` instance at index `mI`.
			inline void deinitAt(SizeT mI) noexcept(isNothrowDtor<T>())
			{
				(*this)[mI].~T();
			}

			// Getters
			inline T& operator[](SizeT mI) noexcept				{ return castStorage<T>(data[mI]); }
			inline const T& operator[](SizeT mI) const noexcept	{ return castStorage<T>(data[mI]); }
	};
}

#endif
