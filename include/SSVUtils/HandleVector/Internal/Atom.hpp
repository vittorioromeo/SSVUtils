// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_ATOM
#define SSVU_HANDLEVECTOR_INTERNAL_ATOM

namespace ssvu
{
	template<typename> class HandleVector;

	namespace Internal
	{
		/// @brief Internal HandleVector class storing data and metadata.
		template<typename T> class Atom
		{
			template<typename> friend class ssvu::HandleVector;

			private:
				/// @brief Data being stored.
				AlignedStorageFor<T> data;

				/// @brief Stat of the atom.
				HVStat stat;

				/// @brief Allocates and constructs the data. Assumes the atom is not alive.
				template<typename... TArgs> inline void initData(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
				{
					SSVU_ASSERT(!stat.alive);
					new (&data) T(fwd<TArgs>(mArgs)...);
				}

				/// @brief Deallocates and destroys the data. Assumes the atom is alive.
				inline void deinitData() noexcept(isNothrowDtor<T>())
				{
					SSVU_ASSERT(!stat.alive);
					getDataImpl<T&>().~T();
				}

				template<typename TR> inline TR getDataImpl() noexcept { return castStorage<T>(data); }

			public:
				inline Atom(HIdx mMarkIdx) noexcept : stat{mMarkIdx} { }
				inline Atom(Atom&&) = default;
				inline Atom& operator=(Atom&&) = default;

				inline Atom(const Atom&) = delete;
				inline Atom& operator=(const Atom&) = delete;

				/// @brief Returns a reference to the data. Assumes the atom is alive.
				inline T& getData() noexcept { SSVU_ASSERT(stat.alive); return getDataImpl<T&>(); }

				/// @brief Returns a const reference to the data. Assumes the atom is alive.
				inline const T& getData() const noexcept { SSVU_ASSERT(stat.alive); return getDataImpl<const T&>(); }

				/// @brief Sets the atom's status to alive.
				inline void setAlive() noexcept { stat.alive = true; }

				/// @brief Sets the atom's status to dead.
				inline void setDead() noexcept { stat.alive = false; }

				/// @brief Returns whether the atom is alive or not.
				inline bool isAlive() const noexcept { return stat.alive; }

				inline static constexpr Atom<T>* getAtomFromPtr(T* mPtr) noexcept
				{
					return SSVU_GET_BASEPTR_FROM_MEMBERPTR(Atom<T>, mPtr, data);
				}

				inline auto getMarkIdx() const noexcept { return stat.markIdx; }
		};
	}
}

#endif
