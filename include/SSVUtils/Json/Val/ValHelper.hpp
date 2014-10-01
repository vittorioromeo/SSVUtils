// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_VALHELPER
#define SSVU_JSON_VAL_VALHELPER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			#define SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(mType) \
				template<> struct ValHelper<mType> \
				{ \
					inline static void set(Val& mV, const mType& mX) noexcept { mV.setNum(Num{mX}); } \
					inline static decltype(auto) as(const Val& mV) noexcept { return mV.getNum().as<mType>(); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Num; } \
				};

			// TODO: try getTypeMove() && std::move(...) ?
			#define SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE(mType) \
				template<> struct ValHelper<mType> \
				{ \
					template<typename T> inline static void set(Val& mV, T&& mX) noexcept(noexcept(SSVPP_CAT(mV.set, mType)(fwd<T>(mX)))) { SSVPP_CAT(mV.set, mType)(fwd<T>(mX)); } \
					inline static const auto& as(const Val& mV)	noexcept(noexcept(SSVPP_CAT(mV.get, mType)())) { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto as(Val&& mV)				noexcept(noexcept(SSVPP_CAT(mV.get, mType)())) { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			#define SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE(mType) \
				template<> struct ValHelper<mType> \
				{ \
					inline static void set(Val& mV, const mType& mX) noexcept { SSVPP_CAT(mV.set, mType)(mX); } \
					inline static decltype(auto) as(const Val& mV) noexcept { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(char)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(int)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(long int)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(unsigned char)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(unsigned int)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(unsigned long int)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(float)
			SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL(double)

			SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE(Obj)
			SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE(Arr)
			SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE(Str)

			SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE(Num)
			SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE(Bln)
			SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE(Nll)

			#undef SSVU_JSON_DEFINE_VALHELPER_NUM_IMPL
			#undef SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE
			#undef SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE

			template<> struct ValHelper<Val>
			{
				template<typename T> inline static void set(Val& mV, T&& mX) noexcept(noexcept(mV.init(fwd<T>(mX)))) { mV.init(fwd<T>(mX)); }
				inline static const auto& as(const Val& mV) noexcept { return mV; }
				inline static auto as(Val&& mV) noexcept { return mV; }
				inline static auto& as(Val& mV) noexcept { return mV; }
				inline static auto is(const Val& mV) noexcept { return true; }
			};

			template<std::size_t TS> struct ValHelper<char[TS]>
			{
				inline static void set(Val& mV, const char(&mX)[TS]) { mV.setStr(mX); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};

			template<> struct ValHelper<const char*>
			{
				inline static void set(Val& mV, const char* mX) { mV.setStr(mX); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};

			template<typename TK, typename TV> struct ValHelper<std::pair<TK, TV>>
			{
				using Type = std::pair<TK, TV>;

				inline static void set(Val& mV, const Type& mX) { mV.setArr(Arr{{mX.first}, {mX.second}}); }
				inline static auto as(const Val& mV) { return std::make_pair(mV[0].as<TK>(), mV[1].as<TV>()); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == 2 && mV[0].is<TK>() && mV[1].is<TV>(); }
			};

			namespace Impl
			{
				template<std::size_t TI, typename TTpl> using TplArg = TupleElem<TI, RemoveAll<TTpl>>;

				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI == sizeof...(TArgs)> toTpl(const Val&, std::tuple<TArgs...>&) { }
				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI < sizeof...(TArgs)> toTpl(const Val& mVal, std::tuple<TArgs...>& mTpl)
				{
					std::get<TI>(mTpl) = mVal[TI].as<TplArg<TI, decltype(mTpl)>>();
					toTpl<TI + 1, TArgs...>(mVal, mTpl);
				}

				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI == sizeof...(TArgs)> fromTpl(Arr&, const std::tuple<TArgs...>&) { }
				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI < sizeof...(TArgs)> fromTpl(Arr& mArr, const std::tuple<TArgs...>& mTpl)
				{
					mArr.emplace_back(std::get<TI>(mTpl));
					fromTpl<TI + 1, TArgs...>(mArr, mTpl);
				}

				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI == sizeof...(TArgs), bool> isTpl(const Val&) { return true; }
				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI < sizeof...(TArgs), bool> isTpl(const Val& mVal)
				{
					if(!mVal[TI].is<TplArg<TI, std::tuple<TArgs...>>>()) return false;
					return isTpl<TI + 1, TArgs...>(mVal);
				}
			}

			template<typename... TArgs> struct ValHelper<std::tuple<TArgs...>>
			{
				using Type = std::tuple<TArgs...>;

				inline static void set(Val& mV, const Type& mX)
				{
					Arr result;
					Impl::fromTpl(result, mX);
					mV.setArr(std::move(result));
				}
				inline static auto as(const Val& mV)
				{
					Type result;
					Impl::toTpl(mV, result);
					return result;
				}
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == sizeof...(TArgs) && Impl::isTpl<0, TArgs...>(mV);
				}
			};

			template<typename T> struct ValHelper<std::vector<T>>
			{
				using Type = std::vector<T>;

				inline static void set(Val& mV, const Type& mX)	{ Arr result; for(const auto& v : mX) result.emplace_back(v); mV.setArr(std::move(result)); }
				inline static void set(Val& mV, Type&& mX)		{ Arr result; for(const auto& v : mX) result.emplace_back(std::move(v)); mV.setArr(std::move(result)); }
				inline static auto as(const Val& mV)	{ Type result; for(const auto& v : mV.forArrAs<T>()) result.emplace_back(v); return result; }
				inline static auto as(Val&& mV)			{ Type result; for(auto& v : mV.forArrAs<T>()) result.emplace_back(std::move(v)); return result; }
				inline static auto is(const Val& mV)
				{
					if(mV.getType() != Val::Type::Arr) return false;
					for(const auto& v : mV.forArr()) if(!v.template is<T>()) return false;
					return true;
				}
			};

			template<typename TItem, std::size_t TS> struct ValHelper<TItem[TS]>
			{
				using Type = TItem[TS];

				inline static void set(Val& mV, const Type& mX)	{ Arr result; for(auto i(0u); i < TS; ++i) result.emplace_back(mX[i]); mV.setArr(std::move(result)); }
				inline static void set(Val& mV, Type&& mX)		{ Arr result; for(auto i(0u); i < TS; ++i) result.emplace_back(std::move(mX[i])); mV.setArr(std::move(result)); }
				inline static auto as(const Val& mV)	{ Type result; for(auto i(0u); i < TS; ++i) result[i] = mV[i].as<TItem>(); return result; }
				inline static auto as(Val&& mV)			{ Type result; for(auto i(0u); i < TS; ++i) result[i] = std::move(mV[i].as<TItem>()); return result; }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == TS; }
			};
		}
	}
}

#endif

