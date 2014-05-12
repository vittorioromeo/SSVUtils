#ifndef SSVU_HANDLEVECTOR_INTERNAL_ATOM
#define SSVU_HANDLEVECTOR_INTERNAL_ATOM

#include "SSVUtils/HandleVector/Internal/Uncertain.hpp"

namespace ssvu
{
	namespace Internal
	{
		/// @brief Internal HandleVector class storing data and metadata.
		template<typename T> class Atom
		{
			template<typename> friend class HandleVector;

			private:
				Uncertain<T> data;	///< @brief Data being stored.
				HIdx markIdx;		///< @brief Index of the mark controlling this atom.
				bool alive{false};	///< @brief Status of the atom.

				/// @brief Allocates and constructs the data. Assumes the atom is not alive.
				template<typename... TArgs> inline void initData(TArgs&&... mArgs)
					noexcept(noexcept(data.init(std::forward<TArgs>(mArgs)...)))
				{
					SSVU_ASSERT(!alive);
					data.init(std::forward<TArgs>(mArgs)...);
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
		};
	}
}

#endif
