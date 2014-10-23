// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CONTAINER_VECMAP
#define SSVU_CONTAINER_VECMAP

namespace ssvu
{
	/// @brief Map-like sorted container implemented on top of an `std::vector`.
	/// @details Key/value pairs are stored in a sorted vector of `std::pair<TK, TV>`.
	/// @tparam TK Key type.
	/// @tparam TV Value type.
	template<typename TK, typename TV> class VecMap : public Internal::VecMapBase<VecMap<TK, TV>>
	{
		template<typename> friend class Internal::VecMapBase;

		public:
			/// @typedef Type of object stored in the internal vector.
			using Item = std::pair<TK, TV>;

		private:
			std::vector<Item> data;

			// Map-like lookup based on keys
			template<typename T> inline static auto lookupHelper(T& mVecMap, const TK& mKey) noexcept
			{
				return lowerBound(mVecMap.data, mKey, [](const auto& mA, const auto& mB){ return mA.first < mB; });
			}

			inline auto lookup(const TK& mKey) noexcept			{ return lookupHelper(*this, mKey); }
			inline auto lookup(const TK& mKey) const noexcept	{ return lookupHelper(*this, mKey); }

			// Returns validity of a looked-up object
			template<typename T> inline bool is(const T& mItr, const TK& mKey) const noexcept
			{
				return mItr != std::end(data) && mItr->first == mKey;
			}

		public:
			inline VecMap() = default;
			inline VecMap(const VecMap& mVM) : data{mVM.data} { }
			inline VecMap(VecMap&& mVM) noexcept : data{std::move(mVM.data)} { }
			inline VecMap(std::initializer_list<Item>&& mIL) : data{std::move(mIL)}
			{
				sort(data, [](const auto& mA, const auto& mB){ return mA.first < mB.first; });
			}

			inline auto& operator=(const VecMap& mVM) { data = mVM.data; return *this; }
			inline auto& operator=(VecMap&& mVM) noexcept { data = std::move(mVM.data); return *this; }

			inline SizeT count(const TK& mKey) const noexcept { return is(lookup(mKey), mKey) ? 1 : 0; }

			/// @brief Returns a non-const reference to the value with key `mKey`. The key/value pair is created if unexistant.
			template<typename TTK> inline auto& operator[](TTK&& mKey)
			{
				auto itr(lookup(mKey));
				return is(itr, mKey) ? itr->second : data.emplace(itr, fwd<TTK>(mKey), TV{})->second;
			}

			/// @brief Returns a const reference to the value with key `mKey`. An exception is thrown if unexistant.
			inline const auto& at(const TK& mKey) const
			{
				auto itr(lookup(mKey));
				if(is(itr, mKey)) return itr->second;

				throw std::out_of_range{""};
			}

			/// @brief Returns a const reference to the value with key `mKey`. A default-constructed static `TV` is returned if unexistant.
			inline const auto& atOrDefault(const TK& mKey) const noexcept
			{
				static TV defValue;

				auto itr(lookup(mKey));
				if(is(itr, mKey)) return itr->second;
				return defValue;
			}

			/// @brief Returns an iterator to the value with key `mKey`. A past-the-end iterator is returned if unexistant.
			inline auto atItr(const TK& mKey) const noexcept
			{
				auto itr(lookup(mKey));
				if(is(itr, mKey)) return itr;
				return std::end(data);
			}
	};
}

#endif
