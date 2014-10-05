// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL
#define SSVU_JSON_VAL

#include "SSVUtils/Json/Val/Internal/Fwd.hpp"
#include "SSVUtils/Json/Val/Internal/ValItrHelper.hpp"

namespace ssvu
{
	namespace Json
	{
		class Val
		{
			template<typename T> friend struct Internal::Cnv;
			template<typename T> friend struct Internal::Checker;
			template<typename T> friend struct Internal::AsHelper;

			public:
				enum class Type{Obj, Arr, Str, Num, Bln, Nll};

				using Obj = Internal::ObjImpl<Val>;
				using Arr = Internal::ArrImpl<Val>;

			private:
				using Num = Internal::Num;
				using VIH = Internal::ValItrHelper;

				Type type{Type::Nll};

				#if SSVU_DEBUG
					bool clean{true};
				#endif

				inline void setClean(bool mClean) noexcept
				{
					#if SSVU_DEBUG
						clean = mClean;
					#endif
				}
				inline bool isClean() const noexcept
				{
					#if SSVU_DEBUG
						return clean;
					#else
						return true;
					#endif
				}

				union Holder
				{
					Obj hObj;
					Arr hArr;
					Str hStr;
					Num hNum;
					Bln hBool;
\
					inline Holder() noexcept { }
					inline ~Holder() noexcept { }
				} h;

				template<typename T> inline void setObj(T&& mX) noexcept(noexcept(Obj{fwd<T>(mX)}))	{ SSVU_ASSERT(isClean()); type = Type::Obj; new(&h.hObj) Obj{fwd<T>(mX)}; }
				template<typename T> inline void setArr(T&& mX) noexcept(noexcept(Arr{fwd<T>(mX)}))	{ SSVU_ASSERT(isClean()); type = Type::Arr; new(&h.hArr) Arr(fwd<T>(mX)); }
				template<typename T> inline void setStr(T&& mX) noexcept(noexcept(Str{fwd<T>(mX)}))	{ SSVU_ASSERT(isClean()); type = Type::Str; new(&h.hStr) Str(fwd<T>(mX)); }

				inline void setNum(const Num& mX) noexcept											{ SSVU_ASSERT(isClean()); type = Type::Num; h.hNum = mX; }
				inline void setBln(Bln mX) noexcept													{ SSVU_ASSERT(isClean()); type = Type::Bln; h.hBool = mX; }
				inline void setNll(Nll) noexcept													{ SSVU_ASSERT(isClean()); type = Type::Nll; }

				inline auto& getObj() noexcept				{ SSVU_ASSERT(is<Obj>() && !isClean()); return h.hObj; }
				inline const auto& getObj() const noexcept	{ SSVU_ASSERT(is<Obj>() && !isClean()); return h.hObj; }

				inline auto& getArr() noexcept				{ SSVU_ASSERT(is<Arr>() && !isClean()); return h.hArr; }
				inline const auto& getArr() const noexcept	{ SSVU_ASSERT(is<Arr>() && !isClean()); return h.hArr; }

				inline auto& getStr() noexcept				{ SSVU_ASSERT(is<Str>() && !isClean()); return h.hStr; }
				inline const auto& getStr() const noexcept	{ SSVU_ASSERT(is<Str>() && !isClean()); return h.hStr; }

				inline auto getNum() const noexcept			{ SSVU_ASSERT(is<Num>() && !isClean()); return h.hNum; }
				inline auto getBln() const noexcept			{ SSVU_ASSERT(is<Bln>() && !isClean()); return h.hBool; }
				inline auto getNll() const noexcept			{ SSVU_ASSERT(is<Nll>() && !isClean()); return Nll{}; }

				inline void deinitCurrent()
				{
					switch(type)
					{
						case Type::Obj: SSVU_ASSERT(!isClean()); h.hObj.~Obj(); break;
						case Type::Arr: SSVU_ASSERT(!isClean()); h.hArr.~Arr(); break;
						case Type::Str: SSVU_ASSERT(!isClean()); h.hStr.~Str(); break;
						default: break;
					}

					setClean(true);
				}

				template<typename T> inline void init(T&& mV)
				{
					SSVU_ASSERT(isClean());

					switch(mV.type)
					{
						case Type::Obj: setObj(moveIfRValue<decltype(mV)>(mV.getObj())); break;
						case Type::Arr: setArr(moveIfRValue<decltype(mV)>(mV.getArr())); break;
						case Type::Str: setStr(moveIfRValue<decltype(mV)>(mV.getStr())); break;
						case Type::Num: setNum(mV.getNum()); break;
						case Type::Bln: setBln(mV.getBln()); break;
						case Type::Nll: setNll(Nll{}); break;
					}

					setClean(false);
				}

			public:
				inline Val() noexcept = default;
				inline Val(const Val& mV)	{ init(mV);  }
				inline Val(Val&& mV)		{ init(std::move(mV)); }

				template<typename T, SSVU_ENABLEIF_IS_NOT(T, Val)> inline Val(T&& mX) { set(fwd<T>(mX)); }

				inline ~Val() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) noexcept(noexcept(Internal::Cnv<RemoveAll<T>>::toVal(std::declval<Val&>(), fwd<T>(mX))))
				{
					deinitCurrent();
					Internal::Cnv<RemoveAll<T>>::toVal(*this, fwd<T>(mX));
					setClean(false);
				}

				// Check stored type
				template<typename T> inline bool is() const noexcept
				{
					return Internal::Checker<RemoveAll<T>>::is(*this);
				}

				// "Explicit" `as` function gets the inner contents of the value
				//template<typename T> decltype(auto) as() &;
				//template<typename T> decltype(auto) as() const&;
				//template<typename T> decltype(auto) as() &&;
				template<typename T> decltype(auto) as();
				template<typename T> decltype(auto) as() const;

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) noexcept(noexcept(std::declval<Val&>().set(fwd<T>(mX))))
				{
					set(fwd<T>(mX));
					return *this;
				}

				// "Implicit" Val from Obj by Key getters
				inline auto& operator[](Key&& mKey)								{ return getObj()[std::move(mKey)]; }
				inline auto& operator[](const Key& mKey)						{ return getObj()[mKey]; }
				inline const auto& operator[](const Key& mKey) const noexcept	{ return getObj().atOrDefault(mKey); }

				// "Implicit" Val from Arr by Idx getters
				inline auto& operator[](Idx mIdx) 				{ return getArr()[mIdx]; }
				inline const auto& operator[](Idx mIdx) const	{ return getArr().at(mIdx); }

				inline bool has(const Key& mKey) const noexcept { return getObj().has(mKey); }

				inline auto getType() const noexcept { return type; }

				template<typename T> inline decltype(auto) getIfHas(const Key& mKey, T&& mDef) const
				{
					return has(mKey) ? operator[](mKey).as<T>() : fwd<T>(mDef);
				}



				// Equality
				inline bool operator==(const Val& mV) const noexcept
				{
					SSVU_ASSERT(!isClean());

					if(type != mV.type) return false;

					switch(type)
					{
						case Type::Obj: return getObj() == mV.getObj();
						case Type::Arr: return getArr() == mV.getArr();
						case Type::Str: return getStr() == mV.getStr();
						case Type::Num: return getNum() == mV.getNum();
						case Type::Bln: return getBln() == mV.getBln();
						case Type::Nll: return true;
						default: SSVU_UNREACHABLE();
					}
				}
				inline auto operator!=(const Val& mV) const noexcept { return !(operator==(mV)); }




				// IO
				template<typename TWS = WriterSettings<WMode::Pretty>> void writeToStream(std::ostream&) const;
				template<typename TWS = WriterSettings<WMode::Pretty>> inline void writeToStr(std::string& mStr) const			{ std::ostringstream o; writeToStream<TWS>(o); mStr = o.str(); }
				template<typename TWS = WriterSettings<WMode::Pretty>> inline void writeToFile(const ssvufs::Path& mPath) const	{ std::ofstream o{mPath}; writeToStream<TWS>(o); o.close(); }
				template<typename TWS = WriterSettings<WMode::Pretty>> inline auto getWriteToStr() const						{ std::string result; writeToStr<TWS>(result); return result; }

				template<typename TRS = ReaderSettings<RMode::Default>, typename T> void readFromStr(T&& mStr);
				template<typename TRS = ReaderSettings<RMode::Default>> inline void readFromFile(const ssvufs::Path& mPath) { readFromStr(mPath.getContentsAsString()); }

				template<typename T> static inline Val fromStr(T&& mStr)	{ Val result; result.readFromStr(fwd<T>(mStr)); return result; }
				static inline Val fromFile(const ssvufs::Path& mPath)		{ Val result; result.readFromFile(mPath); return result; }



				// Iteration
				/*
				template<typename T> inline auto forObjAs() noexcept		{ return VIH::makeItrObjRange<T>(std::begin(getObj()), std::end(getObj())); }
				template<typename T> inline auto forObjAs() const noexcept	{ return VIH::makeItrObjRange<T>(std::cbegin(getObj()), std::cend(getObj())); }
				template<typename T> inline auto forArrAs() noexcept		{ return VIH::makeItrArrRange<T>(std::begin(getArr()), std::end(getArr())); }
				template<typename T> inline auto forArrAs() const noexcept	{ return VIH::makeItrArrRange<T>(std::cbegin(getArr()), std::cend(getArr())); }

				inline auto forObj() noexcept		{ return forObjAs<Val>(); }
				inline auto forObj() const noexcept	{ return forObjAs<Val>(); }
				inline auto forArr() noexcept		{ return forArrAs<Val>(); }
				inline auto forArr() const noexcept	{ return forArrAs<Val>(); }
				*/

				template<typename T> inline auto forUncheckedObjAs() noexcept		{ return VIH::makeItrObjRange<T>(std::begin(getObj()), std::end(getObj())); }
				template<typename T> inline auto forUncheckedObjAs() const noexcept	{ return VIH::makeItrObjRange<T>(std::cbegin(getObj()), std::cend(getObj())); }
				template<typename T> inline auto forUncheckedArrAs() noexcept		{ return VIH::makeItrArrRange<T>(std::begin(getArr()), std::end(getArr())); }
				template<typename T> inline auto forUncheckedArrAs() const noexcept	{ return VIH::makeItrArrRange<T>(std::cbegin(getArr()), std::cend(getArr())); }

				template<typename T> inline auto forObjAs() noexcept		{ return is<Obj>() ? forUncheckedObjAs<T>() : VIH::makeItrObjRangeEmpty<T, decltype(std::end(h.hObj))>(); }
				template<typename T> inline auto forObjAs() const noexcept	{ return is<Obj>() ? forUncheckedObjAs<T>() : VIH::makeItrObjRangeEmpty<T, decltype(std::cend(h.hObj))>(); }
				template<typename T> inline auto forArrAs() noexcept		{ return is<Arr>() ? forUncheckedArrAs<T>() : VIH::makeItrArrRangeEmpty<T, decltype(std::end(h.hArr))>(); }
				template<typename T> inline auto forArrAs() const noexcept	{ return is<Arr>() ? forUncheckedArrAs<T>() : VIH::makeItrArrRangeEmpty<T, decltype(std::cend(h.hArr))>(); }

				inline auto forUncheckedObj() noexcept			{ return forUncheckedObjAs<Val>(); }
				inline auto forUncheckedObj() const noexcept	{ return forUncheckedObjAs<Val>(); }
				inline auto forUncheckedArr() noexcept			{ return forUncheckedArrAs<Val>(); }
				inline auto forUncheckedArr() const noexcept	{ return forUncheckedArrAs<Val>(); }

				inline auto forObj() noexcept		{ return forObjAs<Val>(); }
				inline auto forObj() const noexcept	{ return forObjAs<Val>(); }
				inline auto forArr() noexcept		{ return forArrAs<Val>(); }
				inline auto forArr() const noexcept	{ return forArrAs<Val>(); }

				/*
				template<typename TV, typename TF> inline void forObjAs(TF mFunc)		noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(		auto& i : forObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forObjAs(TF mFunc) const	noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(const auto& i : forObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc)		noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(		auto& i : forArr()) mFunc(i.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc) const	noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(const auto& i : forArr()) mFunc(i.template as<TV>()); }

				template<typename TF> inline void forObj(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forObj(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
				*/
		};

		using Obj = Val::Obj;
		using Arr = Val::Arr;
	}
}

#include "SSVUtils/Json/Val/Internal/Cnv.hpp"
#include "SSVUtils/Json/Val/Internal/Checker.hpp"
#include "SSVUtils/Json/Val/Internal/Copier.hpp"

#endif
