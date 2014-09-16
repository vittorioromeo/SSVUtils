// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_ATOM
#define SSVU_HANDLEVECTOR_INTERNAL_ATOM

namespace ssvu
{
	namespace Internal
	{
		/// @brief Internal HandleVector class storing data and metadata.
		template<typename T> class Atom
		{
			template<typename> friend class ssvu::HandleVector;

			private:
				Uncertain<T> data;	///< @brief Data being stored.
				HIdx markIdx;		///< @brief Index of the mark controlling this atom.
				bool alive{false};	///< @brief Status of the atom.

				/// @brief Allocates and constructs the data. Assumes the atom is not alive.
				template<typename... TArgs> inline void initData(TArgs&&... mArgs)
					noexcept(noexcept(data.init(fwd<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(!alive);
					data.init(fwd<TArgs>(mArgs)...);
				}

				/// @brief Deallocates and destroys the data. Assumes the atom is alive.
				inline void deinitData() noexcept(noexcept(data.deinit()))
				{
					SSVU_ASSERT(!alive);
					data.deinit();
				}

			public:
				inline Atom(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} { }
				inline Atom(Atom&&) = default;
				inline Atom& operator=(Atom&&) = default;

				// Disallow copies
				inline Atom(const Atom&) = delete;
				inline Atom& operator=(const Atom&) = delete;

				/// @brief Returns a reference to the data. Assumes the atom is alive.
				inline T& getData() noexcept { SSVU_ASSERT(alive); return data.get(); }

				/// @brief Returns a const reference to the data. Assumes the atom is alive.
				inline const T& getData() const noexcept { SSVU_ASSERT(alive); return data.get(); }

				/// @brief Sets the atom's status to dead.
				inline void setDead() noexcept { alive = false; }

				/// @brief Returns whether the atom is alive or not.
				inline bool isAlive() const noexcept { return alive; }

				inline static constexpr Atom<T>* getAtomFromUncertain(Uncertain<T>* mUncertain) noexcept
				{
					return SSVU_GET_BASEPTR_FROM_MEMBERPTR(Atom<T>, mUncertain, data);
				}
		};
	}
}

#endif
