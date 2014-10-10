
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

			if(settings == Settings::MaintainUnexisting)
			{
				// Write the template symbols back
				appendTo(bufResult, "{{", bufKey, "}}");
			}

			return false;
		}

		inline bool Expander::replaceSection()
		{
			auto tmpb(bufKey);

			for(const auto* cd(&dict); cd != nullptr; cd = cd->parentDict)
			{
				if(cd->sections.count(bufKey) <= 0) continue;

				auto& dictVec(cd->sections.at(bufKey));
				if(dictVec.empty()) continue;

				// Separated expansions
				for(auto i(0u); i < dictVec.size() - 1; ++i)
					dictVec[i].expandImpl(src, bufResult, bufKey, sectIdxCntStart, sectIdxCntEnd, true, settings);

				// Non-separated expansion
				dictVec[dictVec.size() - 1].expandImpl(src, bufResult, bufKey, sectIdxCntStart, sectIdxCntEnd, false, settings);

				return true;
			}

			if(settings == Settings::MaintainUnexisting)
			{
				// Write the whole section back (both symbols and contents)
				for(auto i(sectIdxStart); i < sectIdxEnd; ++i) bufResult += getC(i);
			}
			else if(settings == Settings::MaintainUnexistingOnlyCnt)
			{
				// Write only the sections contents back, once
				for(auto i(sectIdxCntStart); i < sectIdxCntEnd; ++i) bufResult += getC(i);
			}

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

				// Beginning of "{{..."
				sectIdxStart = idx;

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
				SSVU_ASSERT(type == Type::Section);

				// Skip second '}'
				SSVU_ASSERT(getC() == '}' && getC(idx + 1) != '}');
				++idx;

				sectIdxCntStart = sectIdxCntEnd = idx;

				// Find section end
				for(; true; ++idx)
				{
					// Skip non-special characters or escaped special characters
					if(getC() != '{') continue;
					if(getC(idx - 1) == '\\') continue;

					sectIdxCntEnd = idx;

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

						// Ending of "...}}"
						sectIdxEnd = idx + 1;

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
