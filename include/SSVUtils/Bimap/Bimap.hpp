// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BIMAP
#define SSVU_BIMAP

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	/// @namespace Internal bimap implementation details.
	namespace Internal
	{
		/// @brief Comparison functor that compares pointer's values.
		template<typename T> struct PtrComparator
		{
			inline bool operator()(const T* mA, const T* mB) const noexcept { return *mA < *mB; }
		};

		/// @typedef Set of pointers, sorted by pointer values.
		template<typename T> using PtrSet = std::set<const T*, Internal::PtrComparator<T>>;

		/// @brief Helper bimap struct.
		template<typename T1, typename T2, typename T> struct BimapHelper;

		/// @brief Helper bimap struct. (specialized for the first type)
		template<typename T1, typename T2> struct BimapHelper<T1, T2, T1>
		{
			/// @typedef The other type.
			using Other = T2;

			/// @brief Returns a const reference to the set of the first type.
			template<typename TBimap> inline static const auto& getSetCurrent(TBimap& mBimap) noexcept { return mBimap.set1; }
		};

		/// @brief Helper bimap struct. (specialized for the second type)
		template<typename T1, typename T2> struct BimapHelper<T1, T2, T2>
		{
			/// @typedef The other type.
			using Other = T1;

			/// @brief Returns a const reference to the set of the second type.
			template<typename TBimap> inline static const auto& getSetCurrent(TBimap& mBimap) noexcept { return mBimap.set2; }
		};
	}

	/// @brief Bi-directional key-value container.
	/// @details Implemented as two std::sets of pointers. Assumes `std::pair` memory contiguity.
	/// @tparam T1 Key/Value type 1
	/// @tparam T2 Key/Value type 2
	template<typename T1, typename T2> class Bimap
	{
		template<typename, typename, typename> friend struct Internal::BimapHelper;

		public:
			/// @typedef Type of pair.
			using BMPair = std::pair<T1, T2>;

			/// @typedef Type of storage.
			using Storage = VecUPtr<BMPair>;

			// Standard iterator support
			using iterator = typename Storage::iterator;
			using const_iterator = typename Storage::const_iterator;
			using reverse_iterator = typename Storage::reverse_iterator;
			using const_reverse_iterator = typename Storage::const_reverse_iterator;

		private:
			/// @brief Storage of key/value pairs.
			/// @details Implemented as an std::vector of ssvu::UPtr.
			Storage storage;

			/// @brief Set of the first type.
			Internal::PtrSet<T1> set1;

			/// @brief Set of the second type.
			Internal::PtrSet<T2> set2;

			/// @brief Internal method to get a BMPair from a pointer.
			template<typename T> inline constexpr BMPair& getPairImpl(const T* mPtr) const noexcept { return *(const_cast<BMPair*>(reinterpret_cast<const BMPair*>(mPtr))); }

			/// @brief Internal method to get a BMPair from T1*.
			inline constexpr BMPair& getPair(const T1* mItem) const noexcept { return getPairImpl(mItem); }

			/// @brief Internal method to get a BMPair from T2*.
			inline constexpr BMPair& getPair(const T2* mItem) const noexcept { return getPairImpl(SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(BMPair, mItem, second)); }

			/// @brief Internal method to get a T2& from the corrisponding T1*.
			inline T2& getItem(const T1* mItem) noexcept { return getPair(mItem).second; }

			/// @brief Internal method to get a T1& from the corrisponding T2*.
			inline T1& getItem(const T2* mItem) noexcept { return getPair(mItem).first; }

			/// @brief Internal method to get a T2& from the corrisponding T1*. (const version)
			inline const T2& getItem(const T1* mItem) const noexcept { return getPair(mItem).second; }

			/// @brief Internal method to get a T1& from the corrisponding T2*. (const version)
			inline const T1& getItem(const T2* mItem) const noexcept { return getPair(mItem).first; }

			/// @brief Internal implementation of the `at` method.
			/// @details Throws an `std::out_of_range` exception if the value isn't found.
			template<typename T> const auto& atImpl(const T& mKey) const noexcept
			{
				const auto& set(Internal::BimapHelper<T1, T2, T>::getSetCurrent(*this));
				const auto& itr(this->find(mKey));
				if(itr == std::end(set)) throw std::out_of_range{"mKey was not found in set"};
				return getItem(*itr);
			}

			/// @brief Internal implementation of the `erase` method.
			/// @details Assumes (and asserts) that the `mKey` value exists in the bimap.
			template<typename T> inline void eraseImpl(const T& mKey)
			{
				SSVU_ASSERT(this->has(mKey));

				const auto& pair(getPair(*this->find(mKey)));

				set1.erase(&pair.first);
				set2.erase(&pair.second);

				eraseRemoveIf(storage, [&pair](const UPtr<BMPair>& mI){ return mI.get() == &pair; });

				SSVU_ASSERT(!this->has(mKey));
			}

		public:
			/// @brief Default constructor.
			/// @details Initializes an empty Bimap.
			inline Bimap() = default;

			/// @brief Initializer list constructor.
			/// @details Initializes a Bimap from a std::initalizer_list of pairs.
			/// @param mPairs Initializer list of BMPair.
			inline Bimap(const std::initializer_list<BMPair>& mPairs) { for(const auto& p : mPairs) emplace(p.first, p.second); }

			/// @brief Emplaces a T1/T2 pair into the bimap.
			/// @details A copy of the values is created and stored into the bimap. Asserts that the value doesn't already exist.
			/// @param mArg1 First value.
			/// @param mArg2 Second value.
			template<typename TA1, typename TA2> inline BMPair& emplace(TA1&& mArg1, TA2&& mArg2)
			{
				SSVU_ASSERT(!this->has(mArg1) && !this->has(mArg2));

				auto& pair(ssvu::getEmplaceUPtr<BMPair>(storage, fwd<TA1>(mArg1), fwd<TA2>(mArg2)));
				set1.emplace(&pair.first);
				set2.emplace(&pair.second);

				return pair;
			}

			/// @brief Inserts a T1/T2 pair into the bimap.
			/// @details Internally uses `emplace`.
			inline BMPair& insert(const BMPair& mPair) { return this->emplace(mPair.first, mPair.second); }

			/// @brief Erases a T1/T2 pair into the bimap.
			/// @param mKey Key of the pair.
			inline void erase(const T1& mKey) { this->eraseImpl(mKey); }

			/// @brief Erases a T1/T2 pair into the bimap.
			/// @param mKey Key of the pair.
			inline void erase(const T2& mKey) { this->eraseImpl(mKey); }

			/// @brief Returns a const reference to a value of a bimap pair.
			/// @details Throws an `std::out_of_range` exception if the value isn't found.
			/// @param mKey Key of the pair.
			inline const T2& at(const T1& mKey) const noexcept { return this->atImpl(mKey); }

			/// @brief Returns a const reference to a value of a bimap pair.
			/// @details Throws an `std::out_of_range` exception if the value isn't found.
			/// @param mKey Key of the pair.
			inline const T1& at(const T2& mKey) const noexcept { return this->atImpl(mKey); }

			/// @brief Returns a reference to a value of a bimap pair, or creates it if unexistant.
			/// @details Checks if the value exists.
			/// @param mKey Key of the pair.
			inline T2& operator[](const T1& mKey) noexcept { return this->has(mKey) ? this->get(mKey) : this->emplace(mKey, T2{}).second; }

			/// @brief Returns a reference to a value of a bimap pair, or creates it if unexistant.
			/// @details Checks if the value exists.
			/// @param mKey Key of the pair.
			inline T1& operator[](const T2& mKey) noexcept { return this->has(mKey) ? this->get(mKey) : this->emplace(T1{}, mKey).first; }

			/// @brief Returns a reference to a value of a bimap pair. (unsafe)
			/// @details Does not check if the value exists.
			/// @param mKey Key of the pair.
			inline T2& get(const T1& mKey) noexcept { SSVU_ASSERT(this->has(mKey)); return getItem(*this->find(mKey)); }

			/// @brief Returns a reference to a value of a bimap pair. (unsafe)
			/// @details Does not check if the value exists.
			/// @param mKey Key of the pair.
			inline T1& get(const T2& mKey) noexcept { SSVU_ASSERT(this->has(mKey)); return getItem(*this->find(mKey)); }

			/// @brief Returns a const reference to a value of a bimap pair. (unsafe)
			/// @details Does not check if the value exists.
			/// @param mKey Key of the pair.
			inline const T2& operator[](const T1& mKey) const noexcept { return this->get(mKey); }

			/// @brief Returns a const reference to a value of a bimap pair. (unsafe)
			/// @details Does not check if the value exists.
			/// @param mKey Key of the pair.
			inline const T1& operator[](const T2& mKey) const noexcept { return this->get(mKey); }

			/// @brief Clears the bimap, destroying its items.
			inline void clear() noexcept { storage.clear(); set1.clear(); set2.clear(); }

			/// @brief Returns true if the bimap is empty.
			inline bool empty() const noexcept { return storage.empty(); }

			/// @brief Returns the size of the bimap.
			inline auto size() const noexcept { return storage.size(); }

			/// @brief Returns the count of items with `mKey` key.
			inline auto count(const T1& mKey) const noexcept { return set1.count(&mKey); }

			/// @brief Returns the count of items with `mKey` key.
			inline auto count(const T2& mKey) const noexcept { return set2.count(&mKey); }

			/// @brief Returns an iterator to the item with `mKey` key.
			inline auto find(const T1& mKey) const noexcept { return set1.find(&mKey); }

			/// @brief Returns an iterator to the item with `mKey` key.
			inline auto find(const T2& mKey) const noexcept { return set2.find(&mKey); }

			/// @brief Returns true if the bimap contains the `mKey` value.
			inline bool has(const T1& mKey) const noexcept { return this->find(mKey) != std::end(set1); }

			/// @brief Returns true if the bimap contains the `mKey` value.
			inline bool has(const T2& mKey) const noexcept { return this->find(mKey) != std::end(set2); }

			// Standard iterator support
			inline auto begin()		noexcept		{ return storage.begin(); }
			inline auto end()		noexcept		{ return storage.end(); }
			inline auto begin()		const noexcept	{ return storage.begin(); }
			inline auto end()		const noexcept	{ return storage.end(); }
			inline auto cbegin()	const noexcept	{ return storage.cbegin(); }
			inline auto cend()		const noexcept	{ return storage.cend(); }
			inline auto rbegin()	noexcept		{ return storage.rbegin(); }
			inline auto rend()		noexcept		{ return storage.rend(); }
			inline auto crbegin()	const noexcept	{ return storage.crbegin(); }
			inline auto crend()		const noexcept	{ return storage.crend(); }
	};

	// Stringifier support
	template<typename T1, typename T2> struct Stringifier<Bimap<T1, T2>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Bimap<T1, T2>& mValue)
		{
			auto itrBegin(std::begin(mValue));
			if(itrBegin == std::end(mValue)) { Internal::printBold<TFmt>(mStream, "{ EMPTY }"); return; }

			Internal::printBold<TFmt>(mStream, "{");

			Internal::repeatPenultimate(itrBegin, std::end(mValue), [&mStream](const auto& mE)
			{
				Internal::callStringifyImpl<TFmt>(mStream, mE->first);
				Internal::printBold<TFmt>(mStream, " <-> ");
				Internal::callStringifyImpl<TFmt>(mStream, mE->second);
			}, [&mStream](const auto&){ Internal::printBold<TFmt>(mStream, "\n"); });

			Internal::printBold<TFmt>(mStream, "}");
		}
	};
}

#endif
