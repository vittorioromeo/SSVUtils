
// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_EXPANDER_INL
#define SSVU_TEMPLATESYSTEM_EXPANDER_INL

namespace ssvu
{
	namespace TemplateSystem
	{
		inline void Expander::replace()
		{
			const Dictionary* currentDict{&dict};

			while(currentDict != nullptr)
			{
				if(currentDict->replacements.count(bufKey) > 0)
				{
					bufResult += currentDict->replacements.at(bufKey);
					return;
				}

				currentDict = currentDict->parentDict;
			}
		}

		inline void Expander::replaceSection()
		{
			const Dictionary* currentDict{&dict};

			while(currentDict != nullptr)
			{
				if(currentDict->sections.count(bufKey) <= 0) { currentDict = currentDict->parentDict; continue; }
				auto& dictVec(currentDict->sections.at(bufKey));
				if(dictVec.empty()) { currentDict = currentDict->parentDict; continue; }

				// Separated expansions
				for(auto i(0u); i < dictVec.size() - 1; ++i)
					Expander{dictVec[i], src, bufResult, bufKey, sectIdxStart, sectIdxEnd, true}.expand();

				// Non-separated expansion
				Expander{dictVec[dictVec.size() - 1], src, bufResult, bufKey, sectIdxStart, sectIdxEnd, false}.expand();

				return;
			}
		}

		inline bool Expander::expand()
		{
			bool result{false};
			enum class Type{Normal, Section, Separator};

			for(; idx < idxEnd; ++idx)
			{
				SSVU_ASSERT(idx >= 0 && idx < src.size());

				// Skip non-special characters or escaped special characters
				if(getC() != '{' || (idx > 0 && getC(idx - 1) == '\\'))
				{
					bufResult += getC();
					continue;
				}
				if(getC(idx + 1) != '{')
				{
					bufResult += getC();
					continue;
				}

				result = true;
				// "{{" combination
				// Skip "{{"
				idx += 2;

				Type type{Type::Normal};

				// Section start
				if(getC() == '#')
				{
					type = Type::Section;

					// Skip '#'
					++idx;
				}
				else if(getC() == '*')
				{
					type = Type::Separator;

					// Skip '['
					++idx;
				}

				bufKey.clear();

				for(; getC() != '}'; ++idx) bufKey += getC();

				SSVU_ASSERT(getC() == '}' && getC(idx + 1) == '}');

				// Skip first '}', second one will be skipped by the for's `idx` increment or by section increment
				++idx;

				// Not a section
				if(type == Type::Normal)	{ replace(); continue; }
				if(type == Type::Separator)	{ if(separate) bufResult += bufKey; continue; }

				// Section
				// Skip second '}'
				SSVU_ASSERT(getC() == '}' && getC(idx + 1) != '}');
				++idx;

				sectIdxStart = sectIdxEnd = idx;

				// Find section end
				for(; true; ++idx)
				{
					// Skip non-special characters or escaped special characters
					if(getC() != '{') continue;
					if(getC(idx - 1) == '\\') continue;

					sectIdxEnd = idx;

					if(getC(idx + 1) == '{' && getC(idx + 2) == '/')
					{
						idx += 3;

						// Check if section key matches
						for(auto kc : bufKey)
						{
							if(getC() != kc) goto next;
							++idx;
						}

						SSVU_ASSERT(getC() == '}' && getC(idx + 1) == '}');

						// Skip first '}', second one will be skipped by the for's `idx` increment
						++idx;
						break;
					}

					next:
					continue;
				}

				replaceSection();
			}

			return result;
		}
	}
}

#endif
