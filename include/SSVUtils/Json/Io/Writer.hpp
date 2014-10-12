// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO_WRITER
#define SSVU_JSON_IO_WRITER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename TWS = WSPretty> class Writer
			{
				private:
					using FmtCC = Console::Color;
					using FmtCS = Console::Style;

					std::string out;
					SizeT depth{0};
					bool needIndent{false};

					inline auto isObjOrArr(const Val& mVal)
					{
						return mVal.getType() == Val::Type::Obj || mVal.getType() == Val::Type::Arr;
					}

					inline void indent()
					{
						for(auto i(0u); i < depth; ++i) out += "    ";
						needIndent = false;
					}

					inline void wFmt(FmtCC mColor, FmtCS mStyle = FmtCS::None)
					{
						if(!TWS::fmt) return;
						appendTo(out, Console::resetFmt(), Console::setColorFG(mColor), Console::setStyle(mStyle));
					}

					inline void wNL() { if(TWS::pretty) { out += "\n"; needIndent = true; } }
					inline void wWS() { if(TWS::pretty) { out += " "; } }

					inline void wOut(const std::string& mStr)
					{
						if(TWS::pretty)
						{
							if(needIndent) indent();
						}

						out += mStr;
					}

					template<typename TItr, typename TF1, typename TF2> inline void repeatWithSeparator(TItr mBegin, TItr mEnd, TF1 mF1, TF2 mF2)
					{
						if(mBegin == mEnd) return;
						for(; mBegin != std::prev(mEnd); ++mBegin) { mF1(mBegin); mF2(); }
						mF1(mBegin);
					}



					inline void write(const Obj& mObj)
					{
						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wOut("{"); wNL();

						++depth;

						repeatWithSeparator(std::begin(mObj), std::end(mObj), [this](auto mItr)
						{
							writeKey(mItr->first);

							wFmt(FmtCC::LightGray, FmtCS::Bold);
							wOut(":"); wWS();

							if(isObjOrArr(mItr->second)) wNL();

							write(mItr->second);
						}, [this]{ wOut(","); wWS(); wNL(); });

						--depth;

						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wNL(); wOut("}");
					}

					inline void write(const Arr& mArr)
					{
						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wOut("["); wNL();

						++depth;

						repeatWithSeparator(std::begin(mArr), std::end(mArr), [this](auto mItr)
						{
							write(*mItr);
						}, [this]
						{
							wFmt(FmtCC::LightGray, FmtCS::Bold);
							wOut(","); wWS(); wNL();
						});

						--depth;

						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wNL(); wOut("]");
					}

					inline void writeKey(const Key& mKey)
					{
						wFmt(FmtCC::LightGray);
						wOut("\"" + mKey + "\"");
					}

					inline void write(const Str& mStr)
					{
						wFmt(FmtCC::LightYellow);
						wOut("\"" + mStr + "\"");
					}

					inline void write(const Num& mNum)
					{
						wFmt(FmtCC::LightRed);

						switch(mNum.getRepr())
						{
							case Num::Repr::IntS: wOut(toStr(mNum.as<IntS>())); break;
							case Num::Repr::IntU: wOut(toStr(mNum.as<IntU>())); break;
							case Num::Repr::Real: wOut(toStr(mNum.as<Real>())); break;
						}
					}

					inline void write(Bln mBool)
					{
						wFmt(FmtCC::LightCyan);
						wOut(mBool ? "true" : "false");
					}

					inline void write(Nll)
					{
						wFmt(FmtCC::LightMagenta);
						wOut("null");
					}

					void write(const Val& mVal);

				public:
					inline void write(const Val& mVal, std::ostream& mStream) { write(mVal); mStream << out; }
			};
		}
	}
}

#endif
