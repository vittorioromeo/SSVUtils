// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL
#define SSVU_JSON_VAL

#include "SSVUtils/Json/Val/Internal/Fwd.hpp"
#include "SSVUtils/Json/Val/Internal/ItrHelper.hpp"

namespace ssvu
{
	namespace Json
	{
		class Val
		{
			template<typename> friend struct Internal::Cnv;
			template<typename> friend struct Internal::Chk;
			template<typename> friend struct Internal::AsHelper;
			friend struct Internal::TplCnvHelper;
			friend struct Internal::TplIsHelper;
			friend struct Internal::CnvFuncHelper;

			public:
				/// @brief Internal storage type.
				enum class Type{Obj, Arr, Str, Num, Bln, Nll};

				/// @typedef `Obj` implementation type, templatized with `Val`.
				using Obj = Internal::ObjImpl<Val>;

				/// @typedef `Arr` implementation type, templatized with `Val`.
				using Arr = Internal::ArrImpl<Val>;

			private:
				// Shortcut typedefs
				using Num = Internal::Num;
				using VIH = Internal::ItrHelper;

				/// @brief Current storage type.
				Type type{Type::Nll};

				/// @brief Checked union storage for `Val` fundamental types.
				UnionVariant<Obj, Arr, Str, Num, Bln> h;

				// Perfect-forwarding setters
				template<typename T> inline void setObj(T&& mX) noexcept(noexcept(Obj{fwd<T>(mX)}))	{ type = Type::Obj; h.init<Obj>(fwd<T>(mX)); }
				template<typename T> inline void setArr(T&& mX) noexcept(noexcept(Arr{fwd<T>(mX)}))	{ type = Type::Arr; h.init<Arr>(fwd<T>(mX)); }
				template<typename T> inline void setStr(T&& mX) noexcept(noexcept(Str{fwd<T>(mX)}))	{ type = Type::Str; h.init<Str>(fwd<T>(mX)); }
				template<typename T> inline void setNum(T&& mX) noexcept(noexcept(Num{fwd<T>(mX)}))	{ type = Type::Num; h.init<Num>(fwd<T>(mX)); }

				// Basic setters
				inline void setBln(Bln mX) noexcept	{ type = Type::Bln; h.init<Bln>(mX); }
				inline void setNll(Nll) noexcept	{ type = Type::Nll; }

				// Ref-qualified getters
				#define SSVJ_DEFINE_VAL_GETTER(mType, mMember) \
					inline mType&		SSVPP_CAT(get, mType)() & noexcept		{ SSVU_ASSERT(is<mType>()); return mMember; } \
					inline const mType&	SSVPP_CAT(get, mType)() const& noexcept	{ SSVU_ASSERT(is<mType>()); return mMember; } \
					inline mType		SSVPP_CAT(get, mType)() && noexcept		{ SSVU_ASSERT(is<mType>()); return std::move(mMember); }

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
						case Type::Obj: h.deinit<Obj>(); break;
						case Type::Arr: h.deinit<Arr>(); break;
						case Type::Str: h.deinit<Str>(); break;
						case Type::Num: h.deinit<Num>(); break;
						case Type::Bln: h.deinit<Bln>(); break;
						default: break;
					}
				}

				/// @brief Initializes the current storage from `mV`, calling the appropriate setter function.
				template<typename T> inline void init(T&& mV)
				{
					switch(mV.type)
					{
						case Type::Obj: setObj(moveIfRValue<decltype(mV)>(mV.getObj())); break;
						case Type::Arr: setArr(moveIfRValue<decltype(mV)>(mV.getArr())); break;
						case Type::Str: setStr(moveIfRValue<decltype(mV)>(mV.getStr())); break;
						case Type::Num: setNum(mV.getNum()); break;
						case Type::Bln: setBln(mV.getBln()); break;
						case Type::Nll: setNll({}); break;
					}
				}

				/// @brief Checks the stored type. Doesn't check number representation.
				template<typename T> inline bool isNoNum() const noexcept
				{
					return Internal::ChkNoNum<RemoveAll<T>>::is(*this);
				}

			public:
				inline Val() noexcept = default;

				// Copy/move constructors
				inline Val(const Val& mV)	{ init(mV); }
				inline Val(Val&& mV)		{ init(std::move(mV)); }

				/// @brief Constructs the `Val` from `mX`.
				template<typename T, SSVU_ENABLEIF_RA_IS_NOT(T, Val)> inline Val(T&& mX) { set(fwd<T>(mX)); }

				// Destructor must deinitalize
				inline ~Val() { deinitCurrent(); }

				/// @brief Sets the `Val`'s internal value to `mX`.
				///	@details The current stored value is deinitialized first.
				template<typename T> inline void set(T&& mX) noexcept(noexcept(Internal::Cnv<RemoveAll<T>>::toVal(std::declval<Val&>(), fwd<T>(mX))))
				{
					deinitCurrent();
					Internal::Cnv<RemoveAll<T>>::toVal(*this, fwd<T>(mX));
				}

				/// @brief Checks if the stored internal value is of type `T`.
				/// @details If checking number representation is required, only use `IntS`, `IntU` or `Real`.
				///	Any other numeric type will not work.
				template<typename T> inline bool is() const noexcept
				{
					return Internal::Chk<RemoveAll<T>>::is(*this);
				}

				/// @brief Gets the internal value as `T`. (non-const version)
				/// @details Returns a copy for most types, a reference for `Obj`, `Arr` and `Str`.
				template<typename T> auto as() & -> decltype(Internal::AsHelper<T>::as(*this));

				/// @brief Gets the internal value as `T`. (const version)
				/// @details Returns a copy for most types, a const reference for `Obj`, `Arr` and `Str`.
				template<typename T> auto as() const& -> decltype(Internal::AsHelper<T>::as(*this));

				/// @brief Gets the internal value as `T`. (rvalue version)
				/// @details Returns a copy for most types, a const reference for `Obj`, `Arr` and `Str`.
				template<typename T> auto as() && -> decltype(Internal::AsHelper<T>::as(*this));

				// "Implicit" `set` function done via `operator=` overloading
				auto& operator=(const Val& mV) noexcept;
				auto& operator=(Val&& mV) noexcept;
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

				// IO writing implementations
				template<typename TWS = WSPretty> void writeToStream(std::ostream&) const;
				template<typename TWS = WSPretty> inline void writeToStr(std::string& mStr) const			{ std::ostringstream o; writeToStream<TWS>(o); mStr = o.str(); }
				template<typename TWS = WSPretty> inline void writeToFile(const ssvufs::Path& mPath) const	{ std::ofstream o{mPath}; writeToStream<TWS>(o); o.close(); }
				template<typename TWS = WSPretty> inline auto getWriteToStr() const							{ std::string result; writeToStr<TWS>(result); return result; }

				// IO reading implementations
				template<typename TRS = RSDefault, typename T> void readFromStr(T&& mStr);
				template<typename TRS = RSDefault> inline void readFromFile(const ssvufs::Path& mPath) { readFromStr(mPath.getContentsAsString()); }

				// Construction from strings or files
				template<typename T> inline static Val fromStr(T&& mStr)	{ Val result; result.readFromStr(fwd<T>(mStr)); return result; }
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
				template<typename T> inline void emplace(T&& mX) noexcept(noexcept(std::declval<Arr>().emplace_back(fwd<T>(mX))))
				{
					getArr().emplace_back(fwd<T>(mX));
				}

				// Size getters
				inline auto getSizeArr() const noexcept { return getArr().size(); }
				inline auto getSizeObj() const noexcept { return getObj().size(); }
		};

		/// @typedef `Obj` implementation typedef, templatized with `Val`.
		using Obj = Val::Obj;

		/// @typedef `Arr` implementation typedef, templatized with `Val`.
		using Arr = Val::Arr;
	}
}

#include "SSVUtils/Json/Val/Internal/Cnv.hpp"
#include "SSVUtils/Json/Val/Internal/Chk.hpp"
#include "SSVUtils/Json/Val/Internal/AsHelper.hpp"

#endif
