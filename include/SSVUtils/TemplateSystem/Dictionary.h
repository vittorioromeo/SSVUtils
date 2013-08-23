// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_DICTIONARY
#define SSVU_TEMPLATESYSTEM_DICTIONARY

#include <string>
#include <vector>
#include <map>
#include "SSVUtils/TemplateSystem/Internal/Symbols.h"
#include "SSVUtils/String/Utils.h"

namespace ssvu
{
	namespace TemplateSystem
	{
		/*!
		 *
		 * @brief Dictionary used for string-template replacements.
		 *
		 * Inspired by CTemplate. It's used to generate data from string templates.
		 *
		 */
		class Dictionary
		{
			private:
				std::map<std::string, std::string> replacements; /*!< Simple key -> string replacements. */
				std::map<std::string, std::vector<Dictionary>> sectionDictionaries; /*!< Key -> subdictionary section replacements. */

			public:
				/*!
				 *
				 * @brief Default constructor.
				 *
				 * Initializes an empty Dictionary.
				 *
				 */
				Dictionary() = default;

				/*!
				 *
				 * @brief std::initializer_list constructor.
				 *
				 * Initializes a dictionary filled with "key -> string" data.
				 *
				 * @code
				 * using namespace ssvu::TemplateSystem;
				 * Dictionary dict{{"key1", "value1"}, {"key2", "value2"}};
				 * string toExpand{".{{key1}}.{{key2}}."};
				 * assert(dict.getExpanded(toExpand) == ".value1.value2.");
				 * @endcode
				 *
				 */
				Dictionary(const std::initializer_list<std::pair<std::string, std::string>>& mPairs) { for(const auto& p : mPairs) replacements.insert(p); }

				/*!
				 *
				 * @brief Expands the dictionary.
				 *
				 * Expands the dictionary, along with subdictionary sections, to a string.
				 * Takes a string parameter which is the template (not a path to the template file).
				 * Consider using ssvu::FileSystem::getFileContents(...) to pass a template file.
				 *
				 * @code
				 * using namespace ssvu::TemplateSystem;
				 * Dictionary dict{{"key1", "value1"}, {"key2", "value2"}};
				 * string toExpand{".{{key1}}.{{key2}}."};
				 * assert(dict.getExpanded(toExpand) == ".value1.value2.");
				 * @endcode
				 *
				 * @param mString Template to use (not a path!).
				 *
				 * @return Returns a string containing the expanded dictionary template.
				 *
				 */
				std::string getExpanded(std::string mString) const
				{
					using namespace Internal;

					for(const auto& p : sectionDictionaries)
					{
						const std::string sectionStart{getSectionStart(p.first)}, sectionEnd{getSectionEnd(p.first)};
						const auto outerSectionStartIndex(mString.find(sectionStart));
						const auto innerSectionEndIndex(mString.find(sectionEnd));
						const auto innerSectionStartIndex(outerSectionStartIndex + sectionStart.size());
						const auto outerSectionEndIndex(innerSectionEndIndex + sectionEnd.size());

						const std::string innerSection{mString.substr(innerSectionStartIndex, innerSectionEndIndex - innerSectionStartIndex)};
						std::string innerSectionResult;

						for(auto d : p.second) innerSectionResult += d.getExpanded(innerSection);

						mString.replace(outerSectionStartIndex, outerSectionEndIndex - outerSectionStartIndex, innerSectionResult);
					}

					for(const auto& p : replacements) replaceAll(mString, getKey(p.first), p.second);

					// TODO: cleanup unexpanded sections/replacements? (use regex!)
					// TODO: newline stripping options?

					return mString;
				}

				/*!
				 *
				 * @brief Sets a "key -> string" replacement.
				 *
				 * Works similarly to std::map::operator[].
				 *
				 * @code
				 * using namespace ssvu::TemplateSystem;
				 * Dictionary dict;
				 * dict["key1"] = "value1";
				 * dict["key2"] = "value2";
				 * string toExpand{".{{key1}}.{{key2}}."};
				 * assert(dict.getExpanded(toExpand) == ".value1.value2.");
				 * @endcode
				 *
				 * @param mString Key of the "key -> string" replacement.
				 *
				 * @return Returns a reference to the string used in the "key -> string" replacement. Set it to the value you desire.
				 *
				 */
				inline std::string& operator[](const std::string& mKey) { return replacements[mKey]; }

				/*!
				 *
				 * @brief Adds a "key -> subdictionary" replacement
				 *
				 * Works similarly to ssvu::Delegate::operator+=.
				 *
				 * @code
				 * using namespace ssvu::TemplateSystem;
				 * Dictionary dict;
				 * Dictionary subdict1{{"key", "value1"}};
				 * Dictionary subdict2{{"key", "value2"}};
				 *
				 * dict += {"section", subdict1};
				 * dict += {"section", subdict2};
				 *
				 * string toExpand{"{{#section}}.{{key}}.{{/section}}"};
				 * assert(dict.getExpanded(toExpand) == ".value1..value2.");
				 * @endcode
				 *
				 * @param mPair Pair containing the key of the section name and the subdictionary to use in the replacement.
				 *
				 */
				inline void operator+=(const std::pair<std::string, Dictionary>& mPair) { sectionDictionaries[mPair.first].push_back(mPair.second); }
		};
	}
}

#endif
