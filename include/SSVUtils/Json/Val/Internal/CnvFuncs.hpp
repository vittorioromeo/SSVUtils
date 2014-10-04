// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CNVFUNCS
#define SSVU_JSON_VAL_INTERNAL_CNVFUNCS

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<Idx TI, typename TArg, typename T> inline void extrArrHelper(T&& mV, TArg& mArg)
			{
				SSVU_ASSERT(mV.template is<Arr>() && mV.template as<Arr>().size() > TI && mV[TI].template is<TArg>());
				mArg = moveIfRValue<decltype(mV)>(mV[TI].template as<TArg>());
			}
			template<Idx TI, typename TArg, typename... TArgs, typename T> inline void extrArrHelper(T&& mV, TArg& mArg, TArgs&... mArgs)
			{
				extrArrHelper<TI>(fwd<T>(mV), mArg);
				extrArrHelper<TI + 1>(fwd<T>(mV), mArgs...);
			}

			template<Idx TI, typename TArg> inline void archArrHelper(Val& mV, TArg&& mArg) { mV.as<Arr>().emplace_back(fwd<TArg>(mArg)); }
			template<Idx TI, typename TArg, typename... TArgs> inline void archArrHelper(Val& mV, TArg&& mArg, TArgs&&... mArgs)
			{
				archArrHelper<TI>(mV, fwd<TArg>(mArg)); archArrHelper<TI + 1>(mV, fwd<TArgs>(mArgs)...);
			}

			template<typename TArg, typename T> inline void extrObjHelper(T&& mV, const Key& mKey, TArg& mArg) { mArg = moveIfRValue<decltype(mV)>(mV[mKey].template as<TArg>()); }
			template<typename TArg, typename... TArgs, typename T> inline void extrObjHelper(T&& mV, const Key& mKey, TArg& mArg, TArgs&... mArgs)
			{
				extrObjHelper(fwd<T>(mV), mKey, mArg); extrObjHelper(fwd<T>(mV), mArgs...);
			}

			template<typename TKey, typename TArg> inline void archObjHelper(Val& mV, TKey&& mKey, TArg&& mArg) { mV[fwd<TKey>(mKey)] = fwd<TArg>(mArg); }
			template<typename TKey, typename TArg, typename... TArgs> inline void archObjHelper(Val& mV, TKey&& mKey, TArg&& mArg, TArgs&&... mArgs)
			{
				archObjHelper(mV, fwd<TKey>(mKey), fwd<TArg>(mArg)); archObjHelper(mV, fwd<TArgs>(mArgs)...);
			}
		}

		template<typename T, typename TFwd> inline void extr(TFwd&& mV, T& mX) noexcept(noexcept(Internal::Cnv<T>::fromVal(fwd<TFwd>(mV), mX))) { Internal::Cnv<T>::fromVal(fwd<TFwd>(mV), mX); }
		template<typename T> inline void arch(Val& mV, T&& mX) noexcept(noexcept(mV = fwd<T>(mX))) { mV = fwd<T>(mX); }

		template<typename T, typename TFwd> inline T getExtr(TFwd&& mV) { T result; extr(fwd<TFwd>(mV), result); return result; }
		template<typename T> inline Val getArch(T&& mX) { Val result; arch(result, fwd<T>(mX)); return result; }

		template<typename... TArgs, typename T> inline void extrArr(T&& mV, TArgs&... mArgs)	{ Internal::extrArrHelper<0>(fwd<T>(mV), mArgs...); }
		template<typename... TArgs> inline void archArr(Val& mV, TArgs&&... mArgs)				{ mV = Arr{}; Internal::archArrHelper<0>(mV, fwd<TArgs>(mArgs)...); }
		template<typename... TArgs> inline Val getArchArr(TArgs&&... mArgs)						{ Val result; archArr(result, fwd<TArgs>(mArgs)...); return result; }

		template<typename... TArgs, typename T> inline void extrObj(T&& mV, TArgs&... mArgs)	{ Internal::extrObjHelper(fwd<T>(mV), mArgs...); }
		template<typename... TArgs> inline void archObj(Val& mV, TArgs&&... mArgs)				{ mV = Obj{}; Internal::archObjHelper(mV, fwd<TArgs>(mArgs)...); }
		template<typename... TArgs> inline Val getArchObj(TArgs&&... mArgs)						{ Val result; archObj(result, fwd<TArgs>(mArgs)...); return result; }

		template<typename T> inline void convert(const Val& mV, T& mX) noexcept(noexcept(extr(mV, mX)))			{ extr(mV, mX); }
		template<typename T> inline void convert(Val&& mV, T& mX) noexcept(noexcept(extr(std::move(mV), mX)))	{ extr(std::move(mV), mX); }
		template<typename T> inline void convert(Val& mV, T&& mX) noexcept(noexcept(arch(mV, fwd<T>(mX))))		{ arch(mV, fwd<T>(mX)); }

		template<typename... TArgs> inline void convertArr(const Val& mV, TArgs&... mArgs)	{ extrArr(mV, mArgs...); }
		template<typename... TArgs> inline void convertArr(Val&& mV, TArgs&... mArgs)		{ extrArr(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void convertArr(Val& mV, TArgs&&... mArgs)		{ archArr(mV, fwd<TArgs>(mArgs)...); }

		template<typename... TArgs> inline void convertObj(const Val& mV, TArgs&... mArgs)	{ extrObj(mV, mArgs...); }
		template<typename... TArgs> inline void convertObj(Val&& mV, TArgs&... mArgs)		{ extrObj(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void convertObj(Val& mV, TArgs&&... mArgs)		{ archObj(mV, fwd<TArgs>(mArgs)...); }
	}
}

#endif
