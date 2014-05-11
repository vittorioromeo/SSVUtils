#ifndef SSVU_HANDLEVECTOR
#define SSVU_HANDLEVECTOR

#include "SSVUtils/Core/Core.hpp"

using HIdx = std::size_t;	///< @typedef HandleVector index type.
using HCtr = int;			///< @typedef HandleVector counter type.

namespace ssvu
{
	template<typename> class HandleVector;
}

#include "SSVUtils/HandleVector/Internal/Uncertain.hpp"
#include "SSVUtils/HandleVector/Internal/Atom.hpp"
#include "SSVUtils/HandleVector/Handle.hpp"

namespace ssvu
{
	template<typename T, typename TItrValue, typename TDerived> class HVecItrBase
	{
		friend TDerived;

		private:
			TItrValue value;

			inline TDerived& getThisDerived() noexcept { return *reinterpret_cast<TDerived*>(this); }
			template<typename TT> inline TT getImpl() noexcept { return getThisDerived().getImpl<TT>(); }

		public:
			inline HVecItrBase(TItrValue mValue) noexcept : value{mValue} { }

			inline TDerived& operator++() noexcept				{ ++value;			return getThisDerived(); }
			inline TDerived& operator++(int) noexcept			{ ++value;			return getThisDerived(); }
			inline TDerived& operator--() noexcept				{ --value;			return getThisDerived(); }
			inline TDerived& operator--(int) noexcept			{ --value;			return getThisDerived(); }
			inline TDerived& operator+=(int mOffset) noexcept	{ value += mOffset;	return getThisDerived(); }
			inline TDerived& operator-=(int mOffset) noexcept	{ value -= mOffset;	return getThisDerived(); }

			inline T& operator*() noexcept 				{ return getImpl<T&>(); }
			inline const T& operator*() const noexcept 	{ return getImpl<const T&>(); }
			inline T* operator->() noexcept 			{ return &(getImpl<T&>()); }
			inline const T* operator->() const noexcept { return &(getImpl<const T&>()); }

			inline bool operator==(const TDerived& mRhs) const noexcept { return value == mRhs.value; }
			inline bool operator!=(const TDerived& mRhs) const noexcept	{ return value != mRhs.value; }
			inline bool operator<(const TDerived& mRhs) const noexcept	{ return value < mRhs.value; }
			inline bool operator>(const TDerived& mRhs) const noexcept	{ return value > mRhs.value; }
			inline bool operator<=(const TDerived& mRhs) const noexcept	{ return value <= mRhs.value; }
			inline bool operator>=(const TDerived& mRhs) const noexcept	{ return value >= mRhs.value; }
	};

	template<typename T> class HVecItrFast : public HVecItrBase<T, Internal::Atom<T>*, HVecItrFast<T>>
	{
		template<typename, typename, typename> friend class HVecItrBase;

		private:
			template<typename TT> inline TT getImpl() noexcept { return this->value->getData(); }

		public:
			inline HVecItrFast(Internal::Atom<T>* mAtomPtr) noexcept : HVecItrBase<T, Internal::Atom<T>*, HVecItrFast<T>>{mAtomPtr} { }
	};

	template<typename T> class HVecItrIdx : public HVecItrBase<T, HIdx, HVecItrIdx<T>>
	{
		template<typename, typename, typename> friend class HVecItrBase;

		private:
			HandleVector<T>* hVec;
			template<typename TT> inline TT getImpl() noexcept { return hVec->getDataAt(this->value); }

		public:
			inline HVecItrIdx(HandleVector<T>& mHVec, HIdx mIdx) noexcept : HVecItrBase<T, HIdx, HVecItrIdx<T>>{mIdx}, hVec(&mHVec) { }
	};

	template<typename T> class HandleVector
	{
		template<typename> friend class Handle;

		private:
			/// @brief Structure controlling validity of the atoms and handles.
			struct Mark { HIdx atomIdx; HCtr ctr; };

		public:
			using AtomType = typename Internal::Atom<T>; ///< @typedef Atom type.

		private:
			std::vector<AtomType> atoms;	///< @brief Internal atom storage.
			std::vector<Mark> marks;		///< @brief Internal mark storage.
			std::size_t size{0u};			///< @brief Current size. Does not take into account newly created atoms.
			std::size_t sizeNext{0u};		///< @brief Next size. Takes into account newly created atoms.

			/// @brief Returns the capacity of the internal storage.
			inline std::size_t getCapacity() const noexcept { return atoms.size(); }

			/// @brief Increases internal storage capacity by mAmount.
			inline void growCapacityBy(std::size_t mAmount)
			{
				auto i(getCapacity()), newCapacity(getCapacity() + mAmount);
				SSVU_ASSERT(newCapacity >= 0 && newCapacity >= getCapacity());

				atoms.resize(newCapacity);
				marks.resize(newCapacity);

				// Initialize resized storage
				for(; i < newCapacity; ++i) atoms[i].markIdx = marks[i].atomIdx = i;
			}

			/// @brief Sets internal storage capacity to mCapacity.
			inline void growCapacityTo(std::size_t mCapacity)
			{
				SSVU_ASSERT(getCapacity() < mCapacity);
				growCapacityBy(mCapacity - getCapacity());
			}

			/// @brief Checks if the current capacity is enough - if it isn't, increases it.
			inline void growIfNeeded()
			{
				constexpr float growMultiplier{2.f};
				constexpr std::size_t growAmount{5};

				if(getCapacity() <= sizeNext) growCapacityTo((getCapacity() + growAmount) * growMultiplier);
			}

			/// @brief Sets the status of the atom pointed by the mark at mMarkIdx to dead.
			inline void destroy(HIdx mMarkIdx) noexcept
			{
				getAtomFromMark(marks[mMarkIdx]).setDead();
			}

			/// @brief Returns a reference to mAtom's controller mark.
			inline Mark& getMarkFromAtom(const AtomType& mAtom)	noexcept { return marks[mAtom.markIdx]; }

			/// @brief Returns a reference to mMark's controlled atom.
			inline AtomType& getAtomFromMark(const Mark& mMark) noexcept { return atoms[mMark.atomIdx]; }

		public:
			inline HandleVector() = default;
			inline ~HandleVector() { clear(); }

			/// @brief Clears the HandleVector, destroying all elements.
			/// @details Does not alter the capacity.
			inline void clear() noexcept
			{
				refresh();

				for(auto i(0u); i < size; ++i)
				{
					auto& atom(atoms[i]);
					auto& mark(marks[i]);

					SSVU_ASSERT(atom.alive);
					atom.alive = false;
					atom.deinitData();
					++mark.ctr;
				}

				size = sizeNext = 0u;
			}

			/// @brief Reserves storage, increasing the capacity.
			inline void reserve(std::size_t mCapacity) { if(getCapacity() < mCapacity) growCapacityTo(mCapacity); }

			/// @brief Creates and returns an handle pointing to mAtom.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			inline Handle<T> createHandleFromAtom(AtomType& mAtom) noexcept
			{
				return {*this, mAtom.markIdx, getMarkFromAtom(mAtom).ctr};
			}

			/// @brief Creates an atom, returning a reference to it.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline AtomType& createAtom(TArgs&&... mArgs)
			{
				// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
				growIfNeeded();

				// `sizeNext` now is the first empty valid index - we create our atom there
				auto& atom(atoms[sizeNext]);
				atom.initData(std::forward<TArgs>(mArgs)...);
				atom.alive = true;

				// Update the mark
				auto& mark(getMarkFromAtom(atom));
				mark.atomIdx = sizeNext;

				// Update next size
				++sizeNext;

				return atom;
			}

			/// @brief Creates an atom, returning an handle pointing to it.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline Handle<T> create(TArgs&&... mArgs)
			{
				return createHandleFromAtom(createAtom(std::forward<TArgs>(mArgs)...));
			}

			/// @brief Refreshes the HandleVector.
			/// @details Dead atoms are deallocated and destroyed. Newly created atoms are now taken into account.
			inline void refresh()
			{
				// Type must be signed, to check with negative values later
				int iDead{0};

				// Convert sizeNext to int to avoid warnings/issues
				const int intSizeNext(sizeNext);

				// Find first alive and first dead atoms
				while(iDead < intSizeNext && atoms[iDead].alive) ++iDead;
				int iAlive{iDead - 1};

				for(int iD{iDead}; iD < intSizeNext; ++iD)
				{
					// Skip alive atoms
					if(atoms[iD].alive) continue;

					// Found a dead atom - `i` now stores its index
					// Look for an alive atom after the dead atom
					for(int iA{iDead + 1}; true; ++iA)
					{
						// No more alive atoms, continue
						if(iA == intSizeNext) goto finishRefresh;

						// Skip dead atoms
						if(!atoms[iA].alive) continue;

						// Found an alive atom after dead `iD` atom - swap and update mark
						std::swap(atoms[iA], atoms[iD]);
						getMarkFromAtom(atoms[iD]).atomIdx = iD;
						iAlive = iD; iDead = iA;

						break;
					}
				}

				finishRefresh:

				// [iAlive + 1, intSizeNext) contains only dead atoms, clean them up
				for(int iD{iAlive + 1}; iD < intSizeNext; ++iD)
				{
					atoms[iD].deinitData();
					++(getMarkFromAtom(atoms[iD]).ctr);
				}

				size = sizeNext = iAlive + 1; // Update size
			}

			/// @brief Iterates over alive data. Newly created atoms aren't taken into account.
			template<typename TFunc> inline void forEach(TFunc mFunc)
			{
				for(auto i(0u); i < size; ++i) mFunc(atoms[i].getData());
			}

			/// @brief Iterates over alive atoms. Newly created atoms aren't taken into account.
			template<typename TFunc> inline void forEachAtom(TFunc mFunc)
			{
				for(auto i(0u); i < size; ++i) mFunc(atoms[i]);
			}

			/// @brief Returns a reference to the atom at mIdx.
			inline AtomType& getAtomAt(HIdx mIdx) noexcept { SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }

			/// @brief Returns a const reference to the atom at mIdx.
			inline const AtomType& getAtomAt(HIdx mIdx) const noexcept { SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }

			/// @brief Returns a reference to the data at mIdx. Assumes the data is initialized.
			inline T& getDataAt(HIdx mIdx) noexcept { return getAtomAt(mIdx).getData(); }

			/// @brief Returns a const reference to the data at mIdx. Assumes the data is initialized.
			inline const T& getDataAt(HIdx mIdx) const noexcept { return getAtomAt(mIdx).getData(); }

			/// @brief Returns the current size of the HandleVector. Newly created atoms aren't taken into account.
			inline std::size_t getSize() const noexcept { return size; }

			/// @brief Returns the next size of the HandleVector. Newly created atoms are taken into account.
			inline std::size_t getSizeNext() const noexcept { return sizeNext; }

			// TODO: test, docs
			inline HVecItrFast<T> begin() noexcept			{ return {&atoms[0]}; }
			inline HVecItrFast<T> end() noexcept			{ return {&atoms[size]}; }
			inline HVecItrFast<T> begin() const noexcept	{ return {&atoms[0]}; }
			inline HVecItrFast<T> end() const noexcept		{ return {&atoms[size]}; }
			//inline HVecItr<T> cbegin() const noexcept	{ return {*this, 0}; }
			//inline HVecItr<T> cend() const noexcept	{ return {*this, size}; }

			inline HVecItrIdx<T> beginIdx() noexcept		{ return {*this, 0}; }
			inline HVecItrIdx<T> endIdx() noexcept			{ return {*this, size}; }
			inline HVecItrIdx<T> beginIdx() const noexcept	{ return {*this, 0}; }
			inline HVecItrIdx<T> endIdx() const noexcept	{ return {*this, size}; }
	};
}

#include "SSVUtils/HandleVector/Handle.inl"

#endif
