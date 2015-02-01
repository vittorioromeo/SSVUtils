// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CNVFUNCS
#define SSVU_JSON_VAL_INTERNAL_CNVFUNCS

namespace ssvu
{
	namespace Json
	{
		template<typename T, typename TFwd> inline void extr(TFwd&& mV, T& mX) noexcept(noexcept(Internal::Cnv<RemoveAll<T>>::fromVal(SSVU_FWD(mV), mX))) { Internal::Cnv<RemoveAll<T>>::fromVal(SSVU_FWD(mV), mX); }
		template<typename T> inline void arch(Val& mV, T&& mX) noexcept(noexcept(mV = SSVU_FWD(mX))) { mV = SSVU_FWD(mX); }

		namespace Internal
		{
			struct CnvFuncHelper
			{
				template<Idx TI, typename TArg, typename T> inline static void hExtrArr(T&& mV, TArg& mArg)
				{
					SSVU_ASSERT(mV.template is<Arr>() && mV.template as<Arr>().size() > TI && mV[TI].template isNoNum<TArg>());
					extr<TArg>(moveIfRValue<decltype(mV)>(mV[TI]), mArg);
				}
				template<Idx TI, typename TArg, typename... TArgs, typename T> inline static void hExtrArr(T&& mV, TArg& mArg, TArgs&... mArgs)
				{
					hExtrArr<TI>(SSVU_FWD(mV), mArg);
					hExtrArr<TI + 1>(SSVU_FWD(mV), mArgs...);
				}

				template<Idx TI, typename TArg> inline static void hArchArr(Val& mV, TArg&& mArg)
				{
					SSVU_ASSERT(mV.template is<Arr>());
					mV.emplace(SSVU_FWD(mArg));
				}
				template<Idx TI, typename TArg, typename... TArgs> inline static void hArchArr(Val& mV, TArg&& mArg, TArgs&&... mArgs)
				{
					hArchArr<TI>(mV, SSVU_FWD(mArg)); hArchArr<TI + 1>(mV, SSVU_FWD(mArgs)...);
				}

				template<typename TArg, typename T> inline static void hExtrObj(T&& mV, const Key& mKey, TArg& mArg)
				{
					SSVU_ASSERT(mV.template is<Obj>() && mV.has(mKey));
					extr<TArg>(moveIfRValue<decltype(mV)>(mV[mKey]), mArg);
				}
				template<typename TArg, typename... TArgs, typename T> inline static void hExtrObj(T&& mV, const Key& mKey, TArg& mArg, TArgs&... mArgs)
				{
					hExtrObj(SSVU_FWD(mV), mKey, mArg); hExtrObj(SSVU_FWD(mV), mArgs...);
				}

				template<typename TKey, typename TArg> inline static void hArchObj(Val& mV, TKey&& mKey, TArg&& mArg)
				{
					SSVU_ASSERT(mV.template is<Obj>());
					arch(mV[SSVU_FWD(mKey)], SSVU_FWD(mArg));
				}
				template<typename TKey, typename TArg, typename... TArgs> inline static void hArchObj(Val& mV, TKey&& mKey, TArg&& mArg, TArgs&&... mArgs)
				{
					hArchObj(mV, SSVU_FWD(mKey), SSVU_FWD(mArg)); hArchObj(mV, SSVU_FWD(mArgs)...);
				}
			};
		}

		template<typename T, typename TFwd> inline T getExtr(TFwd&& mV) { T result; extr(SSVU_FWD(mV), result); return result; }
		template<typename T> inline Val getArch(T&& mX) { Val result; arch(result, SSVU_FWD(mX)); return result; }

		template<typename... TArgs, typename T> inline void extrArr(T&& mV, TArgs&... mArgs)	{ Internal::CnvFuncHelper::hExtrArr<0>(SSVU_FWD(mV), mArgs...); }
		template<typename... TArgs> inline void archArr(Val& mV, TArgs&&... mArgs)				{ mV = Arr{}; Internal::CnvFuncHelper::hArchArr<0>(mV, SSVU_FWD(mArgs)...); }
		template<typename... TArgs> inline Val getArchArr(TArgs&&... mArgs)						{ Val result; archArr(result, SSVU_FWD(mArgs)...); return result; }

		template<typename... TArgs, typename T> inline void extrObj(T&& mV, TArgs&... mArgs)	{ Internal::CnvFuncHelper::hExtrObj(SSVU_FWD(mV), mArgs...); }
		template<typename... TArgs> inline void archObj(Val& mV, TArgs&&... mArgs)				{ mV = Obj{}; Internal::CnvFuncHelper::hArchObj(mV, SSVU_FWD(mArgs)...); }
		template<typename... TArgs> inline Val getArchObj(TArgs&&... mArgs)						{ Val result; archObj(result, SSVU_FWD(mArgs)...); return result; }

		template<typename T> inline void cnv(const Val& mV, T& mX) noexcept(noexcept(extr(mV, mX)))			{ extr(mV, mX); }
		template<typename T> inline void cnv(Val&& mV, T& mX) noexcept(noexcept(extr(std::move(mV), mX)))	{ extr(std::move(mV), mX); }
		template<typename T> inline void cnv(Val& mV, T&& mX) noexcept(noexcept(arch(mV, SSVU_FWD(mX))))		{ arch(mV, SSVU_FWD(mX)); }

		template<typename... TArgs> inline void cnvArr(const Val& mV, TArgs&... mArgs)	{ extrArr(mV, mArgs...); }
		template<typename... TArgs> inline void cnvArr(Val&& mV, TArgs&... mArgs)		{ extrArr(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void cnvArr(Val& mV, TArgs&&... mArgs)		{ archArr(mV, SSVU_FWD(mArgs)...); }

		template<typename... TArgs> inline void cnvObj(const Val& mV, TArgs&... mArgs)	{ extrObj(mV, mArgs...); }
		template<typename... TArgs> inline void cnvObj(Val&& mV, TArgs&... mArgs)		{ extrObj(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void cnvObj(Val& mV, TArgs&&... mArgs)		{ archObj(mV, SSVU_FWD(mArgs)...); }
	}
}

#endif
