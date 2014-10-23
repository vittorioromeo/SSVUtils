// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CONTAINER_VECSORTED
#define SSVU_CONTAINER_VECSORTED

namespace ssvu
{
	/// @brief Vector-like sorted container implemented on top of an `std::vector`.
	/// @details Values are stored in a sorted vector of `T`.
	/// @tparam T Value type.
	/// @tparam TCmp Comparer type.
	template<typename T, typename TCmp = std::less<T>> class VecSorted : public Internal::VecMapBase<VecSorted<T>>
	{
		template<typename> friend class Internal::VecMapBase;

		private:
			std::vector<T> data;
			TCmp cmp;

			// Value lookup helper
			template<typename TT> inline static auto lookupHelper(TT& mVecSorted, const T& mX) noexcept
			{
				return lowerBound(mVecSorted.data, mX, mVecSorted.cmp);
			}

			inline auto lookup(const T& mX) noexcept		{ return lookupHelper(*this, mX); }
			inline auto lookup(const T& mX) const noexcept	{ return lookupHelper(*this, mX); }

			// Returns validity of a looked-up object
			template<typename TT> inline bool is(const TT& mItr, const T& mX) const noexcept
			{
				return mItr != std::end(data) && mItr == mX;
			}

		public:
			inline VecSorted() = default;
			inline VecSorted(const VecSorted& mVM) : data{mVM.data} { }
			inline VecSorted(VecSorted&& mVM) noexcept : data{std::move(mVM.data)} { }
			inline VecSorted(std::initializer_list<T>&& mIL) : data{std::move(mIL)} { sort(data, cmp); }

			inline auto& operator=(const VecSorted& mVS) { data = mVS.data; return *this; }
			inline auto& operator=(VecSorted&& mVS) noexcept { data = std::move(mVS.data); return *this; }

			/// @brief Inserts a value in the sorted vector. Returns an iterator to the emplaced value.
			template<typename TT> inline auto insert(TT&& mX)
			{
				auto itr(lookup(mX));
				return data.emplace(itr, fwd<TT>(mX));
			}

			/// @brief Returns an iterator to the value `mX`. A past-the-end iterator is returned if unexistant.
			inline auto atItr(const T& mX) const noexcept
			{
				auto itr(lookup(mX));
				if(is(itr, mX)) return itr;
				return std::end(data);
			}

			/// @brief Returns a reference to the item stored at index `mI`.
			inline auto& operator[](SizeT mI) noexcept { return data[mI]; }

			/// @brief Returns a const reference to the item stored at index `mI`.
			inline const auto& operator[](SizeT mI) const noexcept { return data[mI]; }
	};
}

#endif
