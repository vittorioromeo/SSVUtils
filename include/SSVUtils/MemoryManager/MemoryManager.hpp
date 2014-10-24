// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"

// Forward declarations
namespace ssvu
{
	namespace Internal
	{
		template<typename, template<typename> class, typename, typename> class BaseRecycler;
		template<typename, template<typename> class> class MonoRecyclerImpl;
		template<typename, template<typename> class, typename> class PolyRecyclerImpl;

		template<typename, typename> class BaseManager;
	}
}

// Implementations
#include "SSVUtils/MemoryManager/Internal/LayoutImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/StorageImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/RecyclerImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/ManagerImpl.hpp"

// User interface
namespace ssvu
{
	/// @brief Memory recycler for a single object type. Doesn't store additional information in the object.
	template<typename TBase> using MonoRecycler = Internal::MonoRecyclerImpl
	<
		TBase,
		Internal::LayoutImpl::LHelperNoBool
	>;

	/// @brief Memory recycler for multiple object types. Doesn't store additional information in the objects.
	template<typename TBase> using PolyRecycler = Internal::PolyRecyclerImpl
	<
		TBase,
		Internal::LayoutImpl::LHelperNoBool,
		Internal::PolyStorage<TBase, Internal::LayoutImpl::LHelperNoBool>
	>;

	/// @brief Memory recycler for multiple object types. Doesn't store additional information in the objects. Supports a fixed amount of object sizes.
	template<typename TBase, SizeT TMaxChunks> using PolyFixedRecycler = Internal::PolyRecyclerImpl
	<
		TBase,
		Internal::LayoutImpl::LHelperNoBool,
		Internal::PolyFixedStorage<TBase, Internal::LayoutImpl::LHelperNoBool, TMaxChunks>
	>;

	/// @brief Memory recycler manager for a single object type. Stores an additional bool in every object.
	template<typename TBase> using MonoManager = Internal::BaseManager
	<
		TBase,
		Internal::MonoRecyclerImpl<TBase, Internal::LayoutImpl::LHelperBool>
	>;

	/// @brief Memory recycler manager for a multiple object types. Stores an additional bool in every object.
	template<typename TBase> using PolyManager = Internal::BaseManager<TBase, Internal::PolyRecyclerImpl
	<
		TBase,
		Internal::LayoutImpl::LHelperBool,
		Internal::PolyStorage<TBase, Internal::LayoutImpl::LHelperBool>>
	>;

	/// @brief Memory recycler manager for a multiple object types. Stores an additional bool in every object. Supports a fixed amount of object sizes.
	template<typename TBase, SizeT TMaxChunks> using PolyFixedManager = Internal::BaseManager<TBase, Internal::PolyRecyclerImpl
	<
		TBase,
		Internal::LayoutImpl::LHelperBool,
		Internal::PolyFixedStorage<TBase, Internal::LayoutImpl::LHelperBool, TMaxChunks>>
	>;
}

#endif
