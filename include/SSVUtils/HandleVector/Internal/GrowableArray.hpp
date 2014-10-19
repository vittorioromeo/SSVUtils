// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_GROWABLEARRAY
#define SSVU_HANDLEVECTOR_INTERNAL_GROWABLEARRAY

namespace ssvu
{
	namespace Internal
	{
		template<typename T> class GrowableArray
		{
			private:
				using TStorage = AlignedStorageFor<T>;
				TStorage* data{nullptr};

				inline void moveImpl(GrowableArray&& mGA) noexcept
				{
					mGA.data = data;
					data = nullptr;
				}

			public:
				inline GrowableArray() noexcept = default;
				inline ~GrowableArray() noexcept { delete[] data; }

				inline GrowableArray(const GrowableArray& mGA) = delete;
				inline GrowableArray(GrowableArray&& mGA) noexcept { moveImpl(mGA); }

				inline auto& operator=(const GrowableArray& mGA) = delete;
				inline auto& operator=(GrowableArray&& mGA) noexcept { moveImpl(mGA); return *this; }

				inline void grow(SizeT mCapacityOld, SizeT mCapacityNew)
				{
					SSVU_ASSERT(mCapacityOld <= mCapacityNew);

					auto newData(new TStorage[mCapacityNew]);
					for(auto i(0u); i < mCapacityOld; ++i) newData[i] = std::move(data[i]);

					std::swap(data, newData);
					delete[] newData;
				}

				template<typename... TArgs> inline void initAt(SizeT mI, TArgs&&... mArgs)
					noexcept(isNothrowCtor<T, TArgs...>())
				{
					new(&data[mI]) T(fwd<TArgs>(mArgs)...);
				}

				inline void deinitAt(SizeT mI) noexcept(isNothrowDtor<T>())
				{
					(*this)[mI].~T();
				}

				inline T& operator[](SizeT mI) noexcept				{ return castStorage<T>(data[mI]); }
				inline const T& operator[](SizeT mI) const noexcept	{ return castStorage<T>(data[mI]); }
		};
	}
}

#endif

// TODO: docs
