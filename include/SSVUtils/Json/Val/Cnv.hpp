// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_CNV
#define SSVU_JSON_VAL_CNV

/// @macro Class mixin that allows SSVJ converters to access the current class's private members.
#define SSVJ_CNV_FRIEND() template<typename> friend struct ssvu::Json::Internal::Cnv

/// @macro Shortcut to serialize a class member as an object with the same name as the member.
#define SSVJ_CNV_OBJ_AUTO(mValue, mVar) #mVar, mValue.mVar

/// @macro Defines a simple converter template specialization to convert classes that do not require special behavior.
#define SSVJ_CNV_SIMPLE(mType, mVName, mXName) \
	struct Cnv<mType> final : CnvImplSimple<mType> \
	{ \
		template<typename TV, typename TX> inline static void impl(TV mVName, TX mXName)

/// @macro End macro, required after defining a converter.
#define SSVJ_CNV_END() }

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename TNum> struct CnvNumImpl
			{
				inline static decltype(auto) getFromVal(const Val& mV) noexcept { return mV.getNum().as<TNum>(); }
				inline static void toVal(Val& mV, const TNum& mX) noexcept { mV.setNum(Num{mX}); }
				inline static void fromVal(const Val& mV, TNum& mX) noexcept { mX = getFromVal(mV); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Num; }
			};

			#define SSVU_JSON_DEFINE_CNV_NUM_IMPL(mType) template<> struct Cnv<mType> final : public CnvNumImpl<mType> { };

			// TODO: try getTypeMove() && std::move(...) ?
			#define SSVU_JSON_DEFINE_CNV_BIG_MUTABLE(mType) \
				template<> struct Cnv<mType> final \
				{ \
					inline static const auto& getFromVal(const Val& mV) noexcept { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto getFromVal(Val&& mV) noexcept { return SSVPP_CAT(mV.get, mType)(); } \
					template<typename T> inline static void toVal(Val& mV, T&& mX) noexcept(noexcept(SSVPP_CAT(mV.set, mType)(fwd<T>(mX)))) { SSVPP_CAT(mV.set, mType)(fwd<T>(mX)); } \
					template<typename T> inline static void fromVal(Val&& mV, T& mX) { return getFromVal(fwd<T>(mV)); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			#define SSVU_JSON_DEFINE_CNV_SMALL_IMMUTABLE(mType) \
				template<> struct Cnv<mType> final \
				{ \
					inline static decltype(auto) getFromVal(const Val& mV) noexcept { return SSVPP_CAT(mV.get, mType)(); } \
					inline static void toVal(Val& mV, const mType& mX) noexcept { SSVPP_CAT(mV.set, mType)(mX); } \
					inline static void fromVal(const Val& mV, mType& mX) noexcept { mX = getFromVal(mV); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			SSVU_JSON_DEFINE_CNV_NUM_IMPL(char)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(int)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(long int)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(unsigned char)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(unsigned int)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(unsigned long int)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(float)
			SSVU_JSON_DEFINE_CNV_NUM_IMPL(double)

			SSVU_JSON_DEFINE_CNV_BIG_MUTABLE(Obj)
			SSVU_JSON_DEFINE_CNV_BIG_MUTABLE(Arr)
			SSVU_JSON_DEFINE_CNV_BIG_MUTABLE(Str)

			SSVU_JSON_DEFINE_CNV_SMALL_IMMUTABLE(Num)
			SSVU_JSON_DEFINE_CNV_SMALL_IMMUTABLE(Bln)
			SSVU_JSON_DEFINE_CNV_SMALL_IMMUTABLE(Nll)

			#undef SSVU_JSON_DEFINE_CNV_NUM_IMPL
			#undef SSVU_JSON_DEFINE_CNV_BIG_MUTABLE
			#undef SSVU_JSON_DEFINE_CNV_SMALL_IMMUTABLE

			// Convert values to themselvesbr
			template<> struct Cnv<Val> final
			{
				inline static const auto& getFromVal(const Val& mV) noexcept { return mV; }
				inline static auto getFromVal(Val&& mV) noexcept { return mV; }
				inline static auto& getFromVal(Val& mV) noexcept { return mV; }
				template<typename T> inline static void toVal(Val& mV, T&& mX) noexcept(noexcept(mV.init(fwd<T>(mX)))) { mV.init(fwd<T>(mX)); }
				template<typename T> inline static void fromVal(Val&& mV, Val& mX) noexcept { mX = fwd<T>(mV); }
				inline static auto is(const Val& mV) noexcept { return true; }
			};

			// Convert C-style string arrays
			template<std::size_t TS> struct Cnv<char[TS]> final
			{
				inline static void toVal(Val& mV, const char(&mX)[TS]) { mV.setStr(mX); }
				inline static void fromVal(const Val& mV, char(&mX)[TS]) noexcept { for(auto i(0u); i < TS; ++i) mX[i] = mV.as<Str>()[i]; }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str && mV.as<Str>().size() == TS; }
			};

			// Convert C-style strings
			template<> struct Cnv<const char*> final
			{
				inline static void toVal(Val& mV, const char* mX) { mV.setStr(mX); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};

			// Convert `std::pair`
			template<typename T1, typename T2> struct Cnv<std::pair<T1, T2>> final
			{
				using Type = std::pair<T1, T2>;

				template<typename T> inline static void toVal(Val& mV, T&& mX)
				{
					mV.setArr(Arr
					{
						Val{moveIfRValue<decltype(mX)>(mX.first)},
						Val{moveIfRValue<decltype(mX)>(mX.second)}
					});
				}
				template<typename T> inline static void fromVal(T&& mV, Type& mX)
				{
					mX.first = moveIfRValue<decltype(mV)>(mV[0].template as<T1>());
					mX.second = moveIfRValue<decltype(mV)>(mV[1].template as<T2>());
				}
				template<typename T> inline static auto getFromVal(T&& mV) { Type result; fromVal(fwd<T>(mV), result); return result; }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == 2 && mV[0].is<T1>() && mV[1].is<T2>(); }
			};

			// Tuple conversion implementation
			namespace Impl
			{
				template<std::size_t TI, typename TTpl> using TplArg = TupleElem<TI, RemoveAll<TTpl>>;

				template<std::size_t TI = 0, typename... TArgs, typename T> inline EnableIf<TI == sizeof...(TArgs)> toTpl(T&&, std::tuple<TArgs...>&) { }
				template<std::size_t TI = 0, typename... TArgs, typename T> inline EnableIf<TI < sizeof...(TArgs)> toTpl(T&& mV, std::tuple<TArgs...>& mX)
				{
					std::get<TI>(mX) = moveIfRValue<decltype(mV)>(mV[TI].template as<TplArg<TI, decltype(mX)>>());
					toTpl<TI + 1, TArgs...>(fwd<T>(mV), mX);
				}

				template<std::size_t TI = 0, typename... TArgs, typename T> inline EnableIf<TI == sizeof...(TArgs)> fromTpl(Arr&, T&&) { }
				template<std::size_t TI = 0, typename... TArgs, typename T> inline EnableIf<TI < sizeof...(TArgs)> fromTpl(Arr& mArr, T&& mX)
				{
					mArr.emplace_back(moveIfRValue<decltype(mX)>(std::get<TI>(mX))); // TODO: emplace_back or []?
					fromTpl<TI + 1, TArgs...>(mArr, fwd<T>(mX));
				}

				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI == sizeof...(TArgs), bool> isTpl(const Val&) noexcept { return true; }
				template<std::size_t TI = 0, typename... TArgs> inline EnableIf<TI < sizeof...(TArgs), bool> isTpl(const Val& mV) noexcept
				{
					if(!mV[TI].is<TplArg<TI, std::tuple<TArgs...>>>()) return false;
					return isTpl<TI + 1, TArgs...>(mV);
				}
			}

			// Convert `std::tuple`
			template<typename... TArgs> struct Cnv<std::tuple<TArgs...>> final
			{
				using Type = std::tuple<TArgs...>;

				template<typename T> inline static void toVal(Val& mV, T&& mX)
				{
					Arr result; result.reserve(sizeof...(TArgs));
					Impl::fromTpl<0, TArgs...>(result, fwd<T>(mX));
					mV.setArr(std::move(result));
				}
				template<typename T> inline static void fromVal(T&& mV, Type& mX) { Impl::toTpl<0, TArgs...>(fwd<T>(mV), mX); }
				template<typename T> inline static auto getFromVal(T&& mV) { Type result; fromVal(fwd<T>(mV), result); return result; }
				inline static auto is(const Val& mV) noexcept
				{
					return mV.getType() == Val::Type::Arr && mV.getArr().size() == sizeof...(TArgs) && Impl::isTpl<0, TArgs...>(mV);
				}
			};

			/// @brief Returns `true` if all items of an `Arr` are of type `T`.
			template<typename T> inline bool areArrItemsOfType(const Val& mV) noexcept
			{
				SSVU_ASSERT(mV.is<Arr>());
				for(const auto& v : mV.forArr()) if(!v.template is<T>()) return false;
				return true;
			}

			// Convert `std::vector`
			template<typename TItem> struct Cnv<std::vector<TItem>> final
			{
				using Type = std::vector<TItem>;

				template<typename T> inline static void toVal(Val& mV, T&& mX)
				{
					Arr result; result.reserve(mX.size());
					for(const auto& v : mX) result.emplace_back(moveIfRValue<decltype(mX)>(v));
					mV.setArr(std::move(result));
				}
				template<typename T> inline static void fromVal(T&& mV, Type& mX)
				{
					const auto& arr(mV.template as<Arr>());
					SSVU_ASSERT(mX.size() >= arr.size());
					for(auto i(0u); i < arr.size(); ++i) mX[i] = moveIfRValue<decltype(mV)>(arr[i]);
				}
				template<typename T> inline static Type getFromVal(T&& mV)
				{
					const auto& arr(mV.template as<Arr>());
					Type result; result.reserve(arr.size());
					for(auto i(0u); i < arr.size(); ++i) result.emplace_back(moveIfRValue<decltype(mV)>(arr[i].template as<TItem>()));
					return result;
				}
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && areArrItemsOfType<TItem>(mV); }
			};

			// Convert C-style array
			template<typename TItem, std::size_t TS> struct Cnv<TItem[TS]>
			{
				using Type = TItem[TS];

				template<typename T> inline static void toVal(Val& mV, T&& mX)
				{
					Arr result; result.reserve(TS);
					for(auto i(0u); i < TS; ++i) result.emplace_back(moveIfRValue<decltype(mX)>(mX[i]));
					mV.setArr(std::move(result));
				}
				template<typename T> inline static void fromVal(T&& mV, Type& mX)
				{
					SSVU_ASSERT(mV.template as<Arr>().size() >= TS);
					for(auto i(0u); i < TS; ++i) mX[i] = moveIfRValue<decltype(mV)>(mV[i].template as<TItem>());
				}
				template<typename T> inline static auto getFromVal(T&& mV) { Type result; fromVal(fwd<T>(mV), result); return result; }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Arr && mV.getArr().size() == TS && areArrItemsOfType<TItem>(mV); }
			};

			template<typename T> struct CnvImplGet
			{
				template<typename TFwd> inline static auto getFromVal(TFwd&& mV) { T result; Cnv<T>::template fromVal(fwd<TFwd>(mV), result); return result; }
			};

			template<typename T> struct CnvImplSimple : public CnvImplGet<T>
			{
				template<typename TFwd> inline static void toVal(Val& mV, TFwd&& mX) { Cnv<T>::template impl<decltype(mV), decltype(mX)>(mV, fwd<TFwd>(mX)); }
				template<typename TFwd> inline static void fromVal(TFwd&& mV, T& mX) { Cnv<T>::template impl<decltype(mV), decltype(mX)>(fwd<TFwd>(mV), mX); }
			};
		}
	}
}

#endif

