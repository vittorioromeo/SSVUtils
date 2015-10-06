// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_FWD
#define SSVU_MEMORYMANAGER_INTERNAL_FWD

// Forward declarations
namespace ssvu
{
    namespace Impl
    {
        template <typename, template <typename> class, typename, typename>
        class BaseRecycler;
        template <typename, template <typename> class>
        struct MonoRecyclerImpl;
        template <typename, template <typename> class, typename>
        struct PolyRecyclerImpl;

        template <typename, typename>
        class BaseManager;
    }
}

#endif