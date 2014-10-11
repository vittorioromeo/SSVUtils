// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_ASHELPER
#define SSVU_JSON_VAL_INTERNAL_ASHELPER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename T> struct AsHelper
			{
				// By default: make a copy, fill it, and return it
				template<typename TFwd> inline static auto as(TFwd&& mV)
				{
					T result;
					Cnv<RemoveAll<T>>::fromVal(fwd<TFwd>(mV), result);
					return result;
				}
			};

			#define SSVU_JSON_DEFINE_ASHELPER_NUM(mType) \
				template<> struct AsHelper<mType> final \
				{ \
					template<typename T> inline static auto as(T&& mV) { return mV.getNum().template as<mType>(); } \
				};

			#define SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE(mType) \
				template<> struct AsHelper<mType> final \
				{ \
					inline static const auto& as(const Val& mV) noexcept	{ return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto as(Val&& mV) noexcept				{ return SSVPP_CAT(mV.get, mType)(); } \
					inline static auto& as(Val& mV) noexcept				{ return SSVPP_CAT(mV.get, mType)(); } \
				};

			#define SSVU_JSON_DEFINE_ASHELPER_SMALL_IMMUTABLE(mType) \
				template<> struct AsHelper<mType> final \
				{ \
					inline static auto as(const Val& mV) noexcept { return SSVPP_CAT(mV.get, mType)(); } \
				};

			SSVU_JSON_DEFINE_ASHELPER_NUM(char)
			SSVU_JSON_DEFINE_ASHELPER_NUM(int)
			SSVU_JSON_DEFINE_ASHELPER_NUM(long int)
			SSVU_JSON_DEFINE_ASHELPER_NUM(unsigned char)
			SSVU_JSON_DEFINE_ASHELPER_NUM(unsigned int)
			SSVU_JSON_DEFINE_ASHELPER_NUM(unsigned long int)
			SSVU_JSON_DEFINE_ASHELPER_NUM(float)
			SSVU_JSON_DEFINE_ASHELPER_NUM(double)

			SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE(Obj)
			SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE(Arr)
			SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE(Str)
			SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE(Num)

			SSVU_JSON_DEFINE_ASHELPER_SMALL_IMMUTABLE(Bln)
			SSVU_JSON_DEFINE_ASHELPER_SMALL_IMMUTABLE(Nll)

			#undef SSVU_JSON_DEFINE_ASHELPER_NUM
			#undef SSVU_JSON_DEFINE_ASHELPER_BIG_MUTABLE
			#undef SSVU_JSON_DEFINE_ASHELPER_SMALL_IMMUTABLE

			template<> struct AsHelper<Val> final
			{
				inline static const auto& as(const Val& mV) noexcept	{ return mV; }
				inline static auto as(Val&& mV) noexcept				{ return mV; }
				inline static auto& as(Val& mV) noexcept				{ return mV; }
			};

			template<typename TItem> struct AsHelper<std::vector<TItem>> final
			{
				template<typename T> inline static auto as(T&& mV)
				{
					const auto& arr(mV.template as<Arr>());
					std::vector<TItem> result; result.reserve(arr.size());
					for(auto i(0u); i < arr.size(); ++i) result.emplace_back(moveIfRValue<decltype(mV)>(arr[i].template as<TItem>()));
					return result;
				}
			};
		}
	}
}

#endif
