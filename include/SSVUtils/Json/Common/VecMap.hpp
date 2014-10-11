// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_COMMON_VECMAP
#define SSVU_JSON_COMMON_VECMAP

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			/// @brief Base CRTP class for vector-based sorted containers.
			template<typename TDerived> class VecMapBase
			{
				private:
					inline auto& getTD() noexcept				{ return castUp<TDerived>(*this); }
					inline const auto& getTD() const noexcept	{ return castUp<TDerived>(*this); }

				public:
					/// @brief Returns whether or not `mValue` is present in the container.
					template<typename T> inline bool has(const T& mValue) const noexcept { return getTD().is(getTD().lookup(mValue), mValue); }

					// Getters for the internal vector storage
					inline auto& getData() noexcept				{ return getTD().data; }
					inline const auto& getData() const noexcept	{ return getTD().data; }

					// Equality/inequality
					template<typename TC> inline auto operator==(const TC& mC) const noexcept { return getData() == mC.getData(); }
					template<typename TC> inline auto operator!=(const TC& mC) const noexcept { return !(operator==(mC)); }

					// Standard (partial) vector interface support
					inline void reserve(std::size_t mV)	{ getData().reserve(mV); }
					inline void clear() noexcept		{ getData().clear(); }
					inline auto size() const noexcept	{ return getData().size(); }
					inline auto empty() const noexcept	{ return getData().empty(); }

					// Standard iterator support
					inline auto begin()		noexcept		{ return std::begin(getData()); }
					inline auto end()		noexcept		{ return std::end(getData()); }
					inline auto begin()		const noexcept	{ return std::begin(getData()); }
					inline auto end()		const noexcept	{ return std::end(getData()); }
					inline auto cbegin()	const noexcept	{ return std::cbegin(getData()); }
					inline auto cend()		const noexcept	{ return std::cend(getData()); }
					inline auto rbegin()	noexcept		{ return std::rbegin(getData()); }
					inline auto rend()		noexcept		{ return std::rend(getData()); }
					inline auto crbegin()	const noexcept	{ return std::crbegin(getData()); }
					inline auto crend()		const noexcept	{ return std::crend(getData()); }
			};

			/// @brief Map-like sorted container implemented on top of an `std::vector`.
			/// @details Key/value pairs are stored in a sorted vector of `std::pair<TK, TV>`.
			/// @tparam TK Key type.
			/// @tparam TV Value type.
			template<typename TK, typename TV> class VecMap : public VecMapBase<VecMap<TK, TV>>
			{
				template<typename TDerived> friend class VecMapBase;

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
					inline VecMap(VecMap&& mVM) : data{std::move(mVM.data)} { }
					inline VecMap(std::initializer_list<Item>&& mIL) : data{std::move(mIL)}
					{
						sort(data, [](const auto& mA, const auto& mB){ return mA.first < mB.first; });
					}

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

					/// @brief Returns a const reference to the value with key `mKey`. A default-constructed `TV` is returned if unexistant.
					inline const auto& atOrDefault(const TK& mKey) const noexcept
					{
						auto itr(lookup(mKey));
						if(is(itr, mKey)) return itr->second;
						return TV{};
					}
			};
		}
	}
}

#endif
