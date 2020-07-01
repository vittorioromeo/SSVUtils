// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"
#include "SSVUtils/MemoryManager/Internal/Fwd.hpp"

// Implementations
#include "SSVUtils/MemoryManager/Internal/LayoutImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/StorageImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/RecyclerImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/ManagerImpl.hpp"

// User interface
namespace ssvu
{
/// @brief Memory recycler for a single object type. Doesn't store
/// additional information in the object.
template <typename TBase>
using MonoRecycler =
    Impl::MonoRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool>;

/// @brief Memory recycler for multiple object types. Doesn't store
/// additional information in the objects.
template <typename TBase>
using PolyRecycler =
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool,
        Impl::PolyStorage<TBase, Impl::LayoutImpl::LHelperNoBool>>;

/// @brief Memory recycler for multiple object types. Doesn't store
/// additional information in the objects. Supports a fixed amount of object
/// sizes.
template <typename TBase, typename... Ts>
using PolyFixedRecycler =
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool,
        Impl::PolyFixedStorage<TBase, Impl::LayoutImpl::LHelperNoBool,
            MPL::List<Ts...>>>;

/// @brief Memory recycler manager for a single object type. Stores an
/// additional bool in every object.
template <typename TBase>
using MonoManager = Impl::BaseManager<TBase,
    Impl::MonoRecyclerImpl<TBase, Impl::LayoutImpl::LHelperBool>>;

/// @brief Memory recycler manager for multiple object types. Stores an
/// additional bool in every object.
template <typename TBase>
using PolyManager = Impl::BaseManager<TBase,
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperBool,
        Impl::PolyStorage<TBase, Impl::LayoutImpl::LHelperBool>>>;

/// @brief Memory recycler manager for multiple object types. Stores an
/// additional bool in every object. Supports a fixed amount of object
/// sizes.
template <typename TBase, typename... Ts>
using PolyFixedManager = Impl::BaseManager<TBase,
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperBool,
        Impl::PolyFixedStorage<TBase, Impl::LayoutImpl::LHelperBool,
            MPL::List<Ts...>>>>;

/// @brief `std::vector` + recycler wrapper class for a single object type.
/// Doesn't store additional data in the object.
template <typename TBase>
using MonoRecVector = Impl::BaseRecVector<TBase,
    Impl::MonoRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool>>;

/// @brief `std::vector` + recycler wrapper class multiple object types.
/// Doesn't store additional data in the objects.
template <typename TBase>
using PolyRecVector = Impl::BaseRecVector<TBase,
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool,
        Impl::PolyStorage<TBase, Impl::LayoutImpl::LHelperNoBool>>>;

/// @brief `std::vector` + recycler wrapper class multiple object types.
/// Doesn't store additional data in the objects. Supports a fixed amount of
/// object sizes.
template <typename TBase, typename... Ts>
using PolyFixedRecVector = Impl::BaseRecVector<TBase,
    Impl::PolyRecyclerImpl<TBase, Impl::LayoutImpl::LHelperNoBool,
        Impl::PolyFixedStorage<TBase, Impl::LayoutImpl::LHelperNoBool,
            MPL::List<Ts...>>>>;
} // namespace ssvu

#endif

// TODO: fixed storage type that takes a variadic list of std::size_t sizes as
// parameters and uses the smallest chunk available, without TBase
