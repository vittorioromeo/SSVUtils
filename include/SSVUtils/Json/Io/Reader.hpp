// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO_READER
#define SSVU_JSON_IO_READER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			inline bool isValidEscapeSequenceChar(char mC) noexcept
			{
				return mC == '"' || mC == '\\' || mC == '/' || mC == 'b' || mC == 'f' || mC == 'n' || mC == 'r' || mC == 't';
			}

			inline char getEscapeSequence(char mC) noexcept
			{
				SSVU_ASSERT(isValidEscapeSequenceChar(mC));

				switch(mC)
				{
					case '"': case '\\': case '/': return mC;

					case 'b': return '\b';
					case 'f': return '\f';
					case 'n': return '\n';
					case 'r': return '\r';
					case 't': return '\t';
					default: SSVU_UNREACHABLE();
				}
			}

			template<typename TRS = ReaderSettings<RMode::Default>> class Reader
			{
				private:
					std::string src;
					Idx idx{0u};

					inline void throwError(std::string mTitle, std::string mBody)
					{
						throw ReadException{std::move(mTitle), std::move(mBody), getErrorSrc()};
					}

					inline void purgeSource()
					{
						for(auto i(0u); i < src.size(); ++i)
						{
							// Skip strings
							if(getC(i) == '"')
							{
								// Skip opening '"'
								++i;

								// Move until closing '"', skipping '\"'
								while(getC(i) != '"' || getC(i - 1) == '\\') ++i;

								// Skip closing '"'
								++i;
							}

							// Check for src end
							if(i >= src.size()) return;

							// Detect C++-style comment
							if(getC(i) != '/' || getC(i + 1) != '/') continue;

							// Replace comment with whitespace
							for(; getC(i) != '\n'; ++i) getC(i) = ' ';
						}
					}

					inline auto getErrorSrc()
					{
						auto iStart(std::max(Idx(0), idx - 20));
						auto iEnd(std::min(src.size() - 1, idx + 20));

						auto iDStart(std::max(Idx(0), idx - 4));
						auto iDEnd(std::min(src.size() - 1, idx + 4));

						auto strMarked
						(
							std::string{std::begin(src) + iStart, std::begin(src) + iDStart}
							+ " o>>> "
							+ std::string{std::begin(src) + iDStart, std::begin(src) + iDEnd}
							+ " <<<o "
							+ std::string{std::begin(src) + iDEnd, std::begin(src) + iEnd}
						);

						auto strUnmarked(std::string{std::begin(src) + iStart, std::begin(src) + iEnd});

						replaceAll(strMarked, "\n", "");
						replaceAll(strUnmarked, "\n", "");

						return strUnmarked + "\n" + strMarked;
					}

					inline static constexpr auto isWhitespace(char mC) noexcept	{ return mC == ' ' || mC == '\t' || mC == '\r' || mC == '\n'; }
					inline static constexpr auto isNumStart(char mC) noexcept	{ return mC == '-' || isDigit(mC); }

					inline char& getC() noexcept						{ SSVU_ASSERT(idx >= 0 && idx < src.size());	return src[idx]; }
					inline char getC() const noexcept					{ SSVU_ASSERT(idx >= 0 && idx < src.size());	return src[idx]; }
					inline char& getC(std::size_t mIdx) noexcept		{ SSVU_ASSERT(mIdx >= 0 && mIdx < src.size());	return src[mIdx]; }
					inline char getC(std::size_t mIdx) const noexcept	{ SSVU_ASSERT(mIdx >= 0 && mIdx < src.size());	return src[mIdx]; }

					inline auto isC(char mC) const noexcept	{ return getC() == mC; }
					inline auto isCDigit() const noexcept	{ return isDigit(getC()); }

					inline void skipWhitespace() noexcept { while(isWhitespace(getC())) ++idx; }

					template<std::size_t TS> inline void match(const char(&mKeyword)[TS])
					{
						for(auto i(0u); i < TS - 1; ++i)
						{
							if(getC() != mKeyword[i]) throwError("Invalid keyword", std::string{"Couldn't match keyword `"} + std::string{mKeyword} + "'");
							++idx;
						}
					}

					inline Str readStr()
					{
						// Skip opening '"'
						++idx;

						// Find end index of the string
						auto end(idx);
						for(; true; ++end)
						{
							// End of the string
							if(getC(end) == '"') break;

							// Skip escape sequences
							if(getC(end) == '\\')
							{
								++end;
								SSVU_ASSERT(isValidEscapeSequenceChar(getC(end)));
								continue;
							}
						}

						// Reserve memory for the string (BOTTLENECK)
						Str result;
						result.reserve(end - idx);

						for(; idx < end; ++idx)
						{
							// Not an escape sequence
							if(!isC('\\')) { result += getC(); continue; }

							// Escape sequence: skip '\'
							++idx;

							// Convert escape sequence
							result += getEscapeSequence(getC());
						}

						// Skip closing '"'
						++idx;

						return result;
					}

					inline Val parseNll()		{ match("null"); return Val{Nll{}}; }
					inline Val parseBlnFalse()	{ match("false"); return Val{false}; }
					inline Val parseBlnTrue()	{ match("true"); return Val{true}; }

					inline Val parseNum()
					{
						char* endChar;

						Real realN(static_cast<Real>(std::strtod(src.data() + idx, &endChar)));
						IntS intSN(static_cast<IntS>(realN));

						idx = endChar - src.data();

						auto isDecimal(intSN != realN);
						if(isDecimal) return Val{Num{realN}};

						return Val{Num{intSN}};
					}

					inline Val parseStr() { return Val{readStr()}; }

					inline Val parseArr()
					{
						Arr arr;

						// Skip '['
						++idx;

						skipWhitespace();

						// Empty array
						if(isC(']')) goto end;

						// Reserve some memory
						arr.reserve(10);

						while(true)
						{
							// Get value
							skipWhitespace();
							arr.emplace_back(parseVal());
							skipWhitespace();

							// Check for another value
							if(isC(',')) { ++idx; continue; }

							// Check for end of the array
							if(isC(']')) break;

							throwError("Invalid array", std::string{"Expected either `,` or `]`, got `"} + getC() + "`");
						}

						end:

						// Skip ']'
						++idx;

						return Val{arr};
					}

					inline Val parseObj()
					{
						Obj obj;

						// Skip '{'
						++idx;

						skipWhitespace();

						// Empty object
						if(isC('}')) goto end;

						// Reserve some memory
						obj.reserve(10);

						while(true)
						{
							// Read string key
							skipWhitespace();
							if(!isC('"')) throwError("Invalid object", std::string{"Expected `\"` , got `"} + getC() + "`");
							auto key(readStr());

							// Read ':'
							skipWhitespace();
							if(!isC(':')) throwError("Invalid object", std::string{"Expected `:` , got `"} + getC() + "`");

							// Skip ':'
							++idx;

							// Read value
							skipWhitespace();
							obj[std::move(key)] = parseVal();
							skipWhitespace();

							// Check for another key-value pair
							if(isC(',')) { ++idx; continue; }

							// Check for end of the object
							if(isC('}')) break;

							throwError("Invalid object", std::string{"Expected either `,` or `}`, got `"} + getC() + "`");
						}

						end:

						// Skip '}'
						++idx;

						return Val{obj};
					}

				public:
					template<typename T> inline Reader(T&& mSrc) : src{fwd<T>(mSrc)}
					{
						if(!TRS::noComments) purgeSource();
					}

					inline Val parseVal()
					{
						skipWhitespace();

						// Check value type
						switch(getC())
						{
							case '{': return parseObj();
							case '[': return parseArr();
							case '"': return parseStr();
							case 't': return parseBlnTrue();
							case 'f': return parseBlnFalse();
							case 'n': return parseNll();
						}

						// Check if value is a number
						if(isNumStart(getC())) return parseNum();

						throwError("Invalid value", std::string{"No match for values beginning with `"} + getC() + "`");
					}
			};
		}
	}
}

#endif
