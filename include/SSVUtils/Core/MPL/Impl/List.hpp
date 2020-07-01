// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_LIST
#define SSVU_CORE_MPL_IMPL_LIST

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/Negate.hpp"
#include "SSVUtils/Core/MPL/Impl/MinMax.hpp"
#include "SSVUtils/Core/MPL/Impl/VAHead.hpp"
#include "SSVUtils/Core/MPL/Impl/VATail.hpp"
#include "SSVUtils/Core/MPL/Impl/VACount.hpp"
#include "SSVUtils/Core/MPL/Impl/SubList.hpp"
#include "SSVUtils/Core/MPL/Impl/Contains.hpp"
#include "SSVUtils/Core/MPL/Impl/Append.hpp"
#include "SSVUtils/Core/MPL/Impl/Unique.hpp"
#include "SSVUtils/Core/MPL/Impl/IdxOf.hpp"
#include "SSVUtils/Core/MPL/Impl/Reverse.hpp"

namespace ssvu
{
namespace MPL
{
// Empty list implementation
template <>
struct List<>
{
    using Type = List<>;
    using AsTpl = Tpl<>;
    using Clear = List<>;
    using Unique = List<>;
    using Reverse = List<>;
    template <template <typename...> class T>
    using Rename = T<>;

    static constexpr std::size_t size{0};
    static constexpr bool empty{true};
    static constexpr bool unique{true};

    static constexpr std::size_t typeMaxSize{0};
    static constexpr std::size_t typeMaxAlign{0};

    template <typename T, std::size_t>
    using Insert = List<T>;
    // `Remove` intentionally not implemented
    template <typename T>
    using PushBack = List<T>;
    template <typename T>
    using PushFront = List<T>;

    // `At` intentionally not implemented
    // `Head` intentionally not implemented
    // `Tail` intentionally not implemented

    // `PopBack` intentionally not implemented
    // `PopFront` intentionally not implemented

    template <template <typename> class>
    using Apply = List<>;
    template <template <typename> class>
    using Filter = List<>;
    template <template <typename> class>
    using FilterIdx = List<>;

    template <typename TL>
    using Append = TL;
    template <typename TL>
    using Prepend = TL;

    template <template <typename> class>
    inline static constexpr bool all() noexcept
    {
        return false;
    }
    template <template <typename> class>
    inline static constexpr bool any() noexcept
    {
        return false;
    }
    template <typename TL>
    inline static constexpr bool isEqualTo() noexcept
    {
        return isSame<Type, TL>();
    }

    template <typename>
    using IdxsOf = ListInt<>;
    template <typename>
    inline static constexpr auto getCountOf() noexcept
    {
        return 0;
    }
    template <typename>
    inline static constexpr bool has() noexcept
    {
        return false;
    }
    template <typename, typename>
    using ReplaceAllOf = List<>;
    // `getIdxOf` intentionally not implemented

    template <typename>
    using IdxsOfSeq = ListInt<>;
    template <typename>
    inline static constexpr auto getCountOfSeq() noexcept
    {
        return 0;
    }
    template <typename>
    inline static constexpr auto hasSeq() noexcept
    {
        return false;
    }
    template <typename, typename>
    using ReplaceAllOfSeq = List<>;

    // `Slice` intentionally not implemented
};

// Non-empty list implementation
template <typename... Ts>
struct List
{
    /// @brief Type of the list itself.
    using Type = List<Ts...>;

    /// @brief Returns this list as a tuple.
    using AsTpl = Tpl<Ts...>;

    /// @brief Returns an empty list.
    using Clear = List<>;

    /// @brief Returns a copy of this list, without duplicate types.
    using Unique = Impl::Unique<Ts...>;

    /// @brief Returns a copy of this list, in reverse order.
    using Reverse = Impl::Reverse<Ts...>;

    /// @brief Replaces `List<Ts...>` with `T<Ts...>`.
    template <template <typename...> class T>
    using Rename = T<Ts...>;



    /// @brief Count of types in the list.
    static constexpr std::size_t size{sizeof...(Ts)};

    /// @brief Is the list empty?
    static constexpr bool empty{size == 0};

    /// @brief True if the list has no duplicate types.
    static constexpr bool unique{isSame<Type, Unique>()};



    /// @brief Size (using `sizeof`) of the biggest type in the list.
    static constexpr std::size_t typeMaxSize{
        getMax<std::size_t>(sizeof(Ts)...)};

    /// @brief Biggest alignment (using `alignof`) of the types in the
    /// list.
    static constexpr std::size_t typeMaxAlign{
        getMax<std::size_t>(alignof(Ts)...)};



    /// @brief Returns a copy of this list with the type `T` inserted in
    /// the index `TN`.
    template <typename T, std::size_t TN>
    using Insert =
        typename Impl::Insert<MkIdxSeq<sizeof...(Ts) + 1>, T, TN, Ts...>::Type;

    /// @brief Returns a copy of this list with the type in the index
    /// `TN` removed.
    template <std::size_t TN>
    using Remove = typename Impl::Remove<TN, Ts...>::Type;

    /// @brief Returns a copy of this list, with `T` added at the back.
    template <typename T>
    using PushBack = List<Ts..., T>;

    /// @brief Returns a copy of this list, with `T` added at the front.
    template <typename T>
    using PushFront = List<T, Ts...>;



    /// @brief Returns the type at index `TS`.
    template <int TI>
    using At = std::tuple_element_t<TI, AsTpl>;

    /// @brief First type in the list. `Null` if empty.
    using Head = At<0>;

    /// @brief Last type in the list. `Null` if empty.
    using Tail = At<int(size) - 1>;



    /// @brief Returns a copy of this list, without the type at the
    /// back.
    using PopBack = Remove<size - 1>;

    /// @brief Returns a copy of this list, without the type at the
    /// front.
    using PopFront = Remove<0>;



    /// @brief Returns a new list, where every type is the result of
    /// applying `TApply`.
    template <template <typename> class TApply>
    using Apply = List<TApply<Ts>...>;

    /// @brief Returns a copy of this list, discarding the types that do
    /// not satisfy `TFilter`.
    template <template <typename> class TFilter>
    using Filter = Impl::Filter<TFilter, Ts...>;

    /// @brief Returns a copy of this list, discarding the types at the
    /// indices that do not satisfy `TFilter`.
    template <typename TFilter>
    using FilterIdx = Impl::FilterIdx<TFilter, Ts...>;



    /// @brief Returns a copy of this list, with all types from the list
    /// `TL` added at the back.
    template <typename TL>
    using Append = typename Impl::Append<Type, TL>::Type;

    /// @brief Returns a copy of this list, with all types from the list
    /// `TL` added at the front.
    template <typename TL>
    using Prepend = typename Impl::Append<TL, Type>::Type;



    /// @brief Returns true if all types in the list match `TFilter`.
    template <template <typename> class TFilter>
    inline static constexpr bool all()
    {
        return Filter<TFilter>::size == size;
    }

    /// @brief Returns true if at least one of the types matches
    /// `TFilter`.
    template <template <typename> class TFilter>
    inline static constexpr bool any()
    {
        return Filter<TFilter>::size > 0;
    }

    /// @brief Returns true if the list `TL` is equal to the current
    /// list.
    template <typename TL>
    inline static constexpr bool isEqualTo() noexcept
    {
        return isSame<Type, TL>();
    }



    /// @brief Returns a `ListInt` of all the indices where the type `T`
    /// occurs in this list.
    template <typename T>
    using IdxsOf = Impl::IdxsOfSeq<Type, List<T>>;

    /// @brief Returns the number of occurrences of `T` in the list.
    template <typename T>
    inline static constexpr std::size_t getCountOf() noexcept
    {
        return Impl::getCountOf<T, Ts...>();
    }

    /// @brief Returns true if at least one occurrence of `T` is present
    /// in the list.
    template <typename T>
    inline static constexpr bool has() noexcept
    {
        return Impl::Contains<T, Ts...>();
    }

    /// @brief Returns a new list with every occurrence of the type `T`
    /// replaced with the contents of the list `TN`.
    template <typename T, typename TN>
    using ReplaceAllOf = Impl::ReplaceAllOfSeq<Type, List<T>, TN>;

    /// @brief Returns the index of the first occurence of `T` in the
    /// type list. Asserts that `T` is in the list.
    template <typename T>
    inline static constexpr std::size_t getIdxOf()
    {
        SSVU_ASSERT_STATIC_NM(has<T>());
        return Impl::IdxOf<T, Ts...>{};
    }



    /// @brief Returns a `ListInt` of all the indices where the sublist
    /// `TL` occurs in this list.
    template <typename TL>
    using IdxsOfSeq = Impl::IdxsOfSeq<Type, TL>;

    /// @brief Returns number of times the sublist `TL` occurs in this
    /// list.
    template <typename TL>
    inline static constexpr auto getCountOfSeq() noexcept
    {
        return IdxsOfSeq<TL>::size;
    }

    /// @brief Returns true if the sublist `TL` occurs in this list.
    template <typename TL>
    inline static constexpr auto hasSeq() noexcept
    {
        return getCountOfSeq<TL>() > 0;
    }

    /// @brief Returns a new list with every occurrence of the sublist
    /// `TL` replaced with the contents of the list `TN`.
    template <typename TL, typename TN>
    using ReplaceAllOfSeq = Impl::ReplaceAllOfSeq<Type, TL, TN>;



    /// @brief Returns the sublist of this list [TS1, TS2).
    template <int TS1, int TS2>
    using Slice = Impl::Slice<Type, TS1, TS2>;
};
} // namespace MPL
} // namespace ssvu

// TODO: sort
// TODO: check stackoverflow question:
// http://stackoverflow.com/questions/28767814/
// GCC5: variadic templates

#endif
