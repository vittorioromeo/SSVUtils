// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT_INL
#define SSVU_CORE_ASSERT_INL

// This file implements `assertImpl`, which requires additional dependencies.

#if !(SSVU_IMPL_ASSERT_DISABLED)

namespace ssvu
{
	namespace Internal
	{
		inline void assertImpl(AssertData&& mAD, bool mExpression, const std::string& mMsg) noexcept
		{
			if(SSVU_LIKELY(mExpression)) return;

			lo() << "\n";
			lo("ASSERTION FAILED") << mMsg << "\n\n"
				<< "Line " << mAD.line << " in file " << mAD.file << "\n"
				<< "Code: " << mAD.code << "\n\n";

			if(getAssertState().skip)
			{
				lo() << "Skipping assertion..." << std::endl;
				return;
			}

			lo() << "Choose what to do:\n\n"
				<< "(  0  ) -> Skip this assertion.\n"
				<< "(  1  ) -> Skip all assertions.\n"
				<< "( ... ) -> Terminate the program." << std::endl;

			std::string userInput;
			std::cin >> userInput;

			if(userInput == "0") { return; }
			if(userInput == "1") { getAssertState().skip = true; return; }

			std::terminate();
		}
	}
}

#endif

#endif
