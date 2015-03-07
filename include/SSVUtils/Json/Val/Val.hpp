// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL
#define SSVU_JSON_VAL

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Union/Union.hpp"
#include "SSVUtils/Json/Common/Common.hpp"
#include "SSVUtils/Json/Num/Num.hpp"
#include "SSVUtils/Json/Val/Internal/Fwd.hpp"
#include "SSVUtils/Json/Val/Internal/ItrHelper.hpp"

namespace ssvu
{
	namespace Json
	{
		namespace Impl
		{
			class Val
			{
				template<typename> friend struct Impl::Cnv;
				template<typename> friend struct Impl::Chk;
				template<typename> friend struct Impl::AsHelper;
				friend struct Impl::TplCnvHelper;
				friend struct Impl::TplIsHelper;
				friend struct Impl::CnvFuncHelper;

				public:
					/// @brief Internal storage type.
					enum class Type{TObj, TArr, TStr, TNum, TBln, TNll};

					/// @typedef `Obj` implementation type, templatized with `Val`.
					using Obj = Impl::ObjImpl<Val>;

					/// @typedef `Arr` implementation type, templatized with `Val`.
					using Arr = Impl::ArrImpl<Val>;

				private:
					// Shortcut typedefs
					using Num = Impl::Num;
					using VIH = Impl::ItrHelper;

					/// @brief Current storage type.
					Type type{Type::TNll};

					/// @brief Checked union storage for `Val` fundamental types.
					Union<Obj, Arr, Str, Num, Bln> h;

					// Perfect-forwarding setters
					template<typename T> inline void setObj(T&& mX) noexcept(noexcept(Obj{FWD(mX)}))	{ type = Type::TObj; h.init<Obj>(FWD(mX)); }
					template<typename T> inline void setArr(T&& mX) noexcept(noexcept(Arr{FWD(mX)}))	{ type = Type::TArr; h.init<Arr>(FWD(mX)); }
					template<typename T> inline void setStr(T&& mX) noexcept(noexcept(Str{FWD(mX)}))	{ type = Type::TStr; h.init<Str>(FWD(mX)); }
					template<typename T> inline void setNum(T&& mX) noexcept(noexcept(Num{FWD(mX)}))	{ type = Type::TNum; h.init<Num>(FWD(mX)); }

					// Basic setters
					inline void setBln(Bln mX) noexcept	{ type = Type::TBln; h.init<Bln>(mX); }
					inline void setNll(Nll) noexcept	{ type = Type::TNll; }

					// Ref-qualified getters
					#define SSVJ_DEFINE_VAL_GETTER(mType, mMember) \
						inline mType&		SSVPP_CAT(get, mType)() & noexcept		{ SSVU_ASSERT(is<mType>()); return mMember; } \
						inline const mType&	SSVPP_CAT(get, mType)() const& noexcept	{ SSVU_ASSERT(is<mType>()); return mMember; } \
						inline mType		SSVPP_CAT(get, mType)() && noexcept		{ SSVU_ASSERT(is<mType>()); return move(mMember); }

					SSVJ_DEFINE_VAL_GETTER(Obj, h.get<Obj>())
					SSVJ_DEFINE_VAL_GETTER(Arr, h.get<Arr>())
					SSVJ_DEFINE_VAL_GETTER(Str, h.get<Str>())
					SSVJ_DEFINE_VAL_GETTER(Num, h.get<Num>())

					#undef SSVJ_DEFINE_VAL_GETTER

					// Other getters
					inline auto getBln() const noexcept { SSVU_ASSERT(is<Bln>()); return h.get<Bln>(); }
					inline auto getNll() const noexcept { SSVU_ASSERT(is<Nll>()); return Nll{}; }

					/// @brief Deinitializes the current storage, calling the appropriate destructor.
					inline void deinitCurrent()
					{
						switch(type)
						{
							case Type::TObj: h.deinit<Obj>(); break;
							case Type::TArr: h.deinit<Arr>(); break;
							case Type::TStr: h.deinit<Str>(); break;
							case Type::TNum: h.deinit<Num>(); break;
							case Type::TBln: h.deinit<Bln>(); break;
							default: break;
						}
					}

					/// @brief Initializes the current storage from `mV`, calling the appropriate setter function.
					template<typename T> inline void init(T&& mV)
					{
						switch(mV.type)
						{
							case Type::TObj: setObj(moveIfRValue<decltype(mV)>(mV.getObj())); break;
							case Type::TArr: setArr(moveIfRValue<decltype(mV)>(mV.getArr())); break;
							case Type::TStr: setStr(moveIfRValue<decltype(mV)>(mV.getStr())); break;
							case Type::TNum: setNum(mV.getNum()); break;
							case Type::TBln: setBln(mV.getBln()); break;
							case Type::TNll: setNll({}); break;
						}
					}

					/// @brief Checks the stored type. Doesn't check number representation.
					template<typename T> inline bool isNoNum() const noexcept
					{
						return Impl::ChkNoNum<RmAll<T>>::is(*this);
					}

				public:
					inline Val() noexcept = default;

					// Copy/move constructors
					inline Val(const Val& mV)	{ init(mV); }
					inline Val(Val&& mV)		{ init(move(mV)); }

					/// @brief Constructs the `Val` from `mX`.
					template<typename T, SSVU_ENABLEIF_RA_IS_NOT(T, Val)> inline Val(T&& mX) { set(FWD(mX)); }

					// Destructor must deinitalize
					inline ~Val() { deinitCurrent(); }

					/// @brief Sets the `Val`'s internal value to `mX`.
					///	@details The current stored value is deinitialized first.
					template<typename T> inline void set(T&& mX) noexcept(noexcept(Impl::Cnv<RmAll<T>>::toVal(std::declval<Val&>(), FWD(mX))))
					{
						deinitCurrent();
						Impl::Cnv<RmAll<T>>::toVal(*this, FWD(mX));
					}

					/// @brief Checks if the stored internal value is of type `T`.
					/// @details If checking number representation is required, only use `IntS`, `IntU` or `Real`.
					///	Any other numeric type will not work.
					template<typename T> inline bool is() const noexcept
					{
						return Impl::Chk<RmAll<T>>::is(*this);
					}

					/// @brief Gets the internal value as `T`. (non-const version)
					/// @details Returns a copy for most types, a reference for `Obj`, `Arr` and `Str`.
					template<typename T> auto as() & -> decltype(Impl::AsHelper<T>::as(*this));

					/// @brief Gets the internal value as `T`. (const version)
					/// @details Returns a copy for most types, a const reference for `Obj`, `Arr` and `Str`.
					template<typename T> auto as() const& -> decltype(Impl::AsHelper<T>::as(*this));

					/// @brief Gets the internal value as `T`. (rvalue version)
					/// @details Returns a copy for most types, a const reference for `Obj`, `Arr` and `Str`.
					template<typename T> auto as() && -> decltype(Impl::AsHelper<T>::as(*this));

					// "Implicit" `set` function done via `operator=` overloading
					auto& operator=(const Val& mV) noexcept;
					auto& operator=(Val&& mV) noexcept;
					template<typename T> inline auto& operator=(T&& mX) noexcept(noexcept(std::declval<Val&>().set(FWD(mX))))
					{
						set(FWD(mX));
						return *this;
					}

					// "Implicit" Val from Obj by Key getters
					inline auto& operator[](Key&& mKey)								{ return getObj()[move(mKey)]; }
					inline auto& operator[](const Key& mKey)						{ return getObj()[mKey]; }
					inline const auto& operator[](const Key& mKey) const noexcept	{ return getObj().atOrDefault(mKey); }

					// "Implicit" Val from Arr by Idx getters
					inline auto& operator[](Idx mIdx) noexcept		{ return getArr()[mIdx]; }
					inline const auto& operator[](Idx mIdx) const	{ return getArr().at(mIdx); }

					/// @brief Returns true if this `Obj` `Val` instance has a value with key `mKey`.
					/// @details Must only be called on `Val` instances storing an `Obj`.
					inline bool has(const Key& mKey) const noexcept { return getObj().has(mKey); }

					/// @brief Returns true if this `Obj` `Arr` instance has a value with index `mIdx`.
					/// @details Must only be called on `Val` instances storing an `Arr`.
					inline bool has(Idx mIdx) const noexcept { return getArr().size() > mIdx; }

					/// @brief Returns the current internal storage type.
					inline auto getType() const noexcept { return type; }

					/// @brief Returns the value with key `mKey` is existant, otherwise `mDef`.
					/// @details Must only be called on `Val` instances storing an `Obj`.
					template<typename T> inline decltype(auto) getIfHas(const Key& mKey, const T& mDef) const
					{
						return has(mKey) ? operator[](mKey).as<T>() : mDef;
					}

					/// @brief Returns the value with index `mIdx` is existant, otherwise `mDef`.
					/// @details Must only be called on `Val` instances storing an `Arr`.
					template<typename T> inline decltype(auto) getIfHas(Idx mIdx, const T& mDef) const
					{
						return has(mIdx) ? operator[](mIdx).as<T>() : mDef;
					}

					// Equality/inequality
					inline bool operator==(const Val& mV) const noexcept
					{
						if(type != mV.type) return false;

						switch(type)
						{
							case Type::TObj: return getObj() == mV.getObj();
							case Type::TArr: return getArr() == mV.getArr();
							case Type::TStr: return getStr() == mV.getStr();
							case Type::TNum: return getNum() == mV.getNum();
							case Type::TBln: return getBln() == mV.getBln();
							case Type::TNll: return true;
							default: SSVU_UNREACHABLE();
						}
					}
					inline auto operator!=(const Val& mV) const noexcept { return !(operator==(mV)); }

					// IO writing implementations
					template<typename TWS = WSPretty> void writeToStream(std::ostream&) const;
					template<typename TWS = WSPretty> inline void writeToStr(std::string& mStr) const			{ std::ostringstream o; writeToStream<TWS>(o); mStr = o.str(); }
					template<typename TWS = WSPretty> inline void writeToFile(const ssvufs::Path& mPath) const	{ std::ofstream o{mPath}; writeToStream<TWS>(o); o.close(); }
					template<typename TWS = WSPretty> inline auto getWriteToStr() const							{ std::string result; writeToStr<TWS>(result); return result; }

					// IO reading implementations
					template<typename TRS = RSDefault, typename T> void readFromStr(T&& mStr);
					template<typename TRS = RSDefault> inline void readFromFile(const ssvufs::Path& mPath) { readFromStr(mPath.getContentsAsString()); }

					// Construction from strings or files
					template<typename T> inline static Val fromStr(T&& mStr)	{ Val result; result.readFromStr(FWD(mStr)); return result; }
					inline static Val fromFile(const ssvufs::Path& mPath)		{ Val result; result.readFromFile(mPath); return result; }

					// Unchecked casted iteration
					template<typename T> inline auto forUncheckedObjAs() noexcept		{ return VIH::makeItrObjRange<T>(std::begin(getObj()), std::end(getObj())); }
					template<typename T> inline auto forUncheckedObjAs() const noexcept	{ return VIH::makeItrObjRange<T>(std::cbegin(getObj()), std::cend(getObj())); }
					template<typename T> inline auto forUncheckedArrAs() noexcept		{ return VIH::makeItrArrRange<T>(std::begin(getArr()), std::end(getArr())); }
					template<typename T> inline auto forUncheckedArrAs() const noexcept	{ return VIH::makeItrArrRange<T>(std::cbegin(getArr()), std::cend(getArr())); }

					// Checked casted iteration
					template<typename T> inline auto forObjAs() noexcept		{ return is<Obj>() ? forUncheckedObjAs<T>() : VIH::makeItrObjRangeEmpty<T, decltype(std::end(h.get<Obj>()))>(); }
					template<typename T> inline auto forObjAs() const noexcept	{ return is<Obj>() ? forUncheckedObjAs<T>() : VIH::makeItrObjRangeEmpty<T, decltype(std::cend(h.get<Obj>()))>(); }
					template<typename T> inline auto forArrAs() noexcept		{ return is<Arr>() ? forUncheckedArrAs<T>() : VIH::makeItrArrRangeEmpty<T, decltype(std::end(h.get<Arr>()))>(); }
					template<typename T> inline auto forArrAs() const noexcept	{ return is<Arr>() ? forUncheckedArrAs<T>() : VIH::makeItrArrRangeEmpty<T, decltype(std::cend(h.get<Arr>()))>(); }

					// Unchecked non-casted iteration
					auto forUncheckedObj() noexcept;
					auto forUncheckedObj() const noexcept;
					auto forUncheckedArr() noexcept;
					auto forUncheckedArr() const noexcept;

					// Checked non-casted iteration
					auto forObj() noexcept;
					auto forObj() const noexcept;
					auto forArr() noexcept;
					auto forArr() const noexcept;

					/// @brief Emplaces a value back into this `Val` instance's `Arr`.
					/// @details Must only be called on `Val` instances storing an `Arr`.
					template<typename T> inline void emplace(T&& mX) noexcept(noexcept(std::declval<Arr>().emplace_back(FWD(mX))))
					{
						getArr().emplace_back(FWD(mX));
					}

					// Size getters
					inline auto getSizeArr() const noexcept { return getArr().size(); }
					inline auto getSizeObj() const noexcept { return getObj().size(); }

					// Empty tests
					inline bool isEmptyArr() const noexcept { return getSizeArr() == 0; }
					inline bool isEmptyObj() const noexcept { return getSizeObj() == 0; }
			};

			/// @typedef `Obj` implementation typedef, templatized with `Val`.
			using Obj = Val::Obj;

			/// @typedef `Arr` implementation typedef, templatized with `Val`.
			using Arr = Val::Arr;
		}

		/// @typedef `Val` - json value.
		using Val = Impl::Val;

		/// @brief Returns a JSON value containing an empty JSON object.
		inline auto mkObj() { return Val{Impl::Obj{}}; }

		/// @brief Returns a JSON value containing a JSON object filled with the passed key-value pairs.
		inline auto mkObj(std::initializer_list<std::pair<Key, Val>>&& mX) { return Val{Impl::Obj{std::move(mX)}}; }

		/// @brief Returns a JSON value containing a JSON array filled with the passed arguments.
		template<typename... TArgs> inline auto mkArr(TArgs&&... mArgs) { return Val{Impl::Arr{FWD(mArgs)...}}; }

		/// @brief Returns a JSON value constructed from the string `mStr`.
		template<typename T> inline auto fromStr(T&& mStr) { return Val::fromStr(FWD(mStr)); }

		/// @brief Returns a JSON value constructed from the file in `mPath`.
		inline auto fromFile(const ssvufs::Path& mPath) { return Val::fromFile(mPath); }
	}
}

#include "SSVUtils/Json/Val/Internal/Cnv.hpp"
#include "SSVUtils/Json/Val/Internal/CnvFuncs.hpp"
#include "SSVUtils/Json/Val/Internal/CnvMacros.hpp"
#include "SSVUtils/Json/Val/Internal/Chk.hpp"
#include "SSVUtils/Json/Val/Internal/AsHelper.hpp"

#endif
