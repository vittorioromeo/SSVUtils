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
					inline static void set(Val& mV, const mType& mX) { mV.setNum(Num{mX}); } \
					inline static decltype(auto) as(const Val& mV) { return mV.getNum().as<mType>(); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Num; } \
				};

			#define SSVU_JSON_DEFINE_VALHELPER_BIG_MUTABLE(mType) \
				template<> struct ValHelper<mType> \
				{ \
					template<typename T> inline static void set(Val& mV, T&& mX) { SSVPP_CAT(mV.set, mType)(fwd<T>(mX)); } \
					inline static const auto& as(const Val& mV) { return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto as(Val&& mV) { return SSVPP_CAT(std::move(mV).get, mType, Move)(); } \
					inline static auto is(const Val& mV) noexcept { return mV.getType() == SSVPP_EXPAND(Val::Type::mType); } \
				};

			#define SSVU_JSON_DEFINE_VALHELPER_SMALL_IMMUTABLE(mType) \
				template<> struct ValHelper<mType> \
				{ \
					inline static void set(Val& mV, const mType& mX) { SSVPP_CAT(mV.set, mType)(mX); } \
					inline static decltype(auto) as(const Val& mV) { return SSVPP_CAT(mV.get, mType)(); } \
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
				template<typename T> inline static void set(Val& mV, T&& mX) { mV.init(fwd<T>(mX)); }
				inline static const auto& as(const Val& mV) noexcept { return mV; }
				inline static auto& as(Val& mV) noexcept { return mV; }
			};

			template<std::size_t TS> struct ValHelper<char[TS]>
			{
				inline static void set(Val& mV, const char(&mX)[TS]) { mV.setStr(mX); }
				inline static auto is(const Val& mV) noexcept { return mV.getType() == Val::Type::Str; }
			};
		}
	}
}

#endif

