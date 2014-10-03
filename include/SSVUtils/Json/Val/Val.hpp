// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL
#define SSVU_JSON_VAL

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename T> struct Cnv;
			template<typename T> struct CnvNumImpl;

			struct ValItrHelper
			{
				template<typename TK, typename TV> struct KVPair { TK key; TV value; };
				template<typename TK, typename TV> inline static constexpr auto makeKVPair(TK&& mK, TV&& mV) noexcept { return KVPair<TK, TV>{fwd<TK>(mK), fwd<TV>(mV)}; }

				template<typename T> struct ImplAsObj
				{
					template<typename TItr> inline static constexpr auto get(TItr mItr) noexcept { return makeKVPair(mItr->first, mItr->second.template as<T>());  }
				};

				template<typename T> struct ImplAsArr
				{
					template<typename TItr> inline static constexpr auto get(TItr mItr) noexcept { return mItr->template as<T>();  }
				};

				template<template<typename> class TImpl, typename T, typename TItr> using ItrAs = ssvu::Internal::AdaptorFromItr<TItr, TImpl<T>>;
				template<template<typename> class TImpl, typename T, typename TItr> inline constexpr static auto makeItrAs(TItr mItr) noexcept { return ItrAs<TImpl, T, TItr>{mItr}; }
				template<template<typename> class TImpl, typename T, typename TItr> inline constexpr static auto makeItrAsRange(TItr mBegin, TItr mEnd) noexcept { return makeRange(makeItrAs<TImpl, T>(mBegin), makeItrAs<TImpl, T>(mEnd)); }

				template<typename T, typename TItr> inline static constexpr auto makeItrObjRange(TItr mBegin, TItr mEnd) noexcept { return makeItrAsRange<ImplAsObj, T>(mBegin, mEnd); }
				template<typename T, typename TItr> inline static constexpr auto makeItrArrRange(TItr mBegin, TItr mEnd) noexcept { return makeItrAsRange<ImplAsArr, T>(mBegin, mEnd); }
			};

			using TypeIdx = std::size_t;

			// Returns the next unique bit index for a type
			inline auto getLastTypeIdx() noexcept
			{
				static TypeIdx lastIdx{0};
				return lastIdx++;
			}

			// Stores a specific bit index for a Component type
			template<typename T> struct TypeIdxInfo { static TypeIdx idx; };
			template<typename T> TypeIdx TypeIdxInfo<T>::idx{getLastTypeIdx()};

			// Shortcut to get the bit index of a Component type
			template<typename T> inline auto getTypeIdx() noexcept
			{
				return TypeIdxInfo<T>::idx;
			}
		}

		class Val
		{
			template<typename T> friend struct Internal::Cnv;
			template<typename T> friend struct Internal::CnvNumImpl;

			public:
				enum class Type{Obj, Arr, Str, Num, Bln, Nll};

				using Obj = Internal::ObjImpl<Val>;
				using Arr = Internal::ArrImpl<Val>;

			private:
				using Num = Internal::Num;
				using VIH = Internal::ValItrHelper;
				using CnvType = Internal::TypeIdx;

				Type type{Type::Nll};
				CnvType cnvType;

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

				template<typename T> inline void setObj(T&& mX) noexcept(noexcept(Obj{fwd<T>(mX)})) { type = Type::Obj; cnvType = Internal::getTypeIdx<Obj>(); new(&h.hObj) Obj{fwd<T>(mX)}; }
				template<typename T> inline void setArr(T&& mX) noexcept(noexcept(Arr{fwd<T>(mX)})) { type = Type::Arr; cnvType = Internal::getTypeIdx<Arr>(); new(&h.hArr) Arr(fwd<T>(mX)); }
				template<typename T> inline void setStr(T&& mX) noexcept(noexcept(Str{fwd<T>(mX)})) { type = Type::Str; cnvType = Internal::getTypeIdx<Str>(); new(&h.hStr) Str(fwd<T>(mX)); }

				inline void setNum(const Num& mX) noexcept	{ type = Type::Num; cnvType = Internal::getTypeIdx<Num>(); h.hNum = mX; }
				inline void setBln(Bln mX) noexcept			{ type = Type::Bln; cnvType = Internal::getTypeIdx<Bln>(); h.hBool = mX; }
				inline void setNll(Nll) noexcept			{ type = Type::Nll; cnvType = Internal::getTypeIdx<Nll>(); }

				inline auto& getObj() noexcept				{ SSVU_ASSERT(is<Obj>()); return h.hObj; }
				inline const auto& getObj() const noexcept	{ SSVU_ASSERT(is<Obj>()); return h.hObj; }

				inline auto& getArr() noexcept				{ SSVU_ASSERT(is<Arr>()); return h.hArr; }
				inline const auto& getArr() const noexcept	{ SSVU_ASSERT(is<Arr>()); return h.hArr; }

				inline auto& getStr() noexcept				{ SSVU_ASSERT(is<Str>()); return h.hStr; }
				inline const auto& getStr() const noexcept	{ SSVU_ASSERT(is<Str>()); return h.hStr; }

				inline auto getNum() const noexcept			{ SSVU_ASSERT(is<Num>()); return h.hNum; }
				inline auto getBln() const noexcept			{ SSVU_ASSERT(is<Bln>()); return h.hBool; }
				inline auto getNll() const noexcept			{ SSVU_ASSERT(is<Nll>()); return Nll{}; }

				inline void deinitCurrent()
				{
					switch(type)
					{
						case Type::Obj: h.hObj.~Obj(); break;
						case Type::Arr: h.hArr.~Arr(); break;
						case Type::Str: h.hStr.~Str(); break;
						default: break;
					}
				}

				template<typename T> inline void init(T&& mV)
				{
					switch(mV.type)
					{
						case Type::Obj: setObj(moveIfRValue<decltype(mV)>(mV.getObj())); break;
						case Type::Arr: setArr(moveIfRValue<decltype(mV)>(mV.getArr())); break;
						case Type::Str: setStr(moveIfRValue<decltype(mV)>(mV.getStr())); break;
						case Type::Num: setNum(mV.getNum()); break;
						case Type::Bln: setBln(mV.getBln()); break;
						case Type::Nll: setNll(Nll{}); break;
					}
				}

			public:
				inline Val() = default;
				inline Val(const Val& mV)	{ init(mV); }
				inline Val(Val&& mV)		{ init(std::move(mV)); }

				template<typename T, SSVU_ENABLEIF_IS_NOT(T, Val)> inline Val(T&& mX) { set(fwd<T>(mX)); }

				inline ~Val() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) noexcept(noexcept(Internal::Cnv<RemoveAll<T>>::toVal(std::declval<Val&>(), fwd<T>(mX))))
				{
					deinitCurrent();
					Internal::Cnv<RemoveAll<T>>::toVal(*this, fwd<T>(mX));
					cnvType = Internal::getTypeIdx<T>();
				}

				// Check stored type
				template<typename T> inline bool is() const noexcept { return cnvType == Internal::getTypeIdx<T>(); }

				// "Explicit" `as` function gets the inner contents of the value
				template<typename T> decltype(auto) as() & noexcept(noexcept(Internal::Cnv<T>::getFromVal(std::declval<Val&>())))
				{
					SSVU_ASSERT(is<T>());
					return Internal::Cnv<T>::getFromVal(*this);
				}
				template<typename T> decltype(auto) as() const& noexcept(noexcept(Internal::Cnv<T>::getFromVal(std::declval<Val&>())))
				{
					SSVU_ASSERT(is<T>());
					return Internal::Cnv<T>::getFromVal(*this);
				}
				template<typename T> decltype(auto) as() && noexcept(noexcept(Internal::Cnv<T>::getFromVal(std::declval<Val&&>())))
				{
					SSVU_ASSERT(is<T>());
					return Internal::Cnv<T>::getFromVal(std::move(*this));
				}

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) noexcept(noexcept(std::declval<Val&>().set(fwd<T>(mX))))
				{
					set(fwd<T>(mX));
					return *this;
				}

				// "Implicit" Val from Obj by Key getters
				inline auto& operator[](Key&& mKey)						{ return getObj()[std::move(mKey)]; }
				inline auto& operator[](const Key& mKey)				{ return getObj()[mKey]; }
				inline const auto& operator[](const Key& mKey) const	{ return getObj().at(mKey); }

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
				inline auto forObj() noexcept		{ return asRange(getObj()); }
				inline auto forObj() const noexcept	{ return asRange(getObj()); }
				inline auto forArr() noexcept		{ return asRange(getArr()); }
				inline auto forArr() const noexcept	{ return asRange(getArr()); }

				template<typename T> inline auto forObjAs() noexcept		{ return VIH::makeItrObjRange<T>(std::begin(getObj()), std::end(getObj())); }
				template<typename T> inline auto forObjAs() const noexcept	{ return VIH::makeItrObjRange<T>(std::cbegin(getObj()), std::cend(getObj())); }
				template<typename T> inline auto forArrAs() noexcept		{ return VIH::makeItrArrRange<T>(std::begin(getArr()), std::end(getArr())); }
				template<typename T> inline auto forArrAs() const noexcept	{ return VIH::makeItrArrRange<T>(std::cbegin(getArr()), std::cend(getArr())); }

				template<typename TV, typename TF> inline void forObjAs(TF mFunc)		noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(		auto& i : forObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forObjAs(TF mFunc) const	noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(const auto& i : forObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc)		noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(		auto& i : forArr()) mFunc(i.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc) const	noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(const auto& i : forArr()) mFunc(i.template as<TV>()); }

				template<typename TF> inline void forObj(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forObj(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
		};

		using Obj = Val::Obj;
		using Arr = Val::Arr;

		namespace Internal
		{
			template<Idx TI, typename TArg, typename T> inline void extrArrHelper(T&& mV, TArg& mArg) { mArg = moveIfRValue<decltype(mV)>(mV[TI].template as<TArg>()); }
			template<Idx TI, typename TArg, typename... TArgs, typename T> inline void extrArrHelper(T&& mV, TArg& mArg, TArgs&... mArgs)
			{
				extrArrHelper<TI>(fwd<T>(mV), mArg); extrArrHelper<TI + 1>(fwd<T>(mV), mArgs...);
			}

			template<Idx TI, typename TArg> inline void archArrHelper(Val& mV, TArg&& mArg) { mV[TI] = fwd<TArg>(mArg); }
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


		template<typename... TArgs, typename T> inline void extrArr(T&& mV, TArgs&... mArgs)	{ Internal::extrArrHelper<0>(fwd<T>(mV), mArgs...); }
		template<typename... TArgs> inline void archArr(Val& mV, TArgs&&... mArgs)				{ Internal::archArrHelper<0>(mV, fwd<TArgs>(mArgs)...); }
		//template<typename... TArgs> inline Val getArchArr(const TArgs&... mArgs) { Val result; archArr(result, fwd<const TArgs&>(mArgs)...); return result; }

		template<typename... TArgs, typename T> inline void extrObj(T&& mV, TArgs&... mArgs)	{ Internal::extrObjHelper(fwd<T>(mV), mArgs...); }
		template<typename... TArgs> inline void archObj(Val& mV, TArgs&&... mArgs)				{ Internal::archObjHelper(mV, fwd<TArgs>(mArgs)...); }
		//template<typename... TArgs> inline Val getArchVal(const TArgs&... mArgs) { Val result; archObj(result, fwd<const TArgs&>(mArgs)...); return result; }

		template<typename T> inline void convert(const Val& mV, T& mX)	{ mX = mV.as<T>(); }
		template<typename T> inline void convert(Val&& mV, T& mX)		{ mX = std::move(mV.as<T>()); }
		template<typename T> inline void convert(Val& mV, T&& mX)		{ mV = fwd<T>(mX); }

		template<typename... TArgs> inline void convertArr(const Val& mV, TArgs&... mArgs)	{ extrArr(mV, mArgs...); }
		template<typename... TArgs> inline void convertArr(Val&& mV, TArgs&... mArgs)		{ extrArr(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void convertArr(Val& mV, TArgs&&... mArgs)		{ archArr(mV, fwd<TArgs>(mArgs)...); }

		template<typename... TArgs> inline void convertObj(const Val& mV, TArgs&... mArgs)	{ extrObj(mV, mArgs...); }
		template<typename... TArgs> inline void convertObj(Val&& mV, TArgs&... mArgs)		{ extrObj(std::move(mV), mArgs...); }
		template<typename... TArgs> inline void convertObj(Val& mV, TArgs&&... mArgs)		{ archObj(mV, fwd<TArgs>(mArgs)...); }

	}
}

#include "SSVUtils/Json/Val/Cnv.hpp"

#endif
