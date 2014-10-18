// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR
#define SSVU_HANDLEVECTOR

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	/// @typedef HandleVector index type.
	using HIdx = SizeT;

	/// @typedef HandleVector counter type.
	using HCtr = int;

	template<typename> class HandleVector;
}

#include "SSVUtils/HandleVector/Internal/GrowableArray.hpp"
#include "SSVUtils/HandleVector/Internal/Uncertain.hpp"
#include "SSVUtils/HandleVector/Internal/Atom.hpp"
#include "SSVUtils/HandleVector/Internal/Iterator.hpp"
#include "SSVUtils/HandleVector/Handle.hpp"

namespace ssvu
{
	template<typename T> class HandleVector
	{
		template<typename> friend class Handle;

		private:
			/// @brief Structure controlling validity of the atoms and handles.
			struct Mark
			{
				HIdx atomIdx;
				HCtr ctr;

				inline Mark(HIdx mAtomIdx) noexcept : atomIdx{mAtomIdx} { }
			};

		public:
			/// @typedef Templatized `Internal::Atom<T>` type.
			using Atom = typename Internal::Atom<T>;

		private:
			/// @brief Internal atom storage.
			Internal::GrowableArray<Atom> atoms;

			/// @brief Internal mark storage.
			Internal::GrowableArray<Mark> marks;

			/// @brief Current size. Does not take into account newly created atoms.
			SizeT size{0u};

			/// @brief Next size. Takes into account newly created atoms.
			SizeT sizeNext{0u};

			/// @brief Storage capacity for atoms/marks.
			SizeT capacity{0u};

			/// @brief Increases internal storage capacity by mAmount.
			inline void growCapacityBy(SizeT mAmount)
			{
				auto capacityNew(capacity + mAmount);
				SSVU_ASSERT(capacityNew >= 0 && capacityNew >= capacity);

				atoms.resize(capacity, capacityNew);
				marks.resize(capacity, capacityNew);

				// Initialize resized storage
				for(; capacity < capacityNew; ++capacity)
				{
					atoms.constructAt(capacity, capacity);
					marks.constructAt(capacity, capacity);
				}
			}

			/// @brief Sets internal storage capacity to mCapacity.
			inline void growCapacityTo(SizeT mCapacityNew)
			{
				SSVU_ASSERT(capacity < mCapacityNew);
				growCapacityBy(mCapacityNew - capacity);
			}

			/// @brief Checks if the current capacity is enough - if it isn't, increases it.
			inline void growIfNeeded()
			{
				constexpr float growMultiplier{2.f};
				constexpr SizeT growAmount{5};

				if(capacity <= sizeNext) growCapacityTo((capacity + growAmount) * growMultiplier);
			}

			/// @brief Sets the status of the atom pointed by the mark at mMarkIdx to dead.
			inline void destroy(HIdx mMarkIdx) noexcept
			{
				getAtomFromMark(marks[mMarkIdx]).setDead();
			}

			/// @brief Returns a reference to mAtom's controller mark.
			inline auto& getMarkFromAtom(const Atom& mAtom)	noexcept { return marks[mAtom.markIdx]; }

			/// @brief Returns a reference to mMark's controlled atom.
			inline auto& getAtomFromMark(const Mark& mMark) noexcept { return atoms[mMark.atomIdx]; }

		public:
			inline HandleVector() { growCapacityBy(10); }
			inline ~HandleVector() noexcept(isNothrowDtor<T>()) { clear(); }

			inline HandleVector(const HandleVector&) = delete;
			inline HandleVector(HandleVector&&) = delete;

			inline auto& operator=(const HandleVector&) = delete;
			inline auto& operator=(HandleVector&&) = delete;

			/// @brief Clears the HandleVector, destroying all elements.
			/// @details Does not alter the capacity.
			inline void clear() noexcept(isNothrowDtor<T>())
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
			inline void reserve(SizeT mCapacity) { if(capacity < mCapacity) growCapacityTo(mCapacity); }

			/// @brief Creates and returns an handle pointing to mAtom.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			inline auto createHandleFromAtom(Atom& mAtom) noexcept
			{
				return Handle<T>{*this, mAtom.markIdx, getMarkFromAtom(mAtom).ctr};
			}

			/// @brief Creates an atom, returning a reference to it.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline auto& createAtom(TArgs&&... mArgs)
			{
				// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
				growIfNeeded();

				// `sizeNext` now is the first empty valid index - we create our atom there
				auto& atom(atoms[sizeNext]);
				atom.initData(fwd<TArgs>(mArgs)...);
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
			template<typename... TArgs> inline auto create(TArgs&&... mArgs)
			{
				return createHandleFromAtom(createAtom(fwd<TArgs>(mArgs)...));
			}

			/// @brief Refreshes the HandleVector.
			/// @details Dead atoms are deallocated and destroyed. Newly created atoms are now taken into account.
			inline void refresh() noexcept(isNothrowDtor<T>())
			{
				// TODO: bottleneck - code review?

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
			inline auto& getAtomAt(HIdx mIdx) noexcept { return atoms[mIdx]; }

			/// @brief Returns a const reference to the atom at mIdx.
			inline const auto& getAtomAt(HIdx mIdx) const noexcept { return atoms[mIdx]; }

			/// @brief Returns a reference to the data at mIdx. Assumes the data is initialized.
			inline T& getDataAt(HIdx mIdx) noexcept { return getAtomAt(mIdx).getData(); }

			/// @brief Returns a const reference to the data at mIdx. Assumes the data is initialized.
			inline const T& getDataAt(HIdx mIdx) const noexcept { return getAtomAt(mIdx).getData(); }

			/// @brief Returns the current size of the HandleVector. Newly created atoms aren't taken into account.
			inline auto getSize() const noexcept { return size; }

			/// @brief Returns the next size of the HandleVector. Newly created atoms are taken into account.
			inline auto getSizeNext() const noexcept { return sizeNext; }

			/// @brief Returns a reference to the internal atom storage.
			inline auto& getAtoms() noexcept { return atoms; }

			/// @brief Returns a const reference to the internal atom storage.
			inline const auto& getAtoms() const noexcept { return atoms; }

			/// @brief Returns a reference to `mData`'s atom. Assumes `mData` is a member of an atom.
			/// @details Will not work correctly if the HandleVector gets resized (either by reserving or adding elements).
			inline constexpr Atom& getAtomFromData(T& mData) noexcept
			{
				return *(Internal::Atom<T>::getAtomFromUncertain(Internal::Uncertain<T>::getUncertainFromData(&mData)));
			}

			/// @brief Returns a reference to `mHandle`'s atom.
			inline constexpr Atom& getAtomFromHandle(Handle<T>& mHandle) noexcept { return getAtomFromData(*mHandle); }

			/// @brief Returns the capacity of the internal storage.
			inline auto getCapacity() const noexcept { return capacity; }


			// Fast iterators

			/// @brief Returns a fast iterator pointing to the first data.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> begin() noexcept { return {&atoms[0]}; }

			/// @brief Returns a fast iterator pointing one after the last data. Newly created atoms aren't taken into account.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> end() noexcept { return {&atoms[size]}; }

			/// @brief Returns a fast iterator pointing one after the last newly-created data. Newly created atoms are taken into account.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> endNext() noexcept { return {&atoms[sizeNext]}; }

			/// @brief Returns a fast iterator pointing to the first data. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> begin() const noexcept { return {&atoms[0]}; }

			/// @brief Returns a fast iterator pointing one after the last data. Newly created atoms aren't taken into account. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> end() const noexcept { return {&atoms[size]}; }

			/// @brief Returns a fast iterator pointing one after the last newly-created data. Newly created atoms are taken into account. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline HVecItrFast<T> endNext() const noexcept { return {&atoms[sizeNext]}; }



			// Idx iterators

			/// @brief Returns an index iterator pointing to the first data.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> beginIdx() noexcept { return {0, *this}; }

			/// @brief Returns an index iterator pointing one after the last data. Newly created atoms aren't taken into account.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> endIdx() noexcept { return {size, *this}; }

			/// @brief Returns an index iterator pointing one after the last newly-created data. Newly created atoms are taken into account.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> endIdxNext() noexcept { return {sizeNext, *this}; }

			/// @brief Returns an index iterator pointing to the first data. (const vesrion)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> beginIdx() const noexcept { return {0, *this}; }

			/// @brief Returns an index iterator pointing one after the last data. Newly created atoms aren't taken into account. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> endIdx() const noexcept { return {size, *this}; }

			/// @brief Returns an index iterator pointing one after the last newly-created data. Newly created atoms are taken into account. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline HVecItrIdx<T> endIdxNext() const noexcept { return {sizeNext, *this}; }
	};
}

#include "SSVUtils/HandleVector/Handle.inl"

#endif

// TODO: docs for array, cast ranges, review
// * forAtoms(), for(), forIdx()
