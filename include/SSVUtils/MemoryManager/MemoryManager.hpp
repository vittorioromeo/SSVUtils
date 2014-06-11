// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<typename, template<typename> class, template<typename, template<typename> class> class, template<typename, template<typename> class> class> class BaseRecycler;
		template<typename, template<typename> class> class MonoRecyclerImpl;
		template<typename, template<typename> class> class PolyRecyclerImpl;

		template<typename, template<typename, template<typename> class> class> class BaseManager;
	}
}

#include "SSVUtils/MemoryManager/Internal/LayoutImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/StorageImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/RecyclerImpl.hpp"
#include "SSVUtils/MemoryManager/Internal/ManagerImpl.hpp"

namespace ssvu
{
	template<typename TBase> using MonoRecycler = Internal::MonoRecyclerImpl<TBase, Internal::LayoutImpl::LHelperNoBool>;
	template<typename TBase> using PolyRecycler = Internal::PolyRecyclerImpl<TBase, Internal::LayoutImpl::LHelperNoBool>;

	template<typename TBase> using MonoManager = Internal::BaseManager<TBase, Internal::MonoRecyclerImpl>;
	template<typename TBase> using PolyManager = Internal::BaseManager<TBase, Internal::PolyRecyclerImpl>;
}

#endif
