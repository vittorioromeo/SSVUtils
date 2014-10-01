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
			template<typename T> struct ValHelper;
		}

		class Val
		{
			template<typename T> friend struct Internal::ValHelper;

			public:
				enum class Type{Obj, Arr, Str, Num, Bln, Nll};

				using Obj = Internal::ObjImpl<Val>;
				using Arr = Internal::ArrImpl<Val>;

			private:
				using Num = Internal::Num;

				Type type{Type::Nll};

				union Holder
				{
					Obj hObj;
					Arr hArr;
					Str hStr;
					Num hNum;
					Bln hBool;

					inline Holder() noexcept { }
					inline ~Holder() noexcept { }
				} h;

				template<typename T> inline void setObj(T&& mX) noexcept(noexcept(Obj{fwd<T>(mX)})) { type = Type::Obj; new(&h.hObj) Obj{fwd<T>(mX)}; }
				template<typename T> inline void setArr(T&& mX) noexcept(noexcept(Arr{fwd<T>(mX)})) { type = Type::Arr; new(&h.hArr) Arr(fwd<T>(mX)); }
				template<typename T> inline void setStr(T&& mX) noexcept(noexcept(Str{fwd<T>(mX)})) { type = Type::Str; new(&h.hStr) Str(fwd<T>(mX)); }

				inline void setNum(const Num& mX) noexcept	{ type = Type::Num; h.hNum = mX; }
				inline void setBln(Bln mX) noexcept			{ type = Type::Bln; h.hBool = mX; }
				inline void setNll(Nll) noexcept			{ type = Type::Nll; }

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

				template<typename T, SSVU_ENABLE_IF_IS_NOT(T, Val)> inline Val(T&& mX) { set(fwd<T>(mX)); }

				inline ~Val() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) noexcept(noexcept(Internal::ValHelper<RemoveAll<T>>::set(std::declval<Val&>(), fwd<T>(mX))))
				{
					deinitCurrent();
					Internal::ValHelper<RemoveAll<T>>::set(*this, fwd<T>(mX));
				}

				// Check stored type
				template<typename T> inline bool is() const noexcept { return Internal::ValHelper<T>::is(*this); }

				// "Explicit" `as` function gets the inner contents of the value
				template<typename T> decltype(auto) as() & noexcept			{ return Internal::ValHelper<T>::as(*this); }
				template<typename T> decltype(auto) as() const& noexcept	{ return Internal::ValHelper<T>::as(*this); }
				template<typename T> decltype(auto) as() && noexcept		{ return Internal::ValHelper<T>::as(std::move(*this)); }

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
					}

					SSVU_JSON_UNREACHABLE();
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
				inline auto asRangeObj() noexcept		{ return asRange(getObj()); }
				inline auto asRangeObj() const noexcept	{ return asRange(getObj()); }

				inline auto asRangeArr() noexcept		{ return asRange(getArr()); }
				inline auto asRangeArr() const noexcept	{ return asRange(getArr()); }

				template<typename TV, typename TF> inline void forObjAs(TF mFunc)		noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(		auto& i : asRangeObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forObjAs(TF mFunc) const	noexcept(noexcept(mFunc(Str{}, std::declval<TV>())))	{ for(const auto& i : asRangeObj()) mFunc(i.first, i.second.template as<TV>()); }

				template<typename TV, typename TF> inline void forArrAs(TF mFunc)		noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(		auto& i : asRangeArr()) mFunc(i.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc) const	noexcept(noexcept(mFunc(std::declval<TV>())))			{ for(const auto& i : asRangeArr()) mFunc(i.template as<TV>()); }

				template<typename TF> inline void forObj(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forObj(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forObjAs<Val>(mFunc)))	{ forObjAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc)			noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
				template<typename TF> inline void forArr(TF mFunc) const	noexcept(noexcept(std::declval<Val&>().forArrAs<Val>(mFunc)))	{ forArrAs<Val>(mFunc); }
		};

		using Obj = Val::Obj;
		using Arr = Val::Arr;
	}
}

#include "SSVUtils/Json/Val/ValHelper.hpp"

#endif
