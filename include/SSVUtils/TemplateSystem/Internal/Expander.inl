
// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_EXPANDER_INL
#define SSVU_TEMPLATESYSTEM_EXPANDER_INL

namespace ssvu
{
	namespace TemplateSystem
	{
		inline bool Expander::replace()
		{
			for(const auto* cd(&dict); cd != nullptr; cd = cd->parentDict)
			{
				if(cd->replacements.count(bufKey) <= 0) continue;

				bufResult += cd->replacements.at(bufKey);
				return true;
			}

			if(maintainNotFound) appendTo(bufResult, "{{", bufKey, "}}");
			return false;
		}

		inline bool Expander::replaceSection()
		{
			for(const auto* cd(&dict); cd != nullptr; cd = cd->parentDict)
			{
				if(cd->sections.count(bufKey) <= 0) continue;

				auto& dictVec(cd->sections.at(bufKey));
				if(dictVec.empty()) continue;

				// Separated expansions
				for(auto i(0u); i < dictVec.size() - 1; ++i)
					dictVec[i].expandImpl(src, bufResult, bufKey, sectIdxStart, sectIdxEnd, true, maintainNotFound);

				// Non-separated expansion
				dictVec[dictVec.size() - 1].expandImpl(src, bufResult, bufKey, sectIdxStart, sectIdxEnd, false, maintainNotFound);

				return true;
			}

			if(maintainNotFound) appendTo(bufResult, "{{#", bufKey, "}}");
			else Dictionary{}.expandImpl(src, bufResult, bufKey, sectIdxStart, sectIdxEnd, false, maintainNotFound);

			return false;
		}

		inline bool Expander::expand()
		{
			bool result{false};
			enum class Type{Normal, Section, Separator};

			for(; idx < idxEnd; ++idx)
			{
				SSVU_ASSERT(idx >= 0 && idx < src.size());

				// Skip non-special characters or escaped special characters
				if(getC() != '{' || (idx > 0 && getC(idx - 1) == '\\') || getC(idx + 1) != '{')
				{
					bufResult += getC();
					continue;
				}

				// "{{" combination
				// Skip "{{"
				idx += 2;

				Type type{Type::Normal};

				// Get eventual section/separator symbol and skip it
				switch(getC())
				{
					case '#': type = Type::Section; ++idx; break;
					case '*': type = Type::Separator; ++idx; break;
				}

				// Read key into `bufKey`
				bufKey.clear();
				for(; getC() != '}'; ++idx) bufKey += getC();

				SSVU_ASSERT(getC() == '}' && getC(idx + 1) == '}');

				// Skip first '}', second one will be skipped by the for's `idx` increment or by section increment
				++idx;

				// Not a section
				if(type == Type::Normal)	{ if(replace()) result = true; continue; }
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

				if(replaceSection()) result = true;
			}

			return result;
		}
	}
}

#endif
