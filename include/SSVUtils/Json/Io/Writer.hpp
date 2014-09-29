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
			template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> class Writer
			{
				private:
					using FmtCC = Console::Color;
					using FmtCS = Console::Style;

					std::string out;
					std::size_t depth{0};
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
						if(!TFmt) return;

						out += Console::resetFmt();
						out += Console::setColorFG(mColor);
						out += Console::setStyle(mStyle);
					}

					inline void wNL() { if(TWS == WriterMode::Pretty) { out += "\n"; needIndent = true; } }
					inline void wWS() { if(TWS == WriterMode::Pretty) { out += " "; } }

					inline void wOut(const std::string& mStr)
					{
						if(TWS == WriterMode::Pretty)
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

						switch(mNum.getType())
						{
							case Num::Type::IntS:	wOut(toStr(mNum.as<Num::IntS>())); break;
							case Num::Type::IntU:	wOut(toStr(mNum.as<Num::IntU>())); break;
							case Num::Type::Real:	wOut(toStr(mNum.as<Num::Real>())); break;
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

					inline void write(const Val& mVal)
					{
						switch(mVal.getType())
						{
							case Val::Type::Obj:	write(mVal.as<Obj>()); break;
							case Val::Type::Arr:	write(mVal.as<Arr>()); break;
							case Val::Type::Str:	write(mVal.as<Str>()); break;
							case Val::Type::Num:	write(mVal.as<Num>()); break;
							case Val::Type::Bln:		write(mVal.as<Bln>()); break;
							case Val::Type::Nll:		write(Nll{}); break;
						}
					}

				public:
					inline void write(const Val& mVal, std::ostream& mStream) { write(mVal); mStream << out; }
			};
		}
	}
}

#endif
