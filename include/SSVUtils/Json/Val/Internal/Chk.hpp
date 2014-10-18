// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CHECKER
#define SSVU_JSON_VAL_INTERNAL_CHECKER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			// Tuple checking implementation
			struct TplIsHelper
			{
				template<SizeT TI, typename TTpl> using TplArg = TplElem<TI, RemoveAll<TTpl>>;

				template<SizeT TI = 0, typename... TArgs> inline static EnableIf<TI == sizeof...(TArgs), bool> isTpl(const Val&) noexcept { return true; }
				template<SizeT TI = 0, typename... TArgs> inline static EnableIf<TI < sizeof...(TArgs), bool> isTpl(const Val& mV) noexcept
				{
					SSVU_ASSERT(mV.is<Arr>() && mV.getArr().size() > TI);
					if(!mV[TI].isNoNum<TplArg<TI, std::tuple<TArgs...>>>()) return false;
					return isTpl<TI + 1, TArgs...>(mV);
				}

				/// @brief Returns `true` if all items of an `Arr` are of type `T`.
				template<typename T> inline static bool areArrItemsOfType(const Val& mV) noexcept
				{
					SSVU_ASSERT(mV.is<Arr>());
					for(const auto& v : mV.getArr()) if(!v.template isNoNum<T>()) return false;
					return true;
				}
			};

			template<typename T> struct Chk
			{
				inline static bool is(const Val&) noexcept { return true; }
			};
			template<typename T> struct ChkNoNum
			{
				inline static bool is(const Val& mV) noexcept { return Chk<RemoveAll<T>>::is(mV); }
			};

			#define SSVJ_DEFINE_CHK_NUM_DISALLOWED(mType) \
				template<> struct Chk<mType> final { };

			#define SSVJ_DEFINE_CHK_NUM_REPR(mType) \
				template<> struct Chk<mType> final { inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Num && mV.getNum().getRepr() == SSVPP_EXPAND(Num::Repr::mType); } };

			#define SSVJ_DEFINE_CHK_BASIC(mType) \
				template<> struct Chk<mType> final \
				{ \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			#define SSVJ_DEFINE_CHKNONUM(mType) \
				template<> struct ChkNoNum<mType> { inline static bool is(const Val& mV) noexcept { return Chk<Num>::is(mV); } };

			// Define disallowed `is<...>` numeric type checks
			SSVJ_DEFINE_CHK_NUM_DISALLOWED(char)
			SSVJ_DEFINE_CHK_NUM_DISALLOWED(int)
			SSVJ_DEFINE_CHK_NUM_DISALLOWED(unsigned char)
			SSVJ_DEFINE_CHK_NUM_DISALLOWED(unsigned int)
			SSVJ_DEFINE_CHK_NUM_DISALLOWED(float)

			// Define allowed `is<...>` numeric representation checks
			SSVJ_DEFINE_CHK_NUM_REPR(IntS)
			SSVJ_DEFINE_CHK_NUM_REPR(IntU)
			SSVJ_DEFINE_CHK_NUM_REPR(Real)

			// Define basic checks
			SSVJ_DEFINE_CHK_BASIC(Obj)
			SSVJ_DEFINE_CHK_BASIC(Arr)
			SSVJ_DEFINE_CHK_BASIC(Str)
			SSVJ_DEFINE_CHK_BASIC(Num)
			SSVJ_DEFINE_CHK_BASIC(Bln)
			SSVJ_DEFINE_CHK_BASIC(Nll)

			// Define numeric type checks that ignore representation (always return true if the `Val` is a `Num`)
			SSVJ_DEFINE_CHKNONUM(char)
			SSVJ_DEFINE_CHKNONUM(int)
			SSVJ_DEFINE_CHKNONUM(long int)
			SSVJ_DEFINE_CHKNONUM(unsigned char)
			SSVJ_DEFINE_CHKNONUM(unsigned int)
			SSVJ_DEFINE_CHKNONUM(unsigned long int)
			SSVJ_DEFINE_CHKNONUM(float)
			SSVJ_DEFINE_CHKNONUM(double)

			#undef SSVJ_DEFINE_CHK_NUM_DISALLOWED
			#undef SSVJ_DEFINE_CHK_NUM_REPR
			#undef SSVJ_DEFINE_CHK_BASIC
			#undef SSVJ_DEFINE_CHKNONUM

			// Check `Val` against itself
			template<> struct Chk<Val> final
			{
				inline static bool is(const Val&) noexcept { return true; }
			};

			// Check C-style string
			template<SizeT TS> struct Chk<char[TS]> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str && mV.getStr().size() == TS; }
			};

			// Check C-style string
			template<> struct Chk<const char*> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};

			// Check `std::pair`
			template<typename T1, typename T2> struct Chk<std::pair<T1, T2>> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == 2 && mV[0].isNoNum<T1>() && mV[1].isNoNum<T2>(); }
			};

			// Check `std::tuple`
			template<typename... TArgs> struct Chk<std::tuple<TArgs...>> final
			{
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == sizeof...(TArgs) && TplIsHelper::isTpl<0, TArgs...>(mV);
				}
			};

			// Check `std::vector`
			template<typename TItem> struct Chk<std::vector<TItem>> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && TplIsHelper::areArrItemsOfType<TItem>(mV); }
			};

			// Check C-style array
			template<typename TItem, SizeT TS> struct Chk<TItem[TS]> final
			{
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == TS && TplIsHelper::areArrItemsOfType<TItem>(mV);
				}
			};
		}
	}
}

#endif
