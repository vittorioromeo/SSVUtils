#ifndef SSVU_TOKENIZER
#define SSVU_TOKENIZER

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "SSVUtils/Utils/UtilsContainers.h"
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	template<typename T> class Token
	{
		private:
			T type;
			std::string str;

		public:
			Token(T mType, const std::string& mStr) : type{mType}, str{mStr} { }

			inline const T& getType() const { return type; }
			inline const std::string& getStr() const { return str; }
	};

	template<typename T> class Tokenizer
	{
		public:
			struct RuleWord
			{
				std::function<T(const std::string&)> getType;
				std::function<bool(char)> isValidStart, isValid;

				RuleWord(std::function<T(const std::string&)> mGetType, std::function<bool(char)> mIsValidStart, std::function<bool(char)> mIsValid) : getType(mGetType), isValidStart(mIsValidStart), isValid(mIsValid) { }
			};

			struct RuleChar
			{
				T type;
				char tokenChar;

				RuleChar(T mType, char mTokenChar) : type{mType}, tokenChar{mTokenChar} { }
			};

		private:
			std::vector<RuleChar> ruleChars;
			std::vector<RuleWord> ruleWords;
			std::vector<Token<T>> tokens;
			std::string source;
			std::string::iterator itr, tokenStart;
			bool finished{false};

			void advance() { ++itr; if(itr >= std::end(source)) finished = true; }
			void emitToken(T mTokenType) { tokens.push_back({mTokenType, std::string(tokenStart, itr)}); }

		public:
			inline void setSource(const std::string& mSource) { source = mSource; }
			inline const std::vector<Token<T>>& getTokens() const { return tokens; }

			template<typename... TArgs> inline void createRuleChar(TArgs&&... mArgs) { ruleChars.emplace_back(std::forward<TArgs>(mArgs)...); }
			template<typename... TArgs> inline void createRuleWord(TArgs&&... mArgs) { ruleWords.emplace_back(std::forward<TArgs>(mArgs)...); }

			void process()
			{
				itr = std::begin(source);
				tokenStart = itr;

				while(!finished)
				{
					tokenStart = itr;

					bool found{false};
					for(auto& r : ruleWords)
					{
						if(r.isValidStart(*itr))
						{
							found = true;

							while(r.isValid(*itr)) advance();
							emitToken(r.getType(std::string(tokenStart, itr)));
							goto end;
						}
					}

					for(auto& r : ruleChars)
					{
						found = true;

						if(r.tokenChar != *itr) continue;
						advance(); emitToken(r.type);
						goto end;
					}

					end:
					if(!found) finished = true;
				}
			}
	};
}

#endif
