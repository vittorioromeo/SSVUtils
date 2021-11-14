// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CNV
#define SSVU_JSON_VAL_INTERNAL_CNV

#include "SSVUtils/Json/Val/Val.hpp"

#include <vrm/pp.hpp>

#include <bitset>
#include <vector>
#include <cassert>

namespace ssvu
{
namespace Json
{
namespace Impl
{
#define SSVJ_DEFINE_CNV_NUM(mType)                                    \
    template <>                                                       \
    struct Cnv<mType, void> final                                     \
    {                                                                 \
        inline static void toVal(Val& mV, const mType& mX) noexcept   \
        {                                                             \
            mV.setNum(Num{mX});                                       \
        }                                                             \
        inline static void fromVal(const Val& mV, mType& mX) noexcept \
        {                                                             \
            mX = mV.getNum().as<mType>();                             \
        }                                                             \
    };

#define SSVJ_DEFINE_CNV_BIG_MUTABLE(mType)                                \
    template <>                                                           \
    struct Cnv<mType, void> final                                         \
    {                                                                     \
        template <typename T>                                             \
        inline static void toVal(Val& mV, T&& mX) noexcept(               \
            noexcept(VRM_PP_CAT(mV.set, mType)(FWD(mX))))                 \
        {                                                                 \
            VRM_PP_CAT(mV.set, mType)(FWD(mX));                           \
        }                                                                 \
        template <typename T>                                             \
        inline static void fromVal(T&& mV, mType& mX)                     \
        {                                                                 \
            mX = moveIfRValue<decltype(mV)>(VRM_PP_CAT(mV.get, mType)()); \
        }                                                                 \
    };

#define SSVJ_DEFINE_CNV_SMALL_IMMUTABLE(mType)                        \
    template <>                                                       \
    struct Cnv<mType, void> final                                     \
    {                                                                 \
        inline static void toVal(Val& mV, const mType& mX) noexcept   \
        {                                                             \
            VRM_PP_CAT(mV.set, mType)(mX);                            \
        }                                                             \
        inline static void fromVal(const Val& mV, mType& mX) noexcept \
        {                                                             \
            mX = VRM_PP_CAT(mV.get, mType)();                         \
        }                                                             \
    };

// Define numeric value converters
SSVJ_DEFINE_CNV_NUM(char)
SSVJ_DEFINE_CNV_NUM(int)
SSVJ_DEFINE_CNV_NUM(long int)
SSVJ_DEFINE_CNV_NUM(unsigned char)
SSVJ_DEFINE_CNV_NUM(unsigned int)
SSVJ_DEFINE_CNV_NUM(unsigned long int)
SSVJ_DEFINE_CNV_NUM(float)
SSVJ_DEFINE_CNV_NUM(double)

// Define `Obj`, `Arr`, `Str` and `Num` converters
SSVJ_DEFINE_CNV_BIG_MUTABLE(Obj)
SSVJ_DEFINE_CNV_BIG_MUTABLE(Arr)
SSVJ_DEFINE_CNV_BIG_MUTABLE(Str)
SSVJ_DEFINE_CNV_BIG_MUTABLE(Num)

// Define other converters
SSVJ_DEFINE_CNV_SMALL_IMMUTABLE(Bln)
SSVJ_DEFINE_CNV_SMALL_IMMUTABLE(Nll)

#undef SSVJ_DEFINE_CNV_NUM
#undef SSVJ_DEFINE_CNV_BIG_MUTABLE
#undef SSVJ_DEFINE_CNV_SMALL_IMMUTABLE

// Convert values to themselves
template <>
struct Cnv<Val, void> final
{
    template <typename T>
    inline static void toVal(Val& mV, T&& mX) noexcept(
        noexcept(mV.init(FWD(mX))))
    {
        mV.init(FWD(mX));
    }
};

template <typename T, typename = void>
struct Cnv final
{
};

// Convert enums
template <typename T>
struct Cnv<T, std::enable_if_t<
                  std::is_enum_v<std::remove_cv_t<std::remove_reference_t<T>>>>>
    final
{
    inline static void toVal(Val& mV, const T& mX) noexcept
    {
        mV = std::underlying_type_t<T>(mX);
    }
    inline static void fromVal(const Val& mV, T& mX) noexcept
    {
        mX = T(mV.template as<std::underlying_type_t<T>>());
    }
};

// Convert C-style string arrays
template <std::size_t TS>
struct Cnv<char[TS]> final
{
    inline static void toVal(Val& mV, const char (&mX)[TS])
    {
        mV.setStr(mX);
    }
    inline static void fromVal(const Val& mV, char (&mX)[TS]) noexcept
    {
        for(auto i(0u); i < TS; ++i) mX[i] = mV.getStr()[i];
    }
};

// Convert C-style strings
template <>
struct Cnv<const char*> final
{
    inline static void toVal(Val& mV, const char* mX)
    {
        mV.setStr(mX);
    }
};

// Convert `std::pair`
template <typename T1, typename T2>
struct Cnv<std::pair<T1, T2>> final
{
    using Type = std::pair<T1, T2>;

    template <typename T>
    inline static void toVal(Val& mV, T&& mX)
    {
        mV.setArr(Arr{Val{moveIfRValue<decltype(mX)>(mX.first)},
            Val{moveIfRValue<decltype(mX)>(mX.second)}});
    }
    template <typename T>
    inline static void fromVal(T&& mV, Type& mX)
    {
        mX.first = moveIfRValue<decltype(mV)>(mV[0].template as<T1>());
        mX.second = moveIfRValue<decltype(mV)>(mV[1].template as<T2>());
    }
};

// Convert `std::tuple`
template <typename... TArgs>
struct Cnv<std::tuple<TArgs...>> final
{
    using Type = std::tuple<TArgs...>;

    template <typename T>
    inline static void toVal(Val& mV, T&& mX)
    {
        Arr result;
        result.reserve(sizeof...(TArgs));
        tplFor([&result](auto&& mI) { result.emplace_back(FWD(mI)); }, FWD(mX));
        mV.setArr(std::move(result));
    }
    template <typename T>
    inline static void fromVal(T&& mV, Type& mX)
    {
        tplForData(
            [&mV](auto mD, auto& mE)
            {
                assert(
                    mV.template is<Arr>() && mV.getArr().size() > mD.getIdx());
                mE = moveIfRValue<decltype(mV)>(
                    FWD(mV)[mD.getIdx()]
                        .template as<std::remove_reference_t<decltype(mE)>>());
            },
            mX);
    }
};

// Convert `std::vector`
template <typename TItem>
struct Cnv<std::vector<TItem>> final
{
    using Type = std::vector<TItem>;

    template <typename T>
    inline static void toVal(Val& mV, T&& mX)
    {
        Arr result;
        result.reserve(mX.size());
        for(const auto& v : mX)
            result.emplace_back(moveIfRValue<decltype(mX)>(v));
        mV.setArr(std::move(result));
    }
    template <typename T>
    inline static void fromVal(T&& mV, Type& mX)
    {
        const auto& arr(mV.getArr());
        mX.reserve(arr.size());
        mX.clear();
        for(auto i(0u); i < arr.size(); ++i)
            mX.emplace_back(
                moveIfRValue<decltype(mV)>(arr[i].template as<TItem>()));
    }
};

// Convert map-like objects
template <typename TKey, typename TValue,
    template <typename, typename, typename...> class TMap, typename... TExtra>
struct Cnv<TMap<TKey, TValue, TExtra...>,
    ssvu::Impl::VoidT<typename TMap<TKey, TValue, TExtra...>::key_type>>
    final
{
    using Type = TMap<TKey, TValue, TExtra...>;

    template <typename T>
    inline static void toVal(Val& mVal, T&& mX)
    {
        mVal = Arr{};
        for(auto& p : mX)
            mVal.getArr().emplace_back(moveIfRValue<decltype(mX)>(p));
    }
    template <typename T>
    inline static void fromVal(T&& mVal, Type& mX)
    {
        for(auto p : mVal.template forArrAs<std::pair<TKey, TValue>>())
        {
            mX[p.first] = p.second;
        }
    }
};

// Convert C-style array
template <typename TItem, std::size_t TS>
struct Cnv<TItem[TS]>
{
    using Type = TItem[TS];

    template <typename T>
    inline static void toVal(Val& mV, T&& mX)
    {
        Arr result;
        result.reserve(TS);
        for(auto i(0u); i < TS; ++i)
            result.emplace_back(moveIfRValue<decltype(mX)>(mX[i]));
        mV.setArr(std::move(result));
    }
    template <typename T>
    inline static void fromVal(T&& mV, Type& mX)
    {
        assert(mV.getArr().size() >= TS);
        for(auto i(0u); i < TS; ++i)
            mX[i] = moveIfRValue<decltype(mV)>(mV[i].template as<TItem>());
    }
};

// Convert `std::bitset`
template <std::size_t TS>
struct Cnv<std::bitset<TS>>
{
    using Type = std::bitset<TS>;

    template <typename T>
    inline static void toVal(Val& mV, T&& mX)
    {
        mV = mX.to_string();
    }
    template <typename T>
    inline static void fromVal(T&& mV, Type& mX)
    {
        mX = Type{mV.template as<Str>()};
    }
};

// Implementation for simple macro-based user-defined converters
template <typename T>
struct CnvImplSimple
{
    inline static void toVal(Val& mV, const T& mX)
    {
        Cnv<T>::template impl<decltype(mV), decltype(mX)>(mV, mX);
    }
    inline static void toVal(Val& mV, T&& mX)
    {
        Cnv<T>::template impl<decltype(mV), decltype(mX)>(mV, mX);
    }
    inline static void fromVal(const Val& mV, T& mX)
    {
        Cnv<T>::template impl<decltype(mV), decltype(mX)>(mV, mX);
    }
    inline static void fromVal(Val&& mV, T& mX)
    {
        Cnv<T>::template impl<decltype(mV), decltype(mX)>(mV, mX);
    }
};
} // namespace Impl
} // namespace Json
} // namespace ssvu

#endif
