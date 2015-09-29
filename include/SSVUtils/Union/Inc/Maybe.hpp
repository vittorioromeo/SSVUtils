// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNION_MAYBE
#define SSVU_IMPL_UNION_MAYBE

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Union/Inc/UnionBase.hpp"
#include "SSVUtils/Union/Inc/Union.hpp"
#include "SSVUtils/Union/Inc/UnionPOD.hpp"

namespace ssvu
{
    namespace Impl
    {
        template <typename T, template <typename...> class TUV>
        class MaybeImpl
        {
        protected:
            TUV<T> uv;

        public:
            template <typename... TArgs>
            inline void init(TArgs&&... mArgs) noexcept(
                noexcept(uv.template init<T>(FWD(mArgs)...)))
            {
                uv.template init<T>(FWD(mArgs)...);
            }

            inline T& get() & noexcept { return uv.template get<T>(); }
            inline const T& get() const& noexcept
            {
                return uv.template get<T>();
            }
            inline const T get() && noexcept
            {
                return mv(uv.template get<T>());
            }

            inline T& operator*() noexcept { return get(); }
            inline const T& operator*() const noexcept { return get(); }

            inline T* operator->() noexcept { return &get(); }
            inline const T* operator->() const noexcept { return &get(); }
        };
    }

    template <typename T>
    struct Maybe : public Impl::MaybeImpl<T, Union>
    {
        inline void deinit() noexcept(isNothrowDtor<T>())
        {
            this->uv.template deinit<T>();
        }
    };

    template <typename T>
    using MaybePOD = Impl::MaybeImpl<T, UnionPOD>;
}

#endif
