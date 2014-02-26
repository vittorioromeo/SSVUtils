// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT_INL
#define SSVU_CORE_ASSERT_INL

// This file implements `assertImpl`, which required additional dependencies.

namespace ssvu
{
	namespace Internal
	{
		inline void assertImpl(bool mExpression, const std::string& mMsg) noexcept
		{
			if(mExpression) return;

			// Data must be cached because other assertions may be tested during logging
			auto cachedData(getAssertState());

			ssvu::lo() << "\n";
			ssvu::lo("ASSERTION FAILED") << mMsg << "\n\n"
				<< "Line " << cachedData.line << " in file " << cachedData.file << "\n"
				<< "Code: " << cachedData.code << "\n" << std::endl;

			if(getAssertState().skip)
			{
				ssvu::lo() << "Skipping assertion..." << std::endl;
				return;
			}

			ssvu::lo() << "Choose what to do:\n\n"
				<< "(0) -> Skip this assertion.\n"
				<< "(1) -> Skip all assertions.\n"
				<< "(2) -> Terminate the program." << std::endl;

			std::string userInput;
			std::cin >> userInput;

			if(userInput == "0") { return; }
			if(userInput == "1") { getAssertState().skip = true; return; }

			std::terminate();
		}
	}
}

#endif
