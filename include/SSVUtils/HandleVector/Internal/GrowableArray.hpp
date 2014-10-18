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

			public:
				inline GrowableArray() = default;
				inline GrowableArray(GrowableArray&& mRA) noexcept
				{
					mRA.data = data;
					data = nullptr;
				}

				inline ~GrowableArray() noexcept { delete[] data; }

				inline auto& operator=(GrowableArray&& mRA) noexcept
				{
					mRA.data = data;
					data = nullptr;
					return *this;
				}

				inline void resize(SizeT mCapacityOld, SizeT mCapacityNew)
				{
					SSVU_ASSERT(mCapacityOld <= mCapacityNew);

					auto newData(new TStorage[mCapacityNew]);
					for(auto i(0u); i < mCapacityOld; ++i) newData[i] = std::move(data[i]);

					delete[] data;
					data = newData;
				}

				template<typename... TArgs> inline void constructAt(SizeT mI, TArgs&&... mArgs)
					noexcept(isNothrowCtor<T, TArgs...>())
				{
					new(&data[mI]) T(fwd<TArgs>(mArgs)...);
				}

				inline void destroyAt(SizeT mI) noexcept(isNothrowDtor<T>())
				{
					(*this)[mI].~T();
				}

				inline auto& operator[](SizeT mI) noexcept				{ return reinterpret_cast<T&>(data[mI]); }
				inline const auto& operator[](SizeT mI) const noexcept	{ return reinterpret_cast<const T&>(data[mI]); }
		};
	}
}

#endif

// TODO: docs
