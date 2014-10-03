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
			namespace Impl
			{
				template<std::size_t TI, typename TTpl> using TplArg = TupleElem<TI, RemoveAll<TTpl>>;

				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI == sizeof...(TArgs), bool> isTpl(const Val&) noexcept { return true; }
				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI < sizeof...(TArgs), bool> isTpl(const Val& mV) noexcept
				{
					if(!mV[TI].is<TplArg<TI, std::tuple<TArgs...>>>()) return false;
					return isTpl<TI + 1, TArgs...>(mV);
				}

				/// @brief Returns `true` if all items of an `Arr` are of type `T`.
				template<typename T> inline bool areArrItemsOfType(const Val& mV) noexcept
				{
					SSVU_ASSERT(mV.is<Arr>());
					for(const auto& v : mV.forArr()) if(!v.template is<T>()) return false;
					return true;
				}
			}

			template<typename T> struct Checker
			{
				inline static bool is(const Val&) noexcept { return true; }
			};

			#define SSVU_JSON_DEFINE_CHECKER_NUM(mType) \
				template<> struct Checker<mType> final \
				{ \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Num; } \
				}; \

			#define SSVU_JSON_DEFINE_CHECKER_BASIC(mType) \
				template<> struct Checker<mType> final \
				{ \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			SSVU_JSON_DEFINE_CHECKER_NUM(char)
			SSVU_JSON_DEFINE_CHECKER_NUM(int)
			SSVU_JSON_DEFINE_CHECKER_NUM(long int)
			SSVU_JSON_DEFINE_CHECKER_NUM(unsigned char)
			SSVU_JSON_DEFINE_CHECKER_NUM(unsigned int)
			SSVU_JSON_DEFINE_CHECKER_NUM(unsigned long int)
			SSVU_JSON_DEFINE_CHECKER_NUM(float)
			SSVU_JSON_DEFINE_CHECKER_NUM(double)

			SSVU_JSON_DEFINE_CHECKER_BASIC(Obj)
			SSVU_JSON_DEFINE_CHECKER_BASIC(Arr)
			SSVU_JSON_DEFINE_CHECKER_BASIC(Str)
			SSVU_JSON_DEFINE_CHECKER_BASIC(Num)
			SSVU_JSON_DEFINE_CHECKER_BASIC(Bln)
			SSVU_JSON_DEFINE_CHECKER_BASIC(Nll)

			#undef SSVU_JSON_DEFINE_CHECKER_NUM
			#undef SSVU_JSON_DEFINE_CHECKER_BASIC

			template<> struct Checker<Val> final
			{
				inline static bool is(const Val&) noexcept { return true; }
			};

			template<std::size_t TS> struct Checker<char[TS]> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str && mV.getStr().size() == TS; }
			};

			template<> struct Checker<const char*> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};

			template<typename T1, typename T2> struct Checker<std::pair<T1, T2>> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == 2 && mV[0].is<T1>() && mV[1].is<T2>(); }
			};

			template<typename... TArgs> struct Checker<std::tuple<TArgs...>> final
			{
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == sizeof...(TArgs) && Impl::isTpl<0, TArgs...>(mV);
				}
			};

			template<typename TItem> struct Checker<std::vector<TItem>> final
			{
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && Impl::areArrItemsOfType<TItem>(mV); }
			};

			template<typename TItem, std::size_t TS> struct Checker<TItem[TS]> final
			{
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == TS && Impl::areArrItemsOfType<TItem>(mV);
				}
			};
		}
	}
}

#endif
