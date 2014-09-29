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

				template<typename T> inline void setObj(T&& mX)	{ type = Type::Obj; new(&h.hObj) Obj(fwd<T>(mX)); }
				template<typename T> inline void setArr(T&& mX)	{ type = Type::Arr; new(&h.hArr) Arr(fwd<T>(mX)); }
				template<typename T> inline void setStr(T&& mX)	{ type = Type::Str; new(&h.hStr) Str(fwd<T>(mX)); }
				inline void setNum(const Num& mX) noexcept		{ type = Type::Num; h.hNum = mX; }
				inline void setBln(Bln mX) noexcept				{ type = Type::Bln; h.hBool = mX; }
				inline void setNll(Nll) noexcept				{ type = Type::Nll; }

				inline auto& getObj() & noexcept			{ SSVU_ASSERT(is<Obj>()); return h.hObj; }
				inline const auto& getObj() const& noexcept	{ SSVU_ASSERT(is<Obj>()); return h.hObj; }
				inline auto getObjMove() && noexcept		{ SSVU_ASSERT(is<Obj>()); return std::move(h.hObj); }

				inline auto& getArr() & noexcept			{ SSVU_ASSERT(is<Arr>()); return h.hArr; }
				inline const auto& getArr() const& noexcept	{ SSVU_ASSERT(is<Arr>()); return h.hArr; }
				inline auto getArrMove() && noexcept		{ SSVU_ASSERT(is<Arr>()); return std::move(h.hArr); }

				inline auto& getStr() & noexcept			{ SSVU_ASSERT(is<Str>()); return h.hStr; }
				inline const auto& getStr() const& noexcept	{ SSVU_ASSERT(is<Str>()); return h.hStr; }
				inline auto getStrMove() && noexcept		{ SSVU_ASSERT(is<Str>()); return std::move(h.hStr); }

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
						case Type::Obj: setObj(fwd<T>(mV).getObj()); break;
						case Type::Arr: setArr(fwd<T>(mV).getArr()); break;
						case Type::Str: setStr(fwd<T>(mV).getStr()); break;
						case Type::Num: setNum(fwd<T>(mV).getNum()); break;
						case Type::Bln: setBln(fwd<T>(mV).getBln()); break;
						case Type::Nll: setNll(Nll{}); break;
						default: break;
					}
				}

			public:
				inline Val() = default;
				inline Val(const Val& mV)	{ init(mV); }
				inline Val(Val&& mV)		{ init(std::move(mV)); }

				template<typename T, SSVU_JSON_ENABLE_IF_IS_NOT(T, Val)> inline Val(T&& mX) { set(fwd<T>(mX)); }

				inline ~Val() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) { deinitCurrent(); Internal::ValHelper<RemoveAll<T>>::set(*this, fwd<T>(mX)); }

				// Check stored type
				template<typename T> inline bool is() const noexcept { return Internal::ValHelper<T>::is(*this); }

				// "Explicit" `as` function gets the inner contents of the value
				template<typename T> decltype(auto) as() & noexcept			{ return Internal::ValHelper<T>::as(*this); }
				template<typename T> decltype(auto) as() const& noexcept	{ return Internal::ValHelper<T>::as(*this); }
				template<typename T> decltype(auto) as() && noexcept		{ return Internal::ValHelper<T>::as(std::move(*this)); }

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) { set(fwd<T>(mX)); return *this; }

				// "Implicit" Val from Obj by Key getters
				inline auto& operator[](const Key& mKey) &				{ return getObj()[mKey]; }
				inline const auto& operator[](const Key& mKey) const&	{ return getObj().at(mKey); }

				// "Implicit" Val from Arr by Idx getters
				inline auto& operator[](Idx mIdx) &				{ return getArr()[mIdx]; }
				inline const auto& operator[](Idx mIdx) const&	{ return getArr().at(mIdx); }

				inline bool has(const Key& mKey) const noexcept { return getObj().has(mKey); }

				inline auto getType() const noexcept { return type; }



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

					std::terminate();
				}
				inline auto operator!=(const Val& mV) const noexcept { return !(operator==(mV)); }




				// IO
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToStream(std::ostream&) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToStr(std::string&) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToFile(const ssvufs::Path&) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> auto getWriteToStr() const;

				void readFromStr(std::string mStr);
				void readFromFile(const ssvufs::Path& mPath);

				static inline Val fromStr(const std::string& mStr)		{ Val result; result.readFromStr(mStr); return result; }
				static inline Val fromFile(const ssvufs::Path& mPath)	{ Val result; result.readFromFile(mPath); return result; }



				// Iteration
				inline auto makeRangeObj() noexcept			{ return makeRange(getObj()); }
				inline auto makeRangeObj() const noexcept	{ return makeRange(getObj()); }

				inline auto makeRangeArr() noexcept			{ return makeRange(getArr()); }
				inline auto makeRangeArr() const noexcept	{ return makeRange(getArr()); }

				template<typename TF> inline void forObj(TF mFunc)						{ for(		auto& i : makeRangeObj()) mFunc(i.first, i.second); }
				template<typename TF> inline void forObj(TF mFunc) const				{ for(const auto& i : makeRangeObj()) mFunc(i.first, i.second); }
				template<typename TV, typename TF> inline void forObjAs(TF mFunc)		{ for(		auto& i : makeRangeObj()) mFunc(i.first, i.second.template as<TV>()); }
				template<typename TV, typename TF> inline void forObjAs(TF mFunc) const	{ for(const auto& i : makeRangeObj()) mFunc(i.first, i.second.template as<TV>()); }

				template<typename TF> inline void forArr(TF mFunc)						{ for(		auto& i : makeRangeArr()) mFunc(i); }
				template<typename TF> inline void forArr(TF mFunc) const				{ for(const auto& i : makeRangeArr()) mFunc(i); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc)		{ for(		auto& i : makeRangeArr()) mFunc(i.template as<TV>()); }
				template<typename TV, typename TF> inline void forArrAs(TF mFunc) const	{ for(const auto& i : makeRangeArr()) mFunc(i.template as<TV>()); }
		};

		using Obj = Val::Obj;
		using Arr = Val::Arr;
	}
}

#include "SSVUtils/Json/Val/ValHelper.hpp"

#endif
