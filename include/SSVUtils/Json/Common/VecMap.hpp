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
			template<typename TK, typename TV> class VecMap
			{
				public:
					using Item = std::pair<TK, TV>;

				private:
					std::vector<Item> data;

					template<typename T> inline static auto lookupHelper(T& mVecMap, const TK& mKey) noexcept
					{
						return lowerBound(mVecMap.data, mKey, [](const auto& mA, const auto& mB){ return mA.first < mB; });
					}

					inline auto lookup(const TK& mKey) noexcept			{ return lookupHelper(*this, mKey); }
					inline auto lookup(const TK& mKey) const noexcept	{ return lookupHelper(*this, mKey); }

					template<typename T> inline bool is(const T& mItr, const TK& mKey) const noexcept
					{
						return mItr != std::end(data) && mItr->first == mKey;
					}

				public:
					inline VecMap() { data.reserve(10); }
					inline VecMap(const VecMap& mVM) : data{mVM.data} { }
					inline VecMap(VecMap&& mVM) : data{std::move(mVM.data)} { }
					inline VecMap(std::initializer_list<Item>&& mIL) : data{std::move(mIL)}
					{
						sort(data, [](const auto& mA, const auto& mB){ return mA.first < mB.first; });
					}

					inline auto& operator[](const TK& mKey)
					{
						auto itr(lookup(mKey));
						return is(itr, mKey) ? itr->second : data.emplace(itr, mKey, TV{})->second;
					}
					inline const auto& at(const TK& mKey) const
					{
						auto itr(lookup(mKey));
						if(is(itr, mKey)) return itr->second;

						throw std::out_of_range{""};
					}

					inline bool has(const TK& mKey) const noexcept { return is(lookup(mKey), mKey); }

					inline auto operator==(const VecMap& mVM) const noexcept { return data == mVM.data; }
					inline auto operator!=(const VecMap& mVM) const noexcept { return !(operator==(mVM)); }


					inline void clear()		noexcept		{ data.clear(); }

					inline auto size()		const noexcept	{ return data.size(); }
					inline auto empty()		const noexcept	{ return data.empty(); }

					inline auto begin()		noexcept		{ return std::begin(data); }
					inline auto end()		noexcept		{ return std::end(data); }
					inline auto begin()		const noexcept	{ return std::begin(data); }
					inline auto end()		const noexcept	{ return std::end(data); }
					inline auto cbegin()	const noexcept	{ return std::cbegin(data); }
					inline auto cend()		const noexcept	{ return std::cend(data); }
					inline auto rbegin()	noexcept		{ return std::rbegin(data); }
					inline auto rend()		noexcept		{ return std::rend(data); }
					inline auto crbegin()	const noexcept	{ return std::crbegin(data); }
					inline auto crend()		const noexcept	{ return std::crend(data); }
			};
		}
	}
}

#endif

// TODO: generalize to sortedvector
