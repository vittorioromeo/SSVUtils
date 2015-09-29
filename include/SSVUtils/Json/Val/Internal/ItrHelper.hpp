// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_ITRHELPER
#define SSVU_JSON_VAL_INTERNAL_ITRHELPER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Union/Union.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/Json/Common/Common.hpp"
#include "SSVUtils/Json/Num/Num.hpp"
#include "SSVUtils/Json/Val/Internal/Fwd.hpp"

namespace ssvu
{
    namespace Json
    {
        namespace Impl
        {
            /// @brief Helper class for iteration on `Val` objects.
            struct ItrHelper
            {
                // Key/value pair class and helper functions
                template <typename TK, typename TV>
                struct KVPair
                {
                    TK key;
                    TV value;
                };
                template <typename TK, typename TV>
                inline static constexpr auto makeKVPair(TK&& mK,
                                                        TV&& mV) noexcept
                {
                    return KVPair<TK, TV>{FWD(mK), FWD(mV)};
                }

                // Iterator adaptor implementation that returns casted KVPair
                // objects from `Obj` iteration
                template <typename T>
                struct ImplAsObj
                {
                    template <typename TItr>
                    inline static constexpr decltype(auto)
                    get(TItr mItr) noexcept
                    {
                        return makeKVPair(mItr->first,
                                          mItr->second.template as<T>());
                    }
                };

                // Iterator adaptor implementation that returns casted objects
                // from `Arr` iteration
                template <typename T>
                struct ImplAsArr
                {
                    template <typename TItr>
                    inline static constexpr decltype(auto)
                    get(TItr mItr) noexcept
                    {
                        return mItr->template as<T>();
                    }
                };

                // Range creation helper functions
                template <template <typename> class TImpl, typename T,
                          typename TItr>
                using ItrAs = ssvu::Impl::AdaptorFromItr<TItr, TImpl<T>>;
                template <template <typename> class TImpl, typename T,
                          typename TItr>
                inline static constexpr auto makeItrAs(TItr mItr) noexcept
                {
                    return ItrAs<TImpl, T, TItr>{mItr};
                }
                template <template <typename> class TImpl, typename T,
                          typename TItr>
                inline static constexpr auto makeItrAsRange(TItr mBegin,
                                                            TItr mEnd) noexcept
                {
                    return makeRange(makeItrAs<TImpl, T>(mBegin),
                                     makeItrAs<TImpl, T>(mEnd));
                }

                // Valid range creation helper functions
                template <typename T, typename TItr>
                inline static constexpr auto makeItrObjRange(TItr mBegin,
                                                             TItr mEnd) noexcept
                {
                    return makeItrAsRange<ImplAsObj, T>(mBegin, mEnd);
                }
                template <typename T, typename TItr>
                inline static constexpr auto makeItrArrRange(TItr mBegin,
                                                             TItr mEnd) noexcept
                {
                    return makeItrAsRange<ImplAsArr, T>(mBegin, mEnd);
                }

                // Empty range creation helper functions
                template <typename T = Val>
                inline static auto& getEmptyObj() noexcept
                {
                    static ObjImpl<T> result;
                    return result;
                }
                template <typename T = Val>
                inline static auto& getEmptyArr() noexcept
                {
                    static ArrImpl<T> result;
                    return result;
                }
            };
        }
    }
}

#endif
