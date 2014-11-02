// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BIMAP_STRINGIFIER
#define SSVU_IMPL_BIMAP_STRINGIFIER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	// Stringifier support
	template<typename T1, typename T2> struct Stringifier<Bimap<T1, T2>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Bimap<T1, T2>& mValue)
		{
			auto itrBegin(std::begin(mValue));
			if(itrBegin == std::end(mValue)) { Internal::printBold<TFmt>(mStream, "{ EMPTY }"); return; }

			Internal::printBold<TFmt>(mStream, "{");

			Internal::repeatPenultimate(itrBegin, std::end(mValue), [&mStream](const auto& mE)
			{
				Internal::callStringifyImpl<TFmt>(mStream, mE->first);
				Internal::printBold<TFmt>(mStream, " <-> ");
				Internal::callStringifyImpl<TFmt>(mStream, mE->second);
			}, [&mStream](const auto&){ Internal::printBold<TFmt>(mStream, "\n"); });

			Internal::printBold<TFmt>(mStream, "}");
		}
	};
}

#endif
