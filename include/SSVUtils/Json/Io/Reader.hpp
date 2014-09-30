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
			inline char getEscapeSequence(char mC) noexcept
			{
				switch(mC)
				{
					case '"': case '\\': case '/': return mC;

					case 'b': return '\b';
					case 'f': return '\f';
					case 'n': return '\n';
					case 'r': return '\r';
					case 't': return '\t';
				}

				SSVU_ASSERT(false);
				std::terminate();
			}

			class Reader
			{
				private:
					std::string src;
					Idx idx{0u};

					inline void purgeSource()
					{
						for(auto i(0u); i < src.size(); ++i)
						{
							// Skip strings
							if(src[i] == '"')
							{
								// Skip opening '"'
								++i;

								// Move until closing '"', skipping '\"'
								while(src[i] != '"' || src[i - 1] == '\\') ++i;

								// Skip closing '"'
								++i;
							}

							// Detect C++-style comment
							if(src[i] != '/' || src[i + 1] != '/') continue;

							// Replace comment with whitespace
							for(; src[i] != '\n'; ++i) src[i] = ' ';
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

					inline char getC() const noexcept		{ return src[idx]; }
					inline auto isC(char mC) const noexcept	{ return getC() == mC; }
					inline auto isCDigit() const noexcept	{ return isDigit(getC()); }

					inline void skipWhitespace() noexcept { while(isWhitespace(getC())) ++idx; }

					template<std::size_t TS> inline void match(const char(&mKeyword)[TS])
					{
						for(auto i(0u); i < TS - 1; ++i)
						{
							if(getC() != mKeyword[i]) throw ReadException("Invalid keyword", std::string{"Couldn't match keyword `"} + std::string{mKeyword} + "'", getErrorSrc());
							++idx;
						}
					}

					inline auto readStr()
					{
						// Skip opening '"'
						++idx;

						// Find end index of the string
						auto end(idx);
						for(; true; ++end)
						{
							// End of the string
							if(src[end] == '"') break;

							// TODO: assert escape sequence validity
							// Skip escape sequences
							if(src[end] == '\\') { ++end; continue; }
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

					inline auto parseNll()		{ match("null"); return Val{Nll{}}; }
					inline auto parseBlnFalse()	{ match("false"); return Val{false}; }
					inline auto parseBlnTrue()	{ match("true"); return Val{true}; }

					inline auto parseNum()
					{
						char* endChar;

						auto realN(static_cast<Num::Real>(std::strtod(src.data() + idx, &endChar)));
						auto intSN(static_cast<Num::IntS>(realN));

						idx = endChar - src.data();

						auto isDecimal(intSN != realN);
						return Val{Num{isDecimal ? realN : intSN}};
					}

					inline auto parseStr() { return Val{readStr()}; }

					inline auto parseArr()
					{
						Arr array;

						// Skip '['
						++idx;

						skipWhitespace();

						// Empty array
						if(isC(']')) goto end;

						while(true)
						{
							// Get value
							skipWhitespace();
							array.emplace_back(parseVal());
							skipWhitespace();

							// Check for another value
							if(isC(',')) { ++idx; continue; }

							// Check for end of the array
							if(isC(']')) break;

							throw ReadException{"Invalid array", "Expected either `,` or `]`, got `"s + getC() + "`", getErrorSrc()};
						}

						end:

						// Skip ']'
						++idx;

						return Val{array};
					}

					inline auto parseObj()
					{
						Obj object;

						// Skip '{'
						++idx;

						skipWhitespace();

						// Empty object
						if(isC('}')) goto end;

						while(true)
						{
							// Read string key
							skipWhitespace();
							if(!isC('"')) throw ReadException{"Invalid object", "Expected `\"` , got `"s + getC() + "`", getErrorSrc()};
							auto key(readStr());

							// Read ':'
							skipWhitespace();
							if(!isC(':')) throw ReadException{"Invalid object", "Expected `:` , got `"s + getC() + "`", getErrorSrc()};

							// Skip ':'
							++idx;

							// Read value
							skipWhitespace();
							object[key] = parseVal();
							skipWhitespace();

							// Check for another key-value pair
							if(isC(',')) { ++idx; continue; }

							// Check for end of the object
							if(isC('}')) break;

							throw ReadException{"Invalid object", "Expected either `,` or `}`, got `"s + getC() + "`", getErrorSrc()};
						}

						end:

						// Skip '}'
						++idx;

						return Val{object};
					}

				public:
					inline Reader(std::string mSrc) : src{std::move(mSrc)} { purgeSource(); }

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

						throw ReadException{"Invalid value", "No match for values beginning with `"s + getC() + "`", getErrorSrc()};
					}
			};
		}
	}
}

#endif
