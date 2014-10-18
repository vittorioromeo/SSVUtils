// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_NUMBER
#define SSVU_JSON_NUMBER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			/// @brief Helper template for `Num` setters and getters.
			template<typename> struct NumHelper;

			/// @briewf Helper template to convert numeric types to representation types.
			template<typename> struct ReprHelper;

			/// @brief Internal class dedicated to the storage of numeric values.
			class Num
			{
				template<typename> friend struct Internal::NumHelper;

				public:
					/// @brief Representation/storage type of numeric values.
					enum class Repr{IntS, IntU, Real};

				private:
					/// @brief Current representation in use.
					Repr repr;

					/// @brief Union POD storage for representation types.
					UnionVariantPOD<IntS, IntU, Real> h;

					// Union setters
					inline void setIntS(const IntU& mX) noexcept { repr = Repr::IntS; h.init<IntS>(mX); }
					inline void setIntU(const IntS& mX) noexcept { repr = Repr::IntU; h.init<IntU>(mX); }
					inline void setReal(const Real& mX) noexcept { repr = Repr::Real; h.init<Real>(mX); }

					/// @brief Returns the stored value as an `IntS`. The value is copied and casted if necessary.
					inline IntS getIntS() const noexcept
					{
						switch(repr)
						{
							case Repr::IntS: return h.get<IntS>();
							case Repr::IntU: return static_cast<IntS>(h.get<IntU>());
							case Repr::Real: return static_cast<IntS>(h.get<Real>());
							default: SSVU_UNREACHABLE();
						}
					}

					/// @brief Returns the stored value as an `IntU`. The value is copied and casted if necessary.
					inline IntU getIntU() const noexcept
					{
						switch(repr)
						{
							case Repr::IntS: return static_cast<IntU>(h.get<IntS>());
							case Repr::IntU: return h.get<IntU>();
							case Repr::Real: return static_cast<IntU>(h.get<Real>());
							default: SSVU_UNREACHABLE();
						}
					}

					/// @brief Returns the stored value as a `Real`. The value is copied and casted if necessary.
					inline Real getReal() const noexcept
					{
						switch(repr)
						{
							case Repr::IntS: return static_cast<Real>(h.get<IntS>());
							case Repr::IntU: return static_cast<Real>(h.get<IntU>());
							case Repr::Real: return h.get<Real>();
							default: SSVU_UNREACHABLE();
						}
					}

				public:
					inline Num() noexcept = default;

					/// @brief Constructs `Num` from the number `mX`.
					template<typename T> inline Num(const T& mX) noexcept { set<T>(mX); }

					/// @brief Sets the stored number to `mX`. Representation may change depending on `mX`'s type.
					template<typename T> inline void set(const T& mX) noexcept { Internal::NumHelper<T>::set(*this, mX); }

					/// @brief Returns a copy of the stored number, as a `T`. Representation will not change.
					/// @details The internal number will not change. The copy will be casted, if necessary.
					template<typename T> inline auto as() const noexcept { return Internal::NumHelper<T>::as(*this); }

					/// @brief Returns the current representation type.
					inline auto getRepr() const noexcept { return repr; }

					// Equality/inequality
					inline bool operator==(const Num& mN) const noexcept
					{
						switch(repr)
						{
							case Repr::IntS: return getIntS() == mN.getIntS();
							case Repr::IntU: return getIntU() == mN.getIntU();
							case Repr::Real: return getReal() == mN.getReal();
							default: SSVU_UNREACHABLE();
						}
					}
					inline auto operator!=(const Num& mN) const noexcept { return !(operator==(mN)); }
			};
		}
	}
}

#include "SSVUtils/Json/Num/NumHelper.hpp"

#endif
