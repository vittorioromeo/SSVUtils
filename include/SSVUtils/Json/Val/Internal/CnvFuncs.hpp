// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CNVFUNCS
#define SSVU_JSON_VAL_INTERNAL_CNVFUNCS

#include "SSVUtils/Json/Val/Val.hpp"
#include "SSVUtils/Json/Val/Internal/Cnv.hpp"

namespace ssvu
{
    namespace Json
    {
        template <typename T, typename TFwd>
        inline void extr(TFwd&& mV, T& mX) noexcept(
            noexcept(Impl::Cnv<RmAll<T>>::fromVal(FWD(mV), mX)))
        {
            Impl::Cnv<RmAll<T>>::fromVal(FWD(mV), mX);
        }
        template <typename T>
        inline void arch(Val& mV, T&& mX) noexcept(noexcept(mV = FWD(mX)))
        {
            mV = FWD(mX);
        }

        namespace Impl
        {
            struct CnvFuncHelper
            {
                template <Idx TI, typename TArg, typename T>
                inline static void hExtrArr(T&& mV, TArg& mArg)
                {
                    SSVU_ASSERT(mV.template is<Arr>() &&
                                mV.template as<Arr>().size() > TI &&
                                mV[TI].template isNoNum<TArg>());
                    extr<TArg>(moveIfRValue<decltype(mV)>(mV[TI]), mArg);
                }
                template <Idx TI, typename TArg, typename... TArgs, typename T>
                inline static void hExtrArr(T&& mV, TArg& mArg, TArgs&... mArgs)
                {
                    hExtrArr<TI>(FWD(mV), mArg);
                    hExtrArr<TI + 1>(FWD(mV), mArgs...);
                }

                template <Idx TI, typename TArg>
                inline static void hArchArr(Val& mV, TArg&& mArg)
                {
                    SSVU_ASSERT(mV.template is<Arr>());
                    mV.emplace(FWD(mArg));
                }
                template <Idx TI, typename TArg, typename... TArgs>
                inline static void hArchArr(
                    Val& mV, TArg&& mArg, TArgs&&... mArgs)
                {
                    hArchArr<TI>(mV, FWD(mArg));
                    hArchArr<TI + 1>(mV, FWD(mArgs)...);
                }

                template <typename TArg, typename T>
                inline static void hExtrObj(T&& mV, const Key& mKey, TArg& mArg)
                {
                    SSVU_ASSERT(mV.template is<Obj>() && mV.has(mKey));
                    extr<TArg>(moveIfRValue<decltype(mV)>(mV[mKey]), mArg);
                }
                template <typename TArg, typename... TArgs, typename T>
                inline static void hExtrObj(
                    T&& mV, const Key& mKey, TArg& mArg, TArgs&... mArgs)
                {
                    hExtrObj(FWD(mV), mKey, mArg);
                    hExtrObj(FWD(mV), mArgs...);
                }

                template <typename TKey, typename TArg>
                inline static void hArchObj(Val& mV, TKey&& mKey, TArg&& mArg)
                {
                    SSVU_ASSERT(mV.template is<Obj>());
                    arch(mV[FWD(mKey)], FWD(mArg));
                }
                template <typename TKey, typename TArg, typename... TArgs>
                inline static void hArchObj(
                    Val& mV, TKey&& mKey, TArg&& mArg, TArgs&&... mArgs)
                {
                    hArchObj(mV, FWD(mKey), FWD(mArg));
                    hArchObj(mV, FWD(mArgs)...);
                }
            };
        }

        template <typename T, typename TFwd>
        inline T getExtr(TFwd&& mV)
        {
            T result;
            extr(FWD(mV), result);
            return result;
        }
        template <typename T>
        inline Val getArch(T&& mX)
        {
            Val result;
            arch(result, FWD(mX));
            return result;
        }

        template <typename... TArgs, typename T>
        inline void extrArr(T&& mV, TArgs&... mArgs)
        {
            Impl::CnvFuncHelper::hExtrArr<0>(FWD(mV), mArgs...);
        }
        template <typename... TArgs>
        inline void archArr(Val& mV, TArgs&&... mArgs)
        {
            mV = Impl::Arr{};
            Impl::CnvFuncHelper::hArchArr<0>(mV, FWD(mArgs)...);
        }
        template <typename... TArgs>
        inline Val getArchArr(TArgs&&... mArgs)
        {
            Val result;
            archArr(result, FWD(mArgs)...);
            return result;
        }

        template <typename... TArgs, typename T>
        inline void extrObj(T&& mV, TArgs&... mArgs)
        {
            Impl::CnvFuncHelper::hExtrObj(FWD(mV), mArgs...);
        }
        template <typename... TArgs>
        inline void archObj(Val& mV, TArgs&&... mArgs)
        {
            mV = Impl::Obj{};
            Impl::CnvFuncHelper::hArchObj(mV, FWD(mArgs)...);
        }
        template <typename... TArgs>
        inline Val getArchObj(TArgs&&... mArgs)
        {
            Val result;
            archObj(result, FWD(mArgs)...);
            return result;
        }

        template <typename T>
        inline void cnv(const Val& mV, T& mX) noexcept(noexcept(extr(mV, mX)))
        {
            extr(mV, mX);
        }
        template <typename T>
        inline void cnv(Val&& mV, T& mX) noexcept(noexcept(extr(mv(mV), mX)))
        {
            extr(mv(mV), mX);
        }
        template <typename T>
        inline void cnv(Val& mV, T&& mX) noexcept(noexcept(arch(mV, FWD(mX))))
        {
            arch(mV, FWD(mX));
        }

        template <typename... TArgs>
        inline void cnvArr(const Val& mV, TArgs&... mArgs)
        {
            extrArr(mV, mArgs...);
        }
        template <typename... TArgs>
        inline void cnvArr(Val&& mV, TArgs&... mArgs)
        {
            extrArr(mv(mV), mArgs...);
        }
        template <typename... TArgs>
        inline void cnvArr(Val& mV, TArgs&&... mArgs)
        {
            archArr(mV, FWD(mArgs)...);
        }

        template <typename... TArgs>
        inline void cnvObj(const Val& mV, TArgs&... mArgs)
        {
            extrObj(mV, mArgs...);
        }
        template <typename... TArgs>
        inline void cnvObj(Val&& mV, TArgs&... mArgs)
        {
            extrObj(mv(mV), mArgs...);
        }
        template <typename... TArgs>
        inline void cnvObj(Val& mV, TArgs&&... mArgs)
        {
            archObj(mV, FWD(mArgs)...);
        }
    }
}

#endif
