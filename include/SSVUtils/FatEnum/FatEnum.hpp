// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FATENUM
#define SSVU_FATENUM

#include "SSVUtils/Bimap/Bimap.hpp"

#include <vrm/pp.hpp>

#include <string>
#include <cassert>

#define SSVU_FATENUM_IMPL_MK_ELEM_VALS(mIdx, mData, mArg) \
    VRM_PP_TPL_ELEM(mArg, 0) = VRM_PP_TPL_ELEM(mArg, 1) VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ELEM_DEFS(mIdx, mData, mArg) \
    mArg VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ELEM_DISPATCH(mDispatch) \
    VRM_PP_CAT(SSVU_FATENUM_IMPL_MK_ELEM_, mDispatch)

#define SSVU_FATENUM_IMPL_MK_BIMAP_ENTRY_VALS(mIdx, mData, mArg) \
    {VRM_PP_TOSTR(VRM_PP_TPL_ELEM(mArg, 0)),                     \
        mData ::VRM_PP_TPL_ELEM(mArg, 0)} VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_BIMAP_ENTRY_DEFS(mIdx, mData, mArg) \
    {VRM_PP_TOSTR(mArg), mData::mArg} VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_BIMAP_ENTRY(mDispatch) \
    VRM_PP_CAT(SSVU_FATENUM_IMPL_MK_BIMAP_ENTRY_, mDispatch)

#define SSVU_FATENUM_IMPL_MK_ARRAY_ENTRY_VALS(mIdx, mData, mArg) \
    mData::VRM_PP_TPL_ELEM(mArg, 0) VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ARRAY_ENTRY_DEFS(mIdx, mData, mArg) \
    mData::mArg VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ARRAY_ENTRY(mDispatch) \
    VRM_PP_CAT(SSVU_FATENUM_IMPL_MK_ARRAY_ENTRY_, mDispatch)

#define SSVU_FATENUM_IMPL_MK_ARRAY_EN_ENTRY_VALS(mIdx, mData, mArg) \
    VRM_PP_TOSTR(VRM_PP_TPL_ELEM(mArg, 0)) VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ARRAY_EN_ENTRY_DEFS(mIdx, mData, mArg) \
    VRM_PP_TOSTR(mArg) VRM_PP_COMMA_IF(mIdx)

#define SSVU_FATENUM_IMPL_MK_ARRAY_EN_ENTRY(mDispatch) \
    VRM_PP_CAT(SSVU_FATENUM_IMPL_MK_ARRAY_EN_ENTRY_, mDispatch)

#define SSVU_FATENUM_IMPL_MK_GETASSTRING(mMgr, mEnum, mX)              \
    template <>                                                        \
    inline const auto& mMgr<mEnum>::getAsStrImpl<mEnum::mX>() noexcept \
    {                                                                  \
        static std::string s{VRM_PP_TOSTR(mX)};                        \
        return s;                                                      \
    }
#define SSVU_FATENUM_IMPL_MK_GETASSTRING_VALS(mIdx, mData, mArg) \
    SSVU_FATENUM_IMPL_MK_GETASSTRING(VRM_PP_TPL_ELEM(mData, 0),  \
        VRM_PP_TPL_ELEM(mData, 1), VRM_PP_TPL_ELEM(mArg, 0))

#define SSVU_FATENUM_IMPL_MK_GETASSTRING_DEFS(mIdx, mData, mArg) \
    SSVU_FATENUM_IMPL_MK_GETASSTRING(                            \
        VRM_PP_TPL_ELEM(mData, 0), VRM_PP_TPL_ELEM(mData, 1), mArg)

#define SSVU_FATENUM_IMPL_MK_GETASSTRING_DISPATCH(mDispatch) \
    VRM_PP_CAT(SSVU_FATENUM_IMPL_MK_GETASSTRING_, mDispatch)

#define SSVU_FATENUM_MGR(mMgr) \
    template <typename>        \
    class mMgr                 \
    {                          \
    }

namespace ssvu
{
namespace Impl
{
template <std::size_t, typename>
struct FatEnumMgrImpl;

template <std::size_t TS, template <typename> class T, typename TEnum>
struct FatEnumMgrImpl<TS, T<TEnum>>
{
    inline static std::size_t getSize() noexcept
    {
        return TS;
    }
    template <TEnum TVal>
    inline static const auto& getAsStr() noexcept
    {
        return T<TEnum>::template getAsStrImpl<TVal>();
    }
    inline static auto& getAsStr(TEnum mValue) noexcept
    {
        assert(T<TEnum>::getBimap().has(mValue));
        return T<TEnum>::getBimap().at(mValue);
    }
    inline static TEnum getFromStr(const std::string& mValue) noexcept
    {
        assert(T<TEnum>::getBimap().has(mValue));
        return T<TEnum>::getBimap().at(mValue);
    }
};
} // namespace Impl
} // namespace ssvu

#define SSVU_FATENUM_IMPL(mMgr, mName, mUnderlying, mDispatch, ...)           \
    enum class mName : mUnderlying                                            \
    {                                                                         \
        VRM_PP_FOREACH_REVERSE(SSVU_FATENUM_IMPL_MK_ELEM_DISPATCH(mDispatch), \
            VRM_PP_EMPTY(), __VA_ARGS__)                                      \
    };                                                                        \
    template <>                                                               \
    struct mMgr<mName> final                                                  \
        : public ssvu::Impl::FatEnumMgrImpl<VRM_PP_ARGCOUNT(__VA_ARGS__),     \
              mMgr<mName>>                                                    \
    {                                                                         \
        template <mName TVal>                                                 \
        inline static const auto& getAsStrImpl() noexcept;                    \
        inline static const auto& getBimap() noexcept                         \
        {                                                                     \
            static ssvu::Bimap<std::string, mName> result{                    \
                VRM_PP_FOREACH_REVERSE(                                       \
                    SSVU_FATENUM_IMPL_MK_BIMAP_ENTRY(mDispatch), mName,       \
                    __VA_ARGS__)};                                            \
            return result;                                                    \
        }                                                                     \
        inline static const auto& getValues() noexcept                        \
        {                                                                     \
            static auto result(::ssvu::mkArray(VRM_PP_FOREACH_REVERSE(        \
                SSVU_FATENUM_IMPL_MK_ARRAY_ENTRY(mDispatch), mName,           \
                __VA_ARGS__)));                                               \
            return result;                                                    \
        }                                                                     \
        inline static const auto& getElementNames() noexcept                  \
        {                                                                     \
            static auto result(::ssvu::mkArray(VRM_PP_FOREACH_REVERSE(        \
                SSVU_FATENUM_IMPL_MK_ARRAY_EN_ENTRY(mDispatch), mName,        \
                __VA_ARGS__)));                                               \
            return result;                                                    \
        }                                                                     \
    };                                                                        \
    VRM_PP_FOREACH_REVERSE(                                                   \
        SSVU_FATENUM_IMPL_MK_GETASSTRING_DISPATCH(mDispatch),                 \
        VRM_PP_TPL_MAKE(mMgr, mName), __VA_ARGS__)

/// @macro Defines a fat enum using tuples of name and values.
/// @code
/// SSVU_FATENUM_MGR(EnumManager);
/// SSVU_FATENUM_VALS(EnumManager, EnumName, int, (EName0, 0), (EName1, 5),
/// (EName2, 2))
/// assert(int(EnumName::EName0) == 0);
/// assert(int(EnumName::EName1) == 5);
/// assert(int(EnumName::EName2) == 2);
/// assert(EnumManager<EnumName>::getSize() == 3);
/// assert(EnumManager<EnumName>::getAsStr(EnumName::EName0) == "EName0");
/// assert(EnumManager<EnumName>::getFromStr("EName0") ==
/// EnumName::EName0);
/// @endcode
/// @details Must end without semicolon.
#define SSVU_FATENUM_VALS(mMgr, mName, mUnderlying, ...) \
    SSVU_FATENUM_IMPL(mMgr, mName, mUnderlying, VALS, __VA_ARGS__)

/// @macro Defines a fat enum using default values.
/// @code
/// SSVU_FATENUM_MGR(EnumManager);
/// SSVU_FATENUM_VALS(EnumManager, EnumName, int, EName0, EName1, EName2)
/// assert(int(EnumName::EName0) == 0);
/// assert(int(EnumName::EName1) == 1);
/// assert(int(EnumName::EName2) == 2);
/// assert(EnumManager<EnumName>::getSize() == 3);
/// assert(EnumManager<EnumName>::getAsStr(EnumName::EName0) == "EName0");
/// assert(EnumManager<EnumName>::getFromStr("EName0") ==
/// EnumName::EName0);
/// @endcode
/// @details Must end without semicolon.
// TODO: since integer values cannot be specified for this enum, use an array
// for string-value lookup?
#define SSVU_FATENUM_DEFS(mMgr, mName, mUnderlying, ...) \
    SSVU_FATENUM_IMPL(mMgr, mName, mUnderlying, DEFS, __VA_ARGS__)

#endif
