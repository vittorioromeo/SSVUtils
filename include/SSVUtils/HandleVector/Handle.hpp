// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_HANDLE
#define SSVU_HANDLEVECTOR_HANDLE

namespace ssvu
{
	/// @brief Handle class that points to HandleVector elements.
	template<typename T> class Handle
	{
		template<typename> friend class HandleVector;

		public:
			/// @typedef Templatized `Internal::Atom<T>` type;
			using Atom = typename Internal::Atom<T>;

		private:
			/// @brief Internal pointer to the HandleVector.
			HandleVector<T>* hVec;

			/// @brief Index of the mark to check.
			HIdx markIdx;

			/// @brief Counter of the handle. Will be compared to the mark's counter.
			HCtr ctr;

			inline Handle(HandleVector<T>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : hVec(&mHVec), markIdx{mMarkIdx}, ctr{mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the atom.
			template<typename TT> inline TT getAtomImpl() noexcept
			{
				SSVU_ASSERT(isAlive());
				return hVec->getAtomFromMark(hVec->marks[markIdx]);
			}

		public:
			/// @brief Returns a reference to the atom. Assumes the handle is valid.
			inline auto& getAtom() noexcept { return getAtomImpl<Atom&>(); }

			/// @brief Returns a const reference to the atom. Assumes the handle is valid.
			inline const auto& getAtom() const noexcept { return getAtomImpl<const Atom&>(); }

			/// @brief Returns a reference to the data. Assumes the handle is valid.
			inline T& get() noexcept { return getAtom().getData(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			inline const T& get() const noexcept { return getAtom().getData(); }

			/// @brief Returns whether the handle is valid or not.
			/// @details The handle is considered valid only when it points to the atom it originally pointed to.
			bool isAlive() const noexcept;

			/// @brief Sets the pointed atom's status as dead.
			void destroy() noexcept;

			// Pointer-like interface
			inline T& operator*() noexcept				{ return get(); }
			inline const T& operator*() const noexcept	{ return get(); }
			inline T* operator->() noexcept				{ return &(get()); }
			inline const T* operator->() const noexcept	{ return &(get()); }
	};
}

#endif
