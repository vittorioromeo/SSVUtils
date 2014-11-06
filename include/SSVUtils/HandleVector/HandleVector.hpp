// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR
#define SSVU_HANDLEVECTOR

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/HandleVector/Internal/Common.hpp"
#include "SSVUtils/HandleVector/Internal/Atom.hpp"
#include "SSVUtils/HandleVector/Internal/Iterator.hpp"
#include "SSVUtils/HandleVector/Handle.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"

namespace ssvu
{
	template<typename T> class HandleVector
	{
		template<typename> friend class Handle;

		private:
			using Stat = Internal::HVStat;
			using Mark = Internal::HVMark;

		public:
			/// @typedef Templatized `Internal::Atom<T>` type.
			using Atom = typename Internal::Atom<T>;

			/// @typedef Pointer-based fast iterator type.
			using ItrFast = Internal::HVecItrFast<T>;

			/// @typedef Index-based safe iterator type.
			using ItrIdx = Internal::HVecItrIdx<T>;

			/// @typedef Index-based safe atom iterator type.
			using ItrAtom = Internal::HVecItrAtom<T>;

		private:
			/// @brief Internal atom storage.
			GrowableArray<Atom> atoms;

			/// @brief Internal mark storage.
			GrowableArray<Mark> marks;

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

				atoms.grow(capacity, capacityNew);
				marks.grow(capacity, capacityNew);

				// Initialize resized storage
				for(; capacity < capacityNew; ++capacity)
				{
					atoms.initAt(capacity, capacity);
					marks.initAt(capacity, capacity);
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
			inline auto& getMarkFromAtom(const Atom& mAtom)	noexcept { return marks[mAtom.getMarkIdx()]; }

			/// @brief Returns a reference to mMark's controlled atom.
			inline auto& getAtomFromMark(const Mark& mMark) noexcept { return atoms[mMark.atomIdx]; }

			inline bool isAliveAt(SizeT mI) const noexcept	{ return atoms[mI].isAlive(); }
			inline bool isDeadAt(SizeT mI) const noexcept	{ return !isAliveAt(mI); }

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

					SSVU_ASSERT(atom.isAlive());
					atom.setDead();
					atom.deinitData();
					++mark.ctr;
				}

				size = sizeNext = 0u;
			}

			/// @brief Reserves storage, increasing the capacity.
			inline void reserve(SizeT mCapacityNew) { if(capacity < mCapacityNew) growCapacityTo(mCapacityNew); }

			/// @brief Creates and returns an handle pointing to mAtom.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			inline auto createHandleFromAtom(Atom& mAtom) noexcept
			{
				return Handle<T>{*this, mAtom.getMarkIdx(), getMarkFromAtom(mAtom).ctr};
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
				atom.setAlive();

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
				Internal::refreshImpl(size, sizeNext,
					[this](SizeT mI){ return isAliveAt(mI); },
					[this](SizeT mD, SizeT mA)
					{
						std::swap(atoms[mD], atoms[mA]);
						getMarkFromAtom(atoms[mD]).atomIdx = mD;
					},
					[this](SizeT mD)
					{
						atoms[mD].deinitData();
						++(getMarkFromAtom(atoms[mD]).ctr);
					});
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
				return *(Internal::Atom<T>::getAtomFromPtr(&mData));
			}

			/// @brief Returns a reference to `mHandle`'s atom.
			inline constexpr Atom& getAtomFromHandle(Handle<T>& mHandle) noexcept { return getAtomFromData(*mHandle); }

			/// @brief Returns the capacity of the internal storage.
			inline auto getCapacity() const noexcept { return capacity; }



			// Fast iterators

			/// @brief Returns a fast iterator pointing to the first data.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto begin() noexcept { return ItrFast{&atoms[0]}; }

			/// @brief Returns a fast iterator pointing one after the last data. Newly created atoms aren't taken into account.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto end() noexcept { return ItrFast{&atoms[size]}; }

			/// @brief Returns a fast iterator pointing one after the last newly-created data. Newly created atoms are taken into account.
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto endNext() noexcept { return ItrFast{&atoms[sizeNext]}; }

			/// @brief Returns a fast iterator pointing to the first data. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto begin() const noexcept { return ItrFast{&atoms[0]}; }

			/// @brief Returns a fast iterator pointing one after the last data. Newly created atoms aren't taken into account. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto end() const noexcept { return ItrFast{&atoms[size]}; }

			/// @brief Returns a fast iterator pointing one after the last newly-created data. Newly created atoms are taken into account. (const version)
			/// @details This iterator will be invalidated if the internal storage grows.
			inline auto endNext() const noexcept { return ItrFast{&atoms[sizeNext]}; }



			// Idx iterators

			/// @brief Returns an index iterator pointing to the first data.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto beginIdx() noexcept { return ItrIdx{0, *this}; }

			/// @brief Returns an index iterator pointing one after the last data. Newly created atoms aren't taken into account.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endIdx() noexcept { return ItrIdx{size, *this}; }

			/// @brief Returns an index iterator pointing one after the last newly-created data. Newly created atoms are taken into account.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endIdxNext() noexcept { return ItrIdx{sizeNext, *this}; }

			/// @brief Returns an index iterator pointing to the first data. (const vesrion)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto beginIdx() const noexcept { return ItrIdx{0, *this}; }

			/// @brief Returns an index iterator pointing one after the last data. Newly created atoms aren't taken into account. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endIdx() const noexcept { return ItrIdx{size, *this}; }

			/// @brief Returns an index iterator pointing one after the last newly-created data. Newly created atoms are taken into account. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endIdxNext() const noexcept { return ItrIdx{sizeNext, *this}; }



			// Atom iterators

			/// @brief Returns an index iterator pointing to the first atom.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto beginAtom() noexcept { return ItrAtom{0, *this}; }

			/// @brief Returns an index iterator pointing one after the last atom.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endAtom() noexcept { return ItrAtom{size, *this}; }

			/// @brief Returns an index iterator pointing to one after the last newly-created atom.
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endAtomNext() noexcept { return ItrAtom{sizeNext, *this}; }

			/// @brief Returns an index iterator pointing to the first atom. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto beginAtom() const noexcept { return ItrAtom{0, *this}; }

			/// @brief Returns an index iterator pointing one after the last atom. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endAtom() const noexcept { return ItrAtom{size, *this}; }

			/// @brief Returns an index iterator pointing to one after the last newly-created atom. (const version)
			/// @details This iterator won't be invalidated if the internal storage grows.
			inline auto endAtomNext() const noexcept { return ItrAtom{sizeNext, *this}; }



			// Fast ranges

			/// @brief Returns a range using fast iterators. Newly created atoms aren't taken into account.
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forFast() noexcept { return makeRange(begin(), end()); }

			/// @brief Returns a range using fast iterators. Newly created atoms aren't taken into account. (const version)
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forFast() const noexcept { return makeRange(begin(), end()); }

			/// @brief Returns a range using fast iterators. Newly created atoms are taken into account.
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forNextFast() noexcept { return makeRange(begin(), endNext()); }

			/// @brief Returns a range using fast iterators. Newly created atoms are taken into account. (const version)
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forNextFast() const noexcept { return makeRange(begin(), endNext()); }



			// Idx ranges

			/// @brief Returns a range using idx iterators. Newly created atoms aren't taken into account.
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forIdx() noexcept { return makeRange(beginIdx(), endIdx()); }

			/// @brief Returns a range using idx iterators. Newly created atoms aren't taken into account. (const version)
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forIdx() const noexcept { return makeRange(beginIdx(), endIdx()); }

			/// @brief Returns a range using idx iterators. Newly created atoms are taken into account.
			/// @details This range will be invalidated if the internal storage gro-ws.
			inline auto forNextIdx() noexcept { return makeRange(beginIdx(), endIdxNext()); }

			/// @brief Returns a range using idx iterators. Newly created atoms are taken into account. (const version)
			/// @details This range will be invalidated if the internal storage gro-ws.
			inline auto forNextIdx() const noexcept { return makeRange(beginIdx(), endIdxNext()); }



			// Atom ranges

			/// @brief Returns a range for atom iteration. Newly created atoms aren't taken into account.
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forAtom() noexcept { return makeRange(beginAtom(), endAtom()); }

			/// @brief Returns a range for atom iteration. Newly created atoms aren't taken into account. (const version)
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forAtom() const noexcept { return makeRange(beginAtom(), endAtom()); }

			/// @brief Returns a range for atom iteration. Newly created atoms are taken into account.
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forNextAtom() noexcept { return makeRange(beginAtom(), endAtomNext()); }

			/// @brief Returns a range for atom iteration. Newly created atoms are taken into account. (const version)
			/// @details This range will be invalidated if the internal storage grows.
			inline auto forNextAtom() const noexcept { return makeRange(beginAtom(), endAtomNext()); }
	};
}

#include "SSVUtils/HandleVector/Handle.inl"

#endif
